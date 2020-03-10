#include "CollisionData.h"
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
			if (body2->type != Primitive::Type::PLANE) restitution *= 2.0f;
			break;
		}
	}

	CalculateContactBasisMatrices();

	relContactPos1 = (point - body1->collisionVolume.centre);
	relContactPos2 = (point - body2->collisionVolume.centre);

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
	double matVals[16] = { normal.x, contactTangents[0].x, contactTangents[1].x, 0,
					normal.y, contactTangents[0].y, contactTangents[1].y, 0,
					normal.z, contactTangents[0].z, contactTangents[1].z, 0,
					0, 0, 0, 1};

	contactToWorld = Matrix(matVals);
	worldToContact = contactToWorld.Transpose();
}

void Contact::CalculateClosingVelocities()
{
	//Body 1
	closingVelocity = body1->rigidbody.angularVelocity.VectorProduct(relContactPos1) + body1->rigidbody.velocity;
	Mathe::Transform(closingVelocity, worldToContact);
	Vector3 prevVelocity1 = body1->rigidbody.GetPreviousVelocity() * Global::deltaTime;
	Mathe::Transform(prevVelocity1, worldToContact);

	closingVelocity += Vector3(0, prevVelocity1.y, prevVelocity1.z); //not interested in normal dir

	//Body2
	if (body2->type != Primitive::Type::PLANE)
	{
		Vector3 closingVelocity2 = body2->rigidbody.angularVelocity.VectorProduct(relContactPos2) + body2->rigidbody.velocity;
		Mathe::Transform(closingVelocity2, worldToContact);
		Vector3 prevVelocity2 = body2->rigidbody.GetPreviousVelocity() * Global::deltaTime;
		Mathe::Transform(prevVelocity2, worldToContact);
		prevVelocity2.x = 0; 

		closingVelocity -= (closingVelocity2 + Vector3(0, prevVelocity2.y, prevVelocity2.z));
	}
}

void Contact::CalculateDesiredDeltaVelocity()
{
	double bodiesVelocity = 0;
	if (body1->rigidbody.isAwake)
	{
		bodiesVelocity += abs(body1->rigidbody.GetPreviousVelocity().ScalarProduct(normal));// *Global::deltaTime;
	}
	if (body2->type != Primitive::Type::PLANE && body2->rigidbody.isAwake)
	{
		bodiesVelocity -= abs(body2->rigidbody.GetPreviousVelocity().ScalarProduct(normal));// *Global::deltaTime;
	}
	float r = restitution;
	if (abs(closingVelocity.x) < 0.25) 
		r = 0.0f;

	desiredDeltaVelocity = -(float)closingVelocity.x - r * (float)(closingVelocity.x - bodiesVelocity);
}

