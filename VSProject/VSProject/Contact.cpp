#include "Contact.h"
#include <algorithm>

void Contact::PrepareResolution()
{
	//find restitution
	switch (Materials::GetBounceCombineType(body1->rigidbody.material, body2->rigidbody.material))
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

	friction_static = Materials::GetCombinedStaticFriction(body1->rigidbody.material, body2->rigidbody.material);
	friction_dynamic = Materials::GetCombinedDynamicFriction(body1->rigidbody.material, body2->rigidbody.material);

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
	//worldToContact.Inverse();
}

void Contact::CalculateClosingVelocities()
{
	closingVelocity = Vector3();
	//Body 1
	if (body1->rigidbody.isAwake)
	{
		closingVelocity = body1->rigidbody.angularVelocity.VectorProduct(relContactPos1) + body1->rigidbody.velocity;
		Mathe::Transform(closingVelocity, worldToContact);
		//global forces
		Vector3 acc = body1->rigidbody.GetTrueAccelerationLastFrame() * Global::deltaTime;
		Mathe::Transform(acc, worldToContact);
		closingVelocity += Vector3(0, acc.y, acc.z); //not interested in normal dir
	}
	//Body2
	if (!body2->isStatic && body2->rigidbody.isAwake)
	{
		Vector3 closingVelocity2 = body2->rigidbody.angularVelocity.VectorProduct(relContactPos2) + body2->rigidbody.velocity;
		Mathe::Transform(closingVelocity2, worldToContact);
		//global forces
		Vector3 acc = body2->rigidbody.GetTrueAccelerationLastFrame() * Global::deltaTime;
		Mathe::Transform(acc, worldToContact);
		closingVelocity2 += Vector3(0, acc.y, acc.z);

		closingVelocity -= closingVelocity2;
	}
}

void Contact::CalculateDesiredDeltaVelocity()
{
	//reduces ground vibration but can cause other issues
	double bodiesVelocity = 0;

	if (body1->rigidbody.isAwake)
	{
		bodiesVelocity += body1->rigidbody.GetTrueAccelerationLastFrame().ScalarProduct(normal) * Global::deltaTime;
	}
	if (!body2->isStatic && body2->rigidbody.isAwake)
	{
		bodiesVelocity -= body2->rigidbody.GetTrueAccelerationLastFrame().ScalarProduct(normal) * Global::deltaTime;
	}
	float r = restitution;
	if (abs(closingVelocity.SumComponents()) < 0.25) 
		r = 0.0f;

	desiredDeltaVelocity = -(float)closingVelocity.x - r * (float)(closingVelocity.x - bodiesVelocity);
	/*if (desiredDeltaVelocity > 100)
	{
		body1->rigidbody.GetTrueAccelerationLastFrame().DebugOutput();
		body2->rigidbody.GetTrueAccelerationLastFrame().DebugOutput();
		normal.DebugOutput();
		std::cout << bodiesVelocity << ", " << desiredDeltaVelocity  << ", " << Global::deltaTime << std::endl;
	}*/
}


//Old, unfinished implementation featuring equations from Erin Catto
//2D so there were problems with getting friction resolution
/*void Contact::Resolve()
{
	// --------------
	// Normal impulse
	// --------------

	Vector3 deltaVelocity = body2->rigidbody.velocity + (body2->rigidbody.angularVelocity.VectorProduct(relContactPos2))
		- body1->rigidbody.velocity - body1->rigidbody.angularVelocity.VectorProduct(relContactPos1);

	Vector3 knRelNormal[2] = { relContactPos1.VectorProduct(normal), relContactPos2.VectorProduct(normal) };
	Mathe::Transform(knRelNormal[0], body1->rigidbody.inverseInertiaTensorWorld);
	Mathe::Transform(knRelNormal[1], body1->rigidbody.inverseInertiaTensorWorld);

	float kn = body1->rigidbody.inverseMass + body2->rigidbody.inverseMass + 
		(knRelNormal[0].VectorProduct(relContactPos1) + knRelNormal[1].VectorProduct(relContactPos2)).ScalarProduct(normal);

	float velocityBias = (0.2f / Global::deltaTime) * std::max(0.0f, penetrationDepth - (penetrationDepth / 3));

	float magnitude = std::max(((deltaVelocity.ScalarProduct(normal) * -1.0) + velocityBias) / kn, 0.0);

	Vector3 impulse = normal * magnitude;
	Vector3 relTransforms[2] = { relContactPos1, relContactPos2 };
	Mathe::Transform(relTransforms[0], body1->rigidbody.inverseInertiaTensorWorld);
	Mathe::Transform(relTransforms[1], body2->rigidbody.inverseInertiaTensorWorld);

	velocityChange[0] = body1->rigidbody.velocity - impulse * body1->rigidbody.inverseMass;
	angularChange[0] = body1->rigidbody.angularVelocity - relTransforms[0].VectorProduct(impulse);

	velocityChange[1] = body2->rigidbody.velocity + impulse * body2->rigidbody.inverseMass;
	angularChange[1] = body2->rigidbody.angularVelocity + relTransforms[1].VectorProduct(impulse);
	
	body1->rigidbody.AddVelocityChange(velocityChange[0]);
	body1->rigidbody.AddVelocityChange(angularChange[0]);
	body2->rigidbody.AddVelocityChange(velocityChange[1]);
	body2->rigidbody.AddRotationChange(angularChange[1]);

	// --------------
	// Friction impulse
	// --------------

	//rotate the velocities so their z axis points along the normal
	//x and y components of the matrix are the tangent vectors

	//Dynamic friction occurs, if the relative tangential velocity u(rel,t) = u(rel) − u(rel,n) between 
	//two colliding bodies at the contact point	is not zero.
	//To simulate dynamic friction an impulse is computed that acts in the opposite direction of the tangent 
	// t = u(rel,t) / |u(rel,t)|

	const Vector3 relativeVelocity = body2->rigidbody.velocity - body1->rigidbody.velocity;
	Vector3 tangents[2] = { Vector3(), Vector3() };
	//relative tangential velocity
	tangents[0] = relativeVelocity - normal * relativeVelocity;

	if (tangents[0].SquaredMagnitude() > 0.001)
	{
		tangents[0] *= (1.0 / tangents[0].Magnitude());
		tangents[0] = tangents[0].Normalise();
		tangents[1] = normal.VectorProduct(tangents[0]).Normalise();
	}
}*/


