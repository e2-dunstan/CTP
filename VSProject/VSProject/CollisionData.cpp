#include "CollisionData.h"

void Contact::PrepareResolution()
{
	CalculateContactBasisMatrices();

	relContactPos1 = point - body1->translation;
	relContactPos2 = point - body2->translation;

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

		//new X axis is at right angles to the world Y axis
		contactTangents[0].x = normal.z * scalingFactor;
		contactTangents[0].y = 0;
		contactTangents[0].z = -normal.x * scalingFactor;

		//new Y axis at right angles to X and Z axes
		contactTangents[1].x = normal.y * contactTangents[0].x;
		contactTangents[1].y = (normal.z * contactTangents[0].x)
			- (normal.x * contactTangents[0].z);
		contactTangents[1].z = -normal.y * contactTangents[0].x;
	}
	else
	{
		const float scalingFactor = 1.0f / (float)sqrt((normal.z * normal.z) + (normal.y * normal.y));

		//new X axis is at right angles to the world X axis
		contactTangents[0].x = 0;
		contactTangents[0].y = -normal.z * scalingFactor;
		contactTangents[0].z = normal.y * scalingFactor;

		contactTangents[1].x = (normal.y * contactTangents[0].z)
			- (normal.z * contactTangents[0].y);
		contactTangents[1].y = -normal.x * normal.z;
		contactTangents[1].z = normal.x * contactTangents[0].y;
	}
	double matVals[16] = { normal.x, contactTangents[0].x, contactTangents[1].x, 0,
					normal.y, contactTangents[0].y, contactTangents[1].y, 0,
					normal.z, contactTangents[0].z, contactTangents[1].z, 0 };

	contactToWorld = Matrix(matVals);
	worldToContact = contactToWorld.Transpose();
}

void Contact::CalculateClosingVelocities()
{
	//Body 1
	closingVelocity = body1->rigidbody.angularVelocity.VectorProduct(relContactPos1);
	closingVelocity += body1->rigidbody.velocity;
	Mathe::Transform(closingVelocity, worldToContact);

	Vector3 prevVelocity1 = body1->rigidbody.GetPreviousAcceleration() * Global::deltaTime;
	Mathe::Transform(prevVelocity1, worldToContact);
	prevVelocity1.x = 0; //not interested in normal dir

	closingVelocity += prevVelocity1;

	//Body2
	if (body2->type != Primitive::Type::PLANE)
	{
		Vector3 closingVelocity2 = body2->rigidbody.angularVelocity.VectorProduct(relContactPos2);
		closingVelocity2 += body2->rigidbody.velocity;
		Mathe::Transform(closingVelocity2, worldToContact);

		Vector3 prevVelocity2 = body2->rigidbody.GetPreviousAcceleration() * Global::deltaTime;
		Mathe::Transform(prevVelocity2, worldToContact);
		prevVelocity2.x = 0; //not interested in normal dir

		closingVelocity -= (closingVelocity2 + prevVelocity2);
	}
}

void Contact::CalculateDesiredDeltaVelocity()
{
	float bodiesVelocity = (float)(body1->rigidbody.GetPreviousAcceleration().ScalarProduct(normal) * Global::deltaTime);
	if (body2->type != Primitive::Type::PLANE)
	{
		bodiesVelocity -= (float)(body2->rigidbody.GetPreviousAcceleration().ScalarProduct(normal) * Global::deltaTime);
	}
	float r = restitution;
	if (abs(closingVelocity.x) < 0.25f) r = 0.0f;

	desiredDeltaVelocty = -(float)closingVelocity.x - r * ((float)closingVelocity.x - bodiesVelocity);
}

