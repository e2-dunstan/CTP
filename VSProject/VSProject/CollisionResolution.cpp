#include "CollisionResolution.h"


void CollisionResolution::ResolveInterpenetration(const Contact& contact)
{
	//if (contact.body1->type == Primitive::Type::BOX
	//	&& contact.body2->type == Primitive::Type::BOX)
	//{
		//std::cout << "Contact penetration: " << contact.penetrationDepth << std::endl;
	//}

	//Get body positions relative to contact
	relContactPos1 = contact.point - contact.body1->translation;
	relContactPos2 = contact.point - contact.body2->translation;

	// -- Nonlinear Projection -- //

	float angularInertia1 = 0, angularInertia2 = 0, linearInertia1 = 0, linearInertia2 = 0;

	//inertia for body 1
	Vector3 angularInertia_W = relContactPos1.VectorProduct(contact.normal);
	Mathe::Transform(angularInertia_W, contact.body1->rigidbody.inverseInertiaTensorWorld);
	angularInertia_W = angularInertia_W.VectorProduct(relContactPos1);

	angularInertia1 = angularInertia_W.ScalarProduct(contact.normal);
	linearInertia1 = contact.body1->rigidbody.inverseMass;

	float totalInertia = linearInertia1 + angularInertia1;

	//inertia for body 2
	if (contact.body2->type != Primitive::Type::PLANE)
	{
		angularInertia_W = relContactPos2.VectorProduct(contact.normal);
		Mathe::Transform(angularInertia_W, contact.body2->rigidbody.inverseInertiaTensorWorld);
		angularInertia_W = angularInertia_W.VectorProduct(relContactPos2);

		angularInertia2 = angularInertia_W.ScalarProduct(contact.normal);
		linearInertia2 = contact.body2->rigidbody.inverseMass;

		totalInertia += linearInertia2 + angularInertia2;
	}

	//move values
	float inverseInertia = 1 / totalInertia;
	float linearMove1 = contact.penetrationDepth * (linearInertia1 / totalInertia); // * inverseInertia;
	float linearMove2 = -contact.penetrationDepth * (linearInertia2 / totalInertia); // *inverseInertia;
	float angularMove1 = contact.penetrationDepth * (angularInertia1 / totalInertia); // * inverseInertia;
	float angularMove2 = -contact.penetrationDepth * (angularInertia2 / totalInertia); // * inverseInertia;
	ApplyAngularMoveLimit(linearMove1, angularMove1, contact.body1->scale.Magnitude());
	ApplyAngularMoveLimit(linearMove2, angularMove2, contact.body2->scale.Magnitude());

	//Applying angular resolution
	// 1. Calculate the rotation needed to move contact point by one unit
	// 2. Multiply by number of units needed
	// 3. Apply to rotation quaternion
	
	Vector3 angularChange1, angularChange2;

	//Body 1
	Vector3 angularDir1 = relContactPos1.VectorProduct(contact.normal);
	Mathe::Transform(angularDir1, contact.body1->rigidbody.inverseInertiaTensorWorld);
	angularChange1 = angularDir1 * (angularMove1 / angularInertia1);

	//Body2
	if (contact.body2->type != Primitive::Type::PLANE)
	{
		Vector3 angularDir2 = relContactPos2.VectorProduct(contact.normal);
		Mathe::Transform(angularDir2, contact.body2->rigidbody.inverseInertiaTensorWorld);
		angularChange2 = angularDir2 * (angularMove2 / angularInertia2);
	}

	bool update1 = false, update2 = false;
	if (linearMove1 > 0.001f)
	{
		contact.body1->translation += contact.normal * linearMove1;
		update1 = true;
	}
	if (angularChange1.Magnitude() > 0.001f)
	{
		contact.body1->rotation += angularChange1;
		update1 = true;
	}
	if (linearMove2 > 0.001f)
	{
		contact.body2->translation += contact.normal * linearMove2;
		update2 = true;
	}
	if (angularChange2.Magnitude() > 0.001f)
	{
		contact.body2->rotation += angularChange2;
		update2 = true;
	}
	if (update1) contact.body1->UpdateTransform();
	if (update2) contact.body2->UpdateTransform();
}

