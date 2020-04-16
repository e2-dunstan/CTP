#include "Contact.h"
#include <algorithm>

void Contact::PrepareResolution()
{
	//find restitution
	switch (bounceCombineType)
	{
		case BounceCombineType::MAXIMUM:
		{
			restitution = std::max(body1->rigidbody.bounciness, body2->rigidbody.bounciness);
			break;
		}
		case BounceCombineType::MULTIPLY:
		{
			restitution = body1->rigidbody.bounciness * body2->rigidbody.bounciness;
			break;
		}
		case BounceCombineType::MINIMUM:
		{
			restitution = std::min(body1->rigidbody.bounciness, body2->rigidbody.bounciness);
			break;
		}
		case BounceCombineType::AVERAGE:
		{
			restitution = (body1->rigidbody.bounciness + body2->rigidbody.bounciness) * 0.5f;
			if (body2->type != PrimitiveType::PLANE) restitution *= 2.0f;
			break;
		}
	}

	//find friction (using average)
	friction = (body1->rigidbody.friction + body2->rigidbody.friction) * 0.5f;
	if (friction < 0.0f) friction = 0.0f;
	else if (friction > 1.0f) friction = 1.0f;

	CalculateContactBasisMatrices();

	relContactPos1 = (point - body1->translation);
	relContactPos2 = (point - body2->translation);

	CalculateClosingVelocities();
	CalculateDesiredDeltaVelocity();
}

void Contact::CalculateContactBasisMatrices()
{
	Vector3 contactTangents[2];

	//Check if Z axis neraer to the X or Y axis
	if (abs(normal.x) > abs(normal.y))
	{
		//scaling factor to ensure normalisation
		const float scalingFactor = 1.0f / (float)sqrt((normal.z * normal.z) + (normal.x * normal.x));

		//new X axis is perpendicular to the world Y axis
		contactTangents[0].x = normal.z * scalingFactor;
		contactTangents[0].y = 0;
		contactTangents[0].z = -normal.x * scalingFactor;

		//new Y axis is perpendicular to X and Z axes
		contactTangents[1].x = normal.y * contactTangents[0].x;
		contactTangents[1].y = (normal.z * contactTangents[0].x)
			- (normal.x * contactTangents[0].z);
		contactTangents[1].z = -normal.y * contactTangents[0].x;
	}
	else
	{
		const float scalingFactor = 1.0f / (float)sqrt((normal.z * normal.z) + (normal.y * normal.y));

		//new X axis is perpendicular to the world X axis
		contactTangents[0].x = 0;
		contactTangents[0].y = -normal.z * scalingFactor;
		contactTangents[0].z = normal.y * scalingFactor;

		contactTangents[1].x = (normal.y * contactTangents[0].z)
			- (normal.z * contactTangents[0].y);
		contactTangents[1].y = -normal.x * normal.z;
		contactTangents[1].z = normal.x * contactTangents[0].y;
	}
	double matVals[9] = { normal.x, contactTangents[0].x, contactTangents[1].x,
					normal.y, contactTangents[0].y, contactTangents[1].y,
					normal.z, contactTangents[0].z, contactTangents[1].z };

	contactToWorld = Matrix3(matVals);
	worldToContact = contactToWorld.GetTranspose();
}

void Contact::CalculateClosingVelocities()
{
	//Body 1
	closingVelocity = body1->rigidbody.angularVelocity.VectorProduct(relContactPos1) + body1->rigidbody.velocity;
	Mathe::Transform(closingVelocity, worldToContact);
	//global forces
	//Vector3 acc = body1->rigidbody.GetPreviousVelocity();
	//Mathe::Transform(acc, worldToContact);

	//closingVelocity += Vector3(0, acc.y, acc.z); //not interested in normal dir

	//Body2
	if (!body2->isStatic)
	{
		Vector3 closingVelocity2 = body2->rigidbody.angularVelocity.VectorProduct(relContactPos2) + body2->rigidbody.velocity;
		Mathe::Transform(closingVelocity2, worldToContact);
		//global forces
		//Vector3 acc2 = body2->rigidbody.GetPreviousVelocity();
		//Mathe::Transform(acc2, worldToContact);

		closingVelocity -= (closingVelocity2);// +Vector3(0, acc.y, acc.z));
	}
}