void Contact::ResolvePenetration()
{
	// -------------------------- //
	// -- Nonlinear Projection -- //
	// -------------------------- //

	//linear impulse = lambda * normal
	//angular impulse (torque) = lambda * (relative contact position X normal)

	// -----------
	// BODY 1 inertia
	// -----------
	Vector3 angularInertia_world = relContactPos1.VectorProduct(normal);
	Mathe::Transform(angularInertia_world, body1->rigidbody.inverseInertiaTensorWorld);
	angularInertia_world = angularInertia_world.VectorProduct(relContactPos1);

	double angularInertia1 = angularInertia_world.ScalarProduct(normal);
	double linearInertia1 = body1->rigidbody.inverseMass;

	double totalInertia = linearInertia1 + angularInertia1;

	double angularInertia2 = 0;
	double linearInertia2 = 0;

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

	// -----------
	// Calculate move values
	// -----------
	linearChange[0] = normal * (penetrationDepth * (linearInertia1 / totalInertia));
	linearChange[1] = normal * (-penetrationDepth * (linearInertia2 / totalInertia));
	double angularMove1 = penetrationDepth * (angularInertia1 / totalInertia);
	double angularMove2 = -penetrationDepth * (angularInertia2 / totalInertia);

	// -----------
	// Apply linear changes
	// -----------
	if (abs(linearChange[0].SumComponents()) > 0)
		body1->translation += linearChange[0].Clamp(-abs(penetrationDepth), abs(penetrationDepth));
	if (abs(linearChange[1].SumComponents()) > 0)
		body2->translation += linearChange[1].Clamp(-abs(penetrationDepth), abs(penetrationDepth));

	// -----------
	// Apply angular changes
	// -----------
	if (abs(angularMove1) > 0)
	{
		Vector3 angularDir1 = relContactPos1.VectorProduct(normal);
		Mathe::Transform(angularDir1, body1->rigidbody.inverseInertiaTensorWorld);
		angularChange[0] = angularDir1 * (angularMove1 / angularInertia1);

		Quaternion q;
		body1->GetOrientation(&q);
		Mathe::AddScaledVector(q, angularChange[0], 1.0, true);
		body1->SetOrientation(q);

		//if (angularChange[0].SumComponents() > 5)
		//	angularChange[0].DebugOutput();
	}
	if (abs(angularMove2) > 0 && !body2->isStatic)
	{
		Vector3 angularDir2 = relContactPos2.VectorProduct(normal);
		Mathe::Transform(angularDir2, body2->rigidbody.inverseInertiaTensorWorld);
		angularChange[1] = angularDir2 * (angularMove2 / angularInertia2);

		Quaternion q;
		body2->GetOrientation(&q);
		Mathe::AddScaledVector(q, angularChange[1], 1.0, true);
		body2->SetOrientation(q);

		//if (angularChange[1].SumComponents() > 5)
		//	angularChange[1].DebugOutput();
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
	if (friction_static == 0.0f) impulse = FrictionlessImpulse();
	else impulse = FrictionImpulse();

	Mathe::Transform(impulse, contactToWorld);

	//v = impulse * mass
	velocityChange[0] = impulse * body1->rigidbody.inverseMass;
	//w = inertia tensor * (rel pos X impulse)
	rotationChange[0] = relContactPos1.VectorProduct(impulse); //impulsive torque
	Mathe::Transform(rotationChange[0], body1->rigidbody.inverseInertiaTensorWorld);

	body1->rigidbody.AddVelocityChange(velocityChange[0].Clamp(-abs(desiredDeltaVelocity), abs(desiredDeltaVelocity)));
	body1->rigidbody.AddRotationChange(rotationChange[0].Clamp(-abs(desiredDeltaVelocity), abs(desiredDeltaVelocity)));

	if (!body2->isStatic)
	{
		velocityChange[1] = impulse * -body2->rigidbody.inverseMass;
		rotationChange[1] = impulse.VectorProduct(relContactPos2); //impulsive torque
		Mathe::Transform(rotationChange[1], body2->rigidbody.inverseInertiaTensorWorld);

		body2->rigidbody.AddVelocityChange(velocityChange[1].Clamp(-abs(desiredDeltaVelocity), abs(desiredDeltaVelocity)));
		body2->rigidbody.AddRotationChange(rotationChange[1].Clamp(-abs(desiredDeltaVelocity), abs(desiredDeltaVelocity)));
	}

	/*if (velocityChange[0].SquaredMagnitude() > 3000)
	{
		std::cout << "WARNING: large velocity change ";
		velocityChange[0].DebugOutput();
	}
	if (velocityChange[1].SquaredMagnitude() > 3000)
	{
		std::cout << "WARNING: large velocity change ";
		velocityChange[1].DebugOutput();
	}*/
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
	//Frictionless only uses one axis (desired delta vel / delta speed) in
	//the direction of the normal. Friction introduces tangential forces
	//so the impulse must be represented by more than one axis

	//Below code is identical to frictionless impulse but instead works on
	//all required axes

	Vector3 impulseContact = Vector3(desiredDeltaVelocity, -closingVelocity.y, -closingVelocity.z);

	//Matrix equivalent of cross product
	//0 -z  y
	//z  0 -x
	//-y x  0
	double skewMatrixVals[9] = { 0.0 };
	skewMatrixVals[1] = -relContactPos1.z;
	skewMatrixVals[2] = relContactPos1.y;
	skewMatrixVals[3] = relContactPos1.z;
	skewMatrixVals[5] = -relContactPos1.x;
	skewMatrixVals[6] = -relContactPos1.y;
	skewMatrixVals[7] = relContactPos1.x;
	
	Matrix3 basisMatrix = Matrix3(skewMatrixVals);
	// v * x = -x * v
	Matrix3 deltaVelocity_w = (basisMatrix * body1->rigidbody.inverseInertiaTensorWorld) * basisMatrix;
	deltaVelocity_w = deltaVelocity_w * -1.0;

	float inverseMass = body1->rigidbody.inverseMass;
	
	if (!body2->isStatic)
	{
		basisMatrix.matrix[0] = basisMatrix.matrix[4] = basisMatrix.matrix[8] = 0.0;
		basisMatrix.matrix[1] = -relContactPos2.z;
		basisMatrix.matrix[2] = relContactPos2.y;
		basisMatrix.matrix[3] = relContactPos2.z;
		basisMatrix.matrix[5] = -relContactPos2.x;
		basisMatrix.matrix[6] = -relContactPos2.y;
		basisMatrix.matrix[7] = relContactPos2.x;

		Matrix3 impulseToVel_W_2 = (basisMatrix * body2->rigidbody.inverseInertiaTensorWorld) * basisMatrix;
		impulseToVel_W_2 = impulseToVel_W_2 * -1.0;

		deltaVelocity_w = deltaVelocity_w + impulseToVel_W_2;

		inverseMass += body2->rigidbody.inverseMass;
	}

	Matrix3 deltaSpeed_c = (worldToContact * deltaVelocity_w) * contactToWorld;

	//Linear velocity change, along the diagonal
	deltaSpeed_c.matrix[0] += inverseMass;
	deltaSpeed_c.matrix[4] += inverseMass;
	deltaSpeed_c.matrix[8] += inverseMass;

	Matrix3 impulseMat = deltaSpeed_c;
	impulseMat.Inverse();
	Mathe::Transform(impulseContact, impulseMat);

	double impulseInPlane = sqrt((impulseContact.y * impulseContact.y) + (impulseContact.z * impulseContact.z));

	if (abs(impulseInPlane) > abs(impulseContact.x * friction_static))
	{
		impulseContact.y /= impulseInPlane;
		impulseContact.z /= impulseInPlane;

		//delta speed
		impulseContact.x = deltaSpeed_c.matrix[0]
			+ deltaSpeed_c.matrix[1] * friction_dynamic * impulseContact.y
			+ deltaSpeed_c.matrix[2] * friction_dynamic * impulseContact.z;
		//desired delta velocity / delta speed
		impulseContact.x = desiredDeltaVelocity / impulseContact.x;
		impulseContact.y *= (friction_dynamic * impulseContact.x);
		impulseContact.z *= (friction_dynamic * impulseContact.x);

		if (!body2->isStatic)
		{
			const float threshold = 0.1f;
			const float sum = impulseContact.y + impulseContact.z;
			if (abs(sum) < threshold)
			{
				//impulseContact.x += sum;
				impulseContact.y = 0;
				impulseContact.z = 0;
			}
		}
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
