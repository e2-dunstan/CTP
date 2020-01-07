#include "CollisionResolution.h"


void CollisionResolution::ResolveInterpenetration(const Contact& contact)
{
	//Get body positions relative to contact
	relContactPos1 = contact.body1->translation;
	Mathe::Transform(relContactPos1, worldToContact);
	relContactPos2 = contact.body2->translation;
	Mathe::Transform(relContactPos2, worldToContact);

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
	float linearMove1 = contact.penetrationDepth * linearInertia1 * inverseInertia;
	float linearMove2 = -contact.penetrationDepth * linearInertia2 * inverseInertia;
	float angularMove1 = contact.penetrationDepth * angularInertia1 * inverseInertia;
	float angularMove2 = -contact.penetrationDepth * angularInertia2 * inverseInertia;

	//Apply linear
	if (linearMove1 != 0)
	{
		contact.body1->translation += contact.normal * linearMove1 * 10;
		contact.body1->UpdateTransform();
	}
	if (linearMove2 != 0)
	{
		contact.body2->translation += contact.normal * linearMove2;
		contact.body2->UpdateTransform();
	}

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

	// 1. Get transform matrices for working in coordinates relative to the contact/world
	contactToWorld = CalculateContactBasis(contact);
	worldToContact = contactToWorld.Transpose();

	ResolveInterpenetration(contact);

	// 2. Calculate change in velocity of the contact point on each object per unit impulse
	float changeInVelocity = CalculateChangeInVelocity(contact);
	Vector3 contactClosingVelocity = CalculateClosingVelocity(contact);
	changeInVelocity = -contactClosingVelocity.x * (1.0f + contact.restitution);
		//due to contact normal, y and z values are 0 and can be ignored

	// 3. Calculate the desired change in velocity
	float desiredChangeInVelocity = CalculateDesiredChangeInVelocity(contact, contactClosingVelocity);

	// 4. Find impulse needed to generate the velocity change needed in the next step
	Vector3 impulse = Vector3(desiredChangeInVelocity / changeInVelocity, 0, 0);
	Mathe::Transform(impulse, contactToWorld);

	// 5. Split impulse into linear and angular components
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
		contactTangents[1].y = (contact.normal.z * contactTangents[0].x)
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

Vector3 CollisionResolution::CalculateClosingVelocity(const Contact& contact)
{
	Vector3 vel = contact.body1->rotation.VectorProduct(relContactPos1);
	vel += contact.body1->rigidbody.velocity;

	if (contact.body2->type != Primitive::Type::PLANE)
	{
		vel += contact.body2->rotation.VectorProduct(relContactPos2);
		vel += contact.body2->rigidbody.velocity;
	}
	
	Mathe::Transform(vel, worldToContact);

	return vel;
}

float CollisionResolution::CalculateDesiredChangeInVelocity(const Contact& contact, const Vector3& contactVel)
{
	float velFromBodies = 0;

	velFromBodies += contact.body1->rigidbody.velocity.ScalarProduct(contact.normal);
	if (contact.body2->type != Primitive::Type::PLANE)
	{
		velFromBodies += contact.body2->rigidbody.velocity.ScalarProduct(contact.normal);
	}
	float thisRestitution = contact.restitution;
	if (abs(velFromBodies) < 0.00001) thisRestitution = 0.0f;

	return -contactVel.x - thisRestitution * (contactVel.x - velFromBodies);
}

void CollisionResolution::CalculateVelocityChangesFromImpulse(const Contact& contact, Vector3 impulse)
{
	Vector3 deltaVelocity1 = impulse * contact.body1->rigidbody.inverseMass;
	Vector3 deltaRotation1 = impulse * relContactPos1; //impulsive torque
	Mathe::Transform(deltaRotation1, contact.body1->rigidbody.inverseInertiaTensorWorld);

	contact.body1->rigidbody.AddVelocityChange(deltaVelocity1);
	contact.body1->rigidbody.AddRotationChange(deltaRotation1);

	if (contact.body2->type != Primitive::Type::PLANE)
	{
		impulse *= -1.0f;
		Vector3 deltaVelocity2 = impulse * contact.body2->rigidbody.inverseMass;
		Vector3 deltaRotation2 = impulse * relContactPos2; //impulsive torque
		Mathe::Transform(deltaRotation2, contact.body2->rigidbody.inverseInertiaTensorWorld);

		contact.body2->rigidbody.AddVelocityChange(deltaVelocity2);
		contact.body2->rigidbody.AddRotationChange(deltaRotation2);
	}
}