void Contact::CalculateDesiredDeltaVelocity()
{
	double bodiesVelocity = 0;
	if (body1->rigidbody.isAwake)
	{
		bodiesVelocity += abs(body1->rigidbody.velocity.ScalarProduct(normal));// *Global::deltaTime;
	}
	if (!body2->isStatic && body2->rigidbody.isAwake)
	{
		bodiesVelocity -= abs(body2->rigidbody.velocity.ScalarProduct(normal));// *Global::deltaTime;
	}
	float r = restitution;
	if (abs(closingVelocity.x) < 0.25) 
		r = 0.0f;

	desiredDeltaVelocity = -(float)closingVelocity.x - r * (float)(closingVelocity.x - bodiesVelocity);
}

void Contact::ResolvePenetration()
{
	// -- Nonlinear Projection -- //

	//linear impulse = lambda * normal
	//angular impulse (torque) = lambda * (relative contact position X normal)

	// -----------
	// BODY 1 inertia
	// -----------

	Vector3 angularInertia_world = relContactPos1.VectorProduct(normal);
	Mathe::Transform(angularInertia_world, body1->rigidbody.inverseInertiaTensorWorld);
	angularInertia_world = angularInertia_world.VectorProduct(relContactPos1);

	float angularInertia1 = angularInertia_world.ScalarProduct(normal);
	float linearInertia1 = body1->rigidbody.inverseMass;

	float totalInertia = linearInertia1 + angularInertia1;

	float angularInertia2 = 0;
	float linearInertia2 = 0;

	// -----------
	// BODY 2 inertia
	// -----------
	if (!body2->isStatic)
	{
		angularInertia_world = relContactPos2.VectorProduct(normal);
		Mathe::Transform(angularInertia_world, body2->rigidbody.inverseInertiaTensorWorld);
		angularInertia_world = angularInertia_world.VectorProduct(relContactPos2);

		angularInertia2 = (float)angularInertia_world.ScalarProduct(normal);
		linearInertia2 = body2->rigidbody.inverseMass;

		totalInertia += linearInertia2 + angularInertia2;
	}

	//move values
	float linearMove1 = penetrationDepth * (linearInertia1 / totalInertia);
	float linearMove2 = -penetrationDepth * (linearInertia2 / totalInertia);
	float angularMove1 = penetrationDepth * (angularInertia1 / totalInertia);
	float angularMove2 = -penetrationDepth * (angularInertia2 / totalInertia);

	//float m = (relContactPos1 - (normal * relContactPos1.ScalarProduct(normal))).SquaredMagnitude();
	//ApplyAngularMoveLimit(linearMove1, angularMove1, m/*(float)body1->scale.SquaredMagnitude());
	//m = (relContactPos2 - (normal * relContactPos2.ScalarProduct(normal))).SquaredMagnitude();
	//ApplyAngularMoveLimit(linearMove2, angularMove2, m/*(float)body2->scale.SquaredMagnitude());

	//Body 1
	if (angularMove1 != 0)
	{
		Vector3 angularDir1 = relContactPos1.VectorProduct(normal);
		Mathe::Transform(angularDir1, body1->rigidbody.inverseInertiaTensorWorld);
		angularChange[0] = angularDir1 * (angularMove1 / angularInertia1);
	}
	else angularChange[0] = Vector3();

	//Body2
	if (angularMove2 != 0 && !body2->isStatic)
	{
		Vector3 angularDir2 = relContactPos2.VectorProduct(normal);
		Mathe::Transform(angularDir2, body2->rigidbody.inverseInertiaTensorWorld);
		angularChange[1] = angularDir2 * (angularMove2 / angularInertia2);
	}
	else angularChange[1] = Vector3();

	if (linearMove1 != 0)
	{
		if (abs(linearMove1) > 0.5)
		{
			//std::cout << "WARNING: large linear change detected: " << linearMove1 << std::endl;
			//linearMove1 = 0.5;
		}
		linearChange[0] = normal * linearMove1;
		body1->translation += linearChange[0];
	}
	if (angularChange[0].SquaredMagnitude() > 0.0f)
	{
		Quaternion q;
		body1->GetOrientation(&q);
		Mathe::AddScaledVector(q, angularChange[0], 1.0, true);
		body1->SetOrientation(q);
	}
	if (linearMove2 != 0)
	{
		if (abs(linearMove2) > 0.5)
		{
			//std::cout << "WARNING: large linear change detected w/ pen depth: " << penetrationDepth << std::endl;
			linearMove2 = 1.0;
		}
		linearChange[1] = normal * linearMove2;
		body2->translation += linearChange[1];
	}
	if (angularChange[1].SquaredMagnitude() > 0.0f && !body2->isStatic)
	{
		Quaternion q;
		body2->GetOrientation(&q);
		Mathe::AddScaledVector(q, angularChange[1], 1.0, true);
		body2->SetOrientation(q);
	}

	body1->updateTransform = true;
	if (!body2->isStatic)
	{
		body2->updateTransform = true;
	}
}