void CollisionResolution::ResolveCollision(const Contact& contact)
{
	//Debugging
	if (contact.body1->type != Primitive::Type::PLANE) contact.body1->colliding = true;
	if (contact.body2->type != Primitive::Type::PLANE) contact.body2->colliding = true;

	//Simple resolution of spheres hitting planes
	//if (contact.body1->type == Primitive::Type::SPHERE && contact.body2->type == Primitive::Type::PLANE)
	//{
	//	contact.body1->rigidbody.AddImpulse(contact.normal, contact.restitution);
	//}
	//if (contact.body2->type == Primitive::Type::SPHERE && contact.body1->type == Primitive::Type::PLANE)
	//{
	//	contact.body2->rigidbody.AddImpulse(contact.normal, contact.restitution);
	//}


	// -- 1. Get transform matrices for working in coordinates relative to the contact/world
	contactToWorld = CalculateContactBasis(contact);
	worldToContact = contactToWorld.Transpose();


	// -- 2. Calculate change in velocity of the contact point on each object per unit impulse
	float changeInVelocity = CalculateChangeInVelocity(contact);
	Vector3 contactClosingVelocity = CalculateClosingVelocity(*contact.body1);
	if (contact.body2->type != Primitive::Type::PLANE)
	{
		contactClosingVelocity -= CalculateClosingVelocity(*contact.body2);
	}
	//due to contact normal, y and z values are 0 and can be ignored
	changeInVelocity = -contactClosingVelocity.x * (1.0f + contact.restitution);


	// -- 3. Calculate the desired change in velocity
	float desiredChangeInVelocity = CalculateDesiredChangeInVelocity(contact, contactClosingVelocity);


	// -- 4. Resolve interpenetration
	ResolveInterpenetration(contact);


	// -- 5. Find impulse needed to generate the velocity change needed in the next step
	Vector3 impulse = Vector3(desiredChangeInVelocity / changeInVelocity, 0, 0);
	//Vector3 impulse;
	//if (contact.friction == 0.0f) impulse = CalculateFrictionlessImpulse(contact);
	//else impulse = CalculateFrictionImpulse(contact);
	
	Mathe::Transform(impulse, contactToWorld);


	// -- 6. Split impulse into linear and angular components
	CalculateVelocityChangesFromImpulse(contact, impulse);
}

Matrix CollisionResolution::CalculateContactBasis(const Contact& contact)
{
	Vector3 contactTangents[2];

	//Check if Z axis neraer to the X or Y axis
	if (abs(contact.normal.x) > abs(contact.normal.y))
	{
		//scaling factor to ensure normalisation
		const float scalingFactor = 1.0f / (float)sqrt((contact.normal.z * contact.normal.z) + (contact.normal.x * contact.normal.x));

		//new X axis is at right angles to the world Y axis
		contactTangents[0].x = contact.normal.z * scalingFactor;
		contactTangents[0].y = 0;
		contactTangents[0].z = -contact.normal.x * scalingFactor;

		//new Y axis at right angles to X and Z axes
		contactTangents[1].x = contact.normal.y * contactTangents[0].x;
		contactTangents[1].y =(contact.normal.z * contactTangents[0].x)
			- (contact.normal.x * contactTangents[0].z);
		contactTangents[1].z = -contact.normal.y * contactTangents[0].x;
	}
	else
	{
		const float scalingFactor = 1.0f / (float)sqrt((contact.normal.z * contact.normal.z) + (contact.normal.y * contact.normal.y));

		//new X axis is at right angles to the world X axis
		contactTangents[0].x = 0;
		contactTangents[0].y = -contact.normal.z * scalingFactor;
		contactTangents[0].z = contact.normal.y * scalingFactor;

		contactTangents[1].x = (contact.normal.y * contactTangents[0].z)
			- (contact.normal.z * contactTangents[0].y);
		contactTangents[1].y = -contact.normal.x * contact.normal.z;
		contactTangents[1].z = contact.normal.x * contactTangents[0].y;
	}
	double matVals[16] = { contact.normal.x, contactTangents[0].x, contactTangents[1].x, 0,
					contact.normal.y, contactTangents[0].y, contactTangents[1].y, 0,
					contact.normal.z, contactTangents[0].z, contactTangents[1].z, 0 };

	return Matrix(matVals);
}