void Contact::ResolveContactPenetration()
{
	// -- Nonlinear Projection -- //

	float angularInertia1 = 0;
	float angularInertia2 = 0;
	float linearInertia1 = 0;
	float linearInertia2 = 0;

	Vector3 angularInertia_W = relContactPos1.VectorProduct(normal);
	Mathe::Transform(angularInertia_W, body1->rigidbody.inverseInertiaTensorWorld);
	angularInertia_W = angularInertia_W.VectorProduct(relContactPos1);

	angularInertia1 = (float)angularInertia_W.ScalarProduct(normal);
	linearInertia1 = body1->rigidbody.inverseMass;

	float totalInertia = linearInertia1 + angularInertia1;

	//inertia for body 2
	if (body2->type != Primitive::Type::PLANE)
	{
		angularInertia_W = relContactPos2.VectorProduct(normal);
		Mathe::Transform(angularInertia_W, body2->rigidbody.inverseInertiaTensorWorld);
		angularInertia_W = angularInertia_W.VectorProduct(relContactPos2);

		angularInertia2 = (float)angularInertia_W.ScalarProduct(normal);
		linearInertia2 = body2->rigidbody.inverseMass;

		totalInertia += linearInertia2 + angularInertia2;
	}

	//move values
	float inverseInertia = 1 / totalInertia;
	float linearMove1 = penetrationDepth * (linearInertia1 / totalInertia);
	float linearMove2 = -penetrationDepth * (linearInertia2 / totalInertia);
	float angularMove1 = penetrationDepth * (angularInertia1 / totalInertia);
	float angularMove2 = -penetrationDepth * (angularInertia2 / totalInertia);
	ApplyAngularMoveLimit(linearMove1, angularMove1, (float)body1->scale.Magnitude());
	ApplyAngularMoveLimit(linearMove2, angularMove2, (float)body2->scale.Magnitude());

	//Applying angular resolution
	// 1. Calculate the rotation needed to move contact point by one unit
	// 2. Multiply by number of units needed
	// 3. Apply to rotation quaternion

	//Body 1
	if (angularMove1 != 0)
	{
		Vector3 angularDir1 = relContactPos1.VectorProduct(normal);
		Mathe::Transform(angularDir1, body1->rigidbody.inverseInertiaTensorWorld);
		angularChange[0] = angularDir1 * (angularMove1 / angularInertia1);
	}
	else angularChange[0] = Vector3();

	//Body2
	if (angularMove2 != 0 && body2->type != Primitive::Type::PLANE)
	{
		Vector3 angularDir2 = relContactPos2.VectorProduct(normal);
		Mathe::Transform(angularDir2, body2->rigidbody.inverseInertiaTensorWorld);
		angularChange[1] = angularDir2 * (angularMove2 / angularInertia2);
	}
	else angularChange[1] = Vector3();

	if (linearMove1 > 0.001f)
	{
		body1->translation += normal * linearMove1;
	}
	if (angularChange[0].Magnitude() > 0.001f)
	{
		Quaternion q;
		body1->GetOrientation(&q);
		Mathe::AddScaledVector(q, angularChange[0], 1.0);
		body1->SetOrientation(q);
	}
	if (linearMove2 > 0.001f)
	{
		body2->translation += normal * linearMove2;
	}
	if (angularChange[1].Magnitude() > 0.001f)
	{
		Quaternion q;
		body2->GetOrientation(&q);
		Mathe::AddScaledVector(q, angularChange[1], 1.0);
		body2->SetOrientation(q);
	}

	if (body1->rigidbody.isAwake)
	{
		body1->orientation.Normalise();
		body1->UpdateTransform();
	}
	else if (body2->type != Primitive::Type::PLANE && body2->rigidbody.isAwake)
	{
		body2->orientation.Normalise();
		body2->UpdateTransform();
	}
}

void Contact::ApplyAngularMoveLimit(float& linear, float& angular, const float objMag)
{
	float angularLimit = 0.2f * objMag;
	if (abs(angular) > angularLimit)
	{
		float total = linear + angular;

		if (angular >= 0) angular = angularLimit;
		else angular = -angularLimit;

		linear = total - angular;
	}
}

void Contact::ResolveContactVelocity()
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

	if (body2->type != Primitive::Type::PLANE)
	{
		//impulse *= -1.0f;
		velocityChange[1] = impulse * -body2->rigidbody.inverseMass;
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

	float deltaSpeed = (float)deltaVelocity_W.ScalarProduct(normal);
	deltaSpeed += body1->rigidbody.inverseMass;

	if (body2->type != Primitive::Type::PLANE)
	{
		deltaVelocity_W = relContactPos2.VectorProduct(normal);
		Mathe::Transform(deltaVelocity_W, body2->rigidbody.inverseInertiaTensorWorld);
		deltaVelocity_W = deltaVelocity_W.VectorProduct(relContactPos2);

		deltaSpeed += (float)deltaVelocity_W.ScalarProduct(normal);
		deltaSpeed += (float)body2->rigidbody.inverseMass;
	}

	return Vector3((double)(desiredDeltaVelocty / deltaSpeed), 0.0, 0.0);
}

Vector3 Contact::FrictionImpulse()
{
	//to be implemented
	return FrictionlessImpulse();
}

void Contact::MatchRigidbodyAwakeStates()
{
	bool b1Awake = body1->rigidbody.isAwake;
	bool b2Awake = body2->rigidbody.isAwake;

	if (b1Awake ^ b2Awake)
	{
		if (b1Awake) body2->rigidbody.SetAwake(true);
		else body1->rigidbody.SetAwake(true);
	}

	//if ((body1->rigidbody.isAwake && !body2->rigidbody.isAwake)
	//	|| (body1->rigidbody.isAwake && !body2->rigidbody.isAwake))
	//{
	//	body2->rigidbody.SetAwake(true);
	//}
	//else /*if (!body1->rigidbody.isAwake && body2->rigidbody.isAwake)*/
	//	body1->rigidbody.SetAwake(false);
}