void Contact::ApplyAngularMoveLimit(float& linear, float& angular, const float projection/*const float objMag*/)
{
	float angularLimit = 1.0f * projection;
	if (angularLimit == 0) angularLimit = 1.0f;
	if (abs(angular) > angularLimit)
	{
		std::cout << "WARNING: angular limit (" << angularLimit << ") reached with value of " << abs(angular) << std::endl;
		float total = linear + angular;

		if (angular >= 0) angular = angularLimit;
		else angular = -angularLimit;

		linear = total - angular;
	}
}

void Contact::ResolveVelocity()
{
	Vector3 impulse;
	if (friction == 0.0f) impulse = FrictionlessImpulse();
	else impulse = FrictionImpulse();

	Mathe::Transform(impulse, contactToWorld);

	velocityChange[0] = impulse * body1->rigidbody.inverseMass;
	rotationChange[0] = relContactPos1.VectorProduct(impulse); //impulsive torque
	Mathe::Transform(rotationChange[0], body1->rigidbody.inverseInertiaTensorWorld);

	body1->rigidbody.AddVelocityChange(velocityChange[0]);
	body1->rigidbody.AddRotationChange(rotationChange[0]);

	if (!body2->isStatic)
	{
		velocityChange[1] = impulse * body2->rigidbody.inverseMass *-1.0;
		rotationChange[1] = impulse.VectorProduct(relContactPos2); //impulsive torque
		Mathe::Transform(rotationChange[1], body2->rigidbody.inverseInertiaTensorWorld);

		body2->rigidbody.AddVelocityChange(velocityChange[1]);
		body2->rigidbody.AddRotationChange(rotationChange[1]);
	}
}

Vector3 Contact::FrictionlessImpulse()
{
	Vector3 deltaVelocity_W = relContactPos1.VectorProduct(normal);
	Mathe::Transform(deltaVelocity_W, body1->rigidbody.inverseInertiaTensorWorld);
	deltaVelocity_W = deltaVelocity_W.VectorProduct(relContactPos1);

	float deltaSpeed = (float)deltaVelocity_W.ScalarProduct(normal) + body1->rigidbody.inverseMass;

	if (!body2->isStatic)
	{
		deltaVelocity_W = relContactPos2.VectorProduct(normal);
		Mathe::Transform(deltaVelocity_W, body2->rigidbody.inverseInertiaTensorWorld);
		deltaVelocity_W = deltaVelocity_W.VectorProduct(relContactPos2);

		deltaSpeed += (float)deltaVelocity_W.ScalarProduct(normal) + body2->rigidbody.inverseMass;
	}
	return Vector3((double)desiredDeltaVelocity / (double)deltaSpeed, 0.0, 0.0);
}