void Contact::ResolveContactPenetration()
{
	// -- Nonlinear Projection -- //

	Vector3 angularInertia_W = relContactPos1.VectorProduct(normal);
	Mathe::Transform(angularInertia_W, body1->rigidbody.inverseInertiaTensorWorld);
	angularInertia_W = angularInertia_W.VectorProduct(relContactPos1);
	angularInertia_W = Vector3(Mathe::ToRadians(angularInertia_W.x), Mathe::ToRadians(angularInertia_W.y), Mathe::ToRadians(angularInertia_W.z));

	float angularInertia1 = (float)angularInertia_W.ScalarProduct(normal);
	float linearInertia1 = body1->rigidbody.inverseMass;

	float totalInertia = linearInertia1 + angularInertia1;

	float angularInertia2 = 0;
	float linearInertia2 = 0;

	//inertia for body 2
	if (body2->type != Primitive::Type::PLANE)
	{
		angularInertia_W = relContactPos2.VectorProduct(normal);
		Mathe::Transform(angularInertia_W, body2->rigidbody.inverseInertiaTensorWorld);
		angularInertia_W = angularInertia_W.VectorProduct(relContactPos2);
		angularInertia_W = Vector3(Mathe::ToRadians(angularInertia_W.x), Mathe::ToRadians(angularInertia_W.y), Mathe::ToRadians(angularInertia_W.z));

		angularInertia2 = (float)angularInertia_W.ScalarProduct(normal);
		linearInertia2 = body2->rigidbody.inverseMass;

		totalInertia += linearInertia2 + angularInertia2;
	}

	//move values
	//float inverseInertia = 1.0f / totalInertia;
	float linearMove1 = penetrationDepth * (linearInertia1 / totalInertia);
	float linearMove2 = -penetrationDepth * (linearInertia2 / totalInertia);
	float angularMove1 = penetrationDepth * (angularInertia1 / totalInertia);
	float angularMove2 = -penetrationDepth * (angularInertia2 / totalInertia);

	float m = (relContactPos1 - (normal * relContactPos1.ScalarProduct(normal))).Magnitude();
	ApplyAngularMoveLimit(linearMove1, angularMove1, m/*(float)body1->scale.SquaredMagnitude()*/);
	m = (relContactPos2 - (normal * relContactPos2.ScalarProduct(normal))).Magnitude();
	ApplyAngularMoveLimit(linearMove2, angularMove2, m/*(float)body2->scale.SquaredMagnitude()*/);

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

	if (linearMove1 != 0)
	{
		linearChange[0] = normal * linearMove1;
		body1->translation += linearChange[0];
	}
	if (angularChange[0].SquaredMagnitude() > 0.01f)
	{
		Quaternion q;
		body1->GetOrientation(&q);
		angularChange[0] = Vector3(Mathe::ToRadians(angularChange[0].x), Mathe::ToRadians(angularChange[0].y), Mathe::ToRadians(angularChange[0].z));
		Mathe::AddScaledVector(q, angularChange[0], 1.0);// Global::deltaTime);
		body1->SetOrientation(q);
	}
	if (linearMove2 != 0)
	{
		linearChange[1] = normal * linearMove2;
		body2->translation += linearChange[1];
	}
	if (angularChange[1].SquaredMagnitude() > 0.01f)
	{
		Quaternion q;
		body2->GetOrientation(&q);
		angularChange[1] = Vector3(Mathe::ToRadians(angularChange[1].x), Mathe::ToRadians(angularChange[1].y), Mathe::ToRadians(angularChange[1].z));
		Mathe::AddScaledVector(q, angularChange[1], 1.0);// Global::deltaTime);
		body2->SetOrientation(q);
	}

	if (body1->rigidbody.isAwake)
	{
		body1->updateTransform = true;
	}
	if (body2->type != Primitive::Type::PLANE && body2->rigidbody.isAwake)
	{
		body2->updateTransform = true;
	}
	//if (!body1->rigidbody.isAwake)
	//	body1->UpdateTransform();
	//if (!body2->rigidbody.isAwake)
	//	body2->UpdateTransform();
	//std::cout << linearMove1 << ", " << linearMove2 << std::endl;
	if (linearMove1 > 0.8f || linearMove2 < -0.8f)
		std::cout << "WARNING: large linear change detected: linear move 1 - " << linearMove1 << ", linear move 2 - " << linearMove2 << std::endl;
}

void Contact::ApplyAngularMoveLimit(float& linear, float& angular, const float projection/*const float objMag*/)
{
	float angularLimit = 1.0f * projection;
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

	velocityChange[0] = impulse * body1->rigidbody.inverseMass;// *normal.SumComponents();
	rotationChange[0] = relContactPos1.VectorProduct(impulse); //impulsive torque
	rotationChange[0] = Vector3(Mathe::ToRadians(rotationChange[0].x), Mathe::ToRadians(rotationChange[0].y), Mathe::ToRadians(rotationChange[0].z));
	Mathe::Transform(rotationChange[0], body1->rigidbody.inverseInertiaTensorWorld);
	
	body1->rigidbody.AddVelocityChange(velocityChange[0]);
	body1->rigidbody.AddRotationChange(rotationChange[0]);

	if (body2->type != Primitive::Type::PLANE)
	{
		velocityChange[1] = impulse * body2->rigidbody.inverseMass * -1.0;// *-normal.SumComponents();
		rotationChange[1] = impulse.VectorProduct(relContactPos2); //impulsive torque
		rotationChange[1] = Vector3(Mathe::ToRadians(rotationChange[1].x), Mathe::ToRadians(rotationChange[1].y), Mathe::ToRadians(rotationChange[1].z));
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

		deltaSpeed += (float)deltaVelocity_W.ScalarProduct(normal) + body2->rigidbody.inverseMass;
	}

	return Vector3((double)desiredDeltaVelocity / (double)deltaSpeed, 0.0, 0.0);
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

	if (b1Awake != b2Awake)
	{
		if (b1Awake) body2->rigidbody.SetAwake(true);
		else body1->rigidbody.SetAwake(true);
	}
}

void CollisionData::SortContactsByPenetration()
{
	std::sort(contacts.begin(), contacts.end(),
		[](Contact const& a, Contact const& b) -> bool
		{ return a.penetrationDepth > b.penetrationDepth; });
}

void CollisionData::SortContactsByVelocityMag()
{
	std::sort(contacts.begin(), contacts.end(), 
		[](Contact const& a, Contact const& b) -> bool
		{ return a.desiredDeltaVelocity > b.desiredDeltaVelocity; });
}