float CollisionResolution::CalculateChangeInVelocity(const Contact& contact)
{
	Vector3 deltaVelocity_W = relContactPos1.VectorProduct(contact.normal);
	Mathe::Transform(deltaVelocity_W, contact.body1->rigidbody.inverseInertiaTensorWorld);
	deltaVelocity_W = deltaVelocity_W.VectorProduct(relContactPos1);

	float deltaSpeed = deltaVelocity_W.ScalarProduct(contact.normal);
	deltaSpeed += contact.body1->rigidbody.inverseMass;

	if (contact.body2->type != Primitive::Type::PLANE)
	{
		deltaVelocity_W = relContactPos2.VectorProduct(contact.normal);
		Mathe::Transform(deltaVelocity_W, contact.body2->rigidbody.inverseInertiaTensorWorld);
		deltaVelocity_W = deltaVelocity_W.VectorProduct(relContactPos2);

		deltaSpeed += deltaVelocity_W.ScalarProduct(contact.normal);
		deltaSpeed += contact.body1->rigidbody.inverseMass;
	}

	return deltaSpeed;
}

Vector3 CollisionResolution::CalculateClosingVelocity(const Primitive& prim)
{
	Vector3 vel = prim.rotation.VectorProduct(relContactPos1);
	vel += prim.rigidbody.velocity;

	//if (contact.body2->type != Primitive::Type::PLANE)
	//{
	//	vel += contact.body2->rotation.VectorProduct(relContactPos2);
	//	vel += contact.body2->rigidbody.velocity;
	//}
	
	Mathe::Transform(vel, worldToContact);

	//could be problem here with not getitng accVelocity and adding it to vel

	return vel;
}

float CollisionResolution::CalculateDesiredChangeInVelocity(const Contact& contact, const Vector3& contactVel)
{
	float velFromBodies = 0;

	velFromBodies += contact.body1->rigidbody.velocity.ScalarProduct(contact.normal);
	if (contact.body2->type != Primitive::Type::PLANE)
	{
		velFromBodies -= contact.body2->rigidbody.velocity.ScalarProduct(contact.normal);
	}
	float thisRestitution = contact.restitution;
	if (abs(contactVel.x) < 0.25) thisRestitution = 0.0f;

	return -contactVel.x - thisRestitution * (contactVel.x - velFromBodies);
}

Vector3 CollisionResolution::CalculateFrictionlessImpulse(const Contact& contact)
{
	Vector3 impulse;

	return impulse;
}

void CollisionResolution::CalculateVelocityChangesFromImpulse(const Contact& contact, Vector3 impulse)
{
	Vector3 deltaVelocity1 = impulse * contact.body1->rigidbody.inverseMass;
	Vector3 deltaRotation1 = impulse.VectorProduct(relContactPos1); //impulsive torque
	Mathe::Transform(deltaRotation1, contact.body1->rigidbody.inverseInertiaTensorWorld);

	contact.body1->rigidbody.AddVelocityChange(deltaVelocity1);
	contact.body1->rigidbody.AddRotationChange(deltaRotation1);

	if (contact.body2->type != Primitive::Type::PLANE)
	{
		//impulse *= -1.0f;
		Vector3 deltaVelocity2 = impulse * -contact.body2->rigidbody.inverseMass;
		Vector3 deltaRotation2 = impulse * relContactPos2; //impulsive torque
		Mathe::Transform(deltaRotation2, contact.body2->rigidbody.inverseInertiaTensorWorld);

		contact.body2->rigidbody.AddVelocityChange(deltaVelocity2);
		contact.body2->rigidbody.AddRotationChange(deltaRotation2);
	}
}

void CollisionResolution::ApplyAngularMoveLimit(float& linear, float& angular, const float objMag)
{
	float angularLimit = 0.2f * objMag;
	if (abs(angular) > angularLimit)
	{
		float total = linear + angular;

		if (angular >= 0) angular = angularLimit;
		else angular = -angularLimit;

		linear = total - angular;
	}
	std::cout << linear << std::endl;
}