Vector3 Contact::FrictionImpulse()
{
	//Matrix equivalent of cross product
	double matVals[9] = { 0.0 };
	matVals[1] = -relContactPos1.z;
	matVals[2] = relContactPos1.y;
	matVals[3] = relContactPos1.z;
	matVals[5] = -relContactPos1.x;
	matVals[6] = -relContactPos1.y;
	matVals[7] = relContactPos1.x;

	//Transpose helps with the spinning
	Matrix3 impulseToTorque = Matrix3(matVals);// .GetTranspose();

	Matrix3 impulseToVel_W = impulseToTorque;
	impulseToVel_W = (impulseToVel_W * body1->rigidbody.inverseInertiaTensorWorld) * impulseToTorque;
	impulseToVel_W = impulseToVel_W * -1.0;

	float inverseMass = body1->rigidbody.inverseMass;
	
	if (!body2->isStatic)
	{
		impulseToTorque.matrix[0] = impulseToTorque.matrix[4] = impulseToTorque.matrix[8] = 0;
		impulseToTorque.matrix[1] = -relContactPos2.z;
		impulseToTorque.matrix[2] = relContactPos2.y;
		impulseToTorque.matrix[3] = relContactPos2.z;
		impulseToTorque.matrix[5] = -relContactPos2.x;
		impulseToTorque.matrix[6] = -relContactPos2.y;
		impulseToTorque.matrix[7] = relContactPos2.x;

		Matrix3 impulseToVel_W_2 = impulseToTorque;
		impulseToVel_W_2 = (impulseToVel_W_2 * body2->rigidbody.inverseInertiaTensorWorld) * impulseToTorque;
		impulseToVel_W_2 = impulseToVel_W_2 * -1.0;

		impulseToVel_W = impulseToVel_W + impulseToVel_W_2;

		inverseMass += body2->rigidbody.inverseMass;
	}
	Matrix3 impulseToVel_C = worldToContact;
	impulseToVel_C = impulseToVel_C * impulseToVel_W;
	impulseToVel_C = impulseToVel_C * contactToWorld;

	impulseToVel_C.matrix[0] += inverseMass;
	impulseToVel_C.matrix[4] += inverseMass;
	impulseToVel_C.matrix[8] += inverseMass;

	Matrix3 impulseMat = impulseToVel_C;
	impulseMat.Inverse();

	Vector3 impulseContact = Vector3(desiredDeltaVelocity, -closingVelocity.y, -closingVelocity.z);
	Mathe::Transform(impulseContact, impulseMat);

	double planarImpulse = sqrt((impulseContact.y * impulseContact.y) + (impulseContact.z * impulseContact.z));
	//Dynamic friction
	if (planarImpulse > impulseContact.x * friction)
	{
		impulseContact.y /= planarImpulse;
		impulseContact.z /= planarImpulse;

		impulseToVel_C.Transpose();

		impulseContact.x = impulseToVel_C.matrix[0]
			+ (impulseToVel_C.matrix[1] * friction * impulseContact.y)
			+ (impulseToVel_C.matrix[2] * friction * impulseContact.z);
		impulseContact.x = desiredDeltaVelocity / impulseContact.x;
		impulseContact.y *= friction * impulseContact.x;
		impulseContact.z *= friction * impulseContact.x;
	}

	return impulseContact;
}

void Contact::MatchRigidbodyAwakeStates()
{
	bool b1Awake = body1->rigidbody.isAwake;
	bool b2Awake = body2->rigidbody.isAwake;

	if (b1Awake != b2Awake)
	{
		if (b1Awake) body2->rigidbody.SetAwake(true);
		else body1->rigidbody.SetAwake(true);
	}
}
