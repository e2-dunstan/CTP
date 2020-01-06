#include "CollisionResolution.h"

void CollisionResolution::Resolve(Contact& c)
{
	contact = c;

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

	// 1. Get transform matrix for working in coordinates relative to the contact
	contactToWorld = CalculateContactBasis();
	worldToContact = contactToWorld.Transpose();

	// 2. Calculate change in velocity of the contact point on each object per unit impulse
	float changeInVelocity = CalculateChangeInVelocity();

	// 3. Find impulse needed to generate the velocity change needed in the next step

	// 4. 

	// 5. 
}

Matrix CollisionResolution::CalculateContactBasis()
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

	return Matrix(matVals); //this should be a 3x3...
}

float CollisionResolution::CalculateChangeInVelocity()
{
	Vector3 relContactBodyPosition = contact.body1->translation;
	Mathe::Transform(relContactBodyPosition, worldToContact);

	Vector3 deltaVelocity_W = relContactBodyPosition.VectorProduct(contact.normal);
	Mathe::Transform(deltaVelocity_W, contact.body1->rigidbody.inverseInertiaTensor);
	deltaVelocity_W = deltaVelocity_W.VectorProduct(relContactBodyPosition);

	float deltaSpeed = deltaVelocity_W.ScalarProduct(contact.normal);
	deltaSpeed += contact.body1->rigidbody.inverseMass;

	if (contact.body2->type != Primitive::Type::PLANE)
	{
		//GET INVERSE INERTIA TENSOR WORLD

		relContactBodyPosition = contact.body2->translation;
		Mathe::Transform(relContactBodyPosition, worldToContact);

		deltaVelocity_W = relContactBodyPosition.VectorProduct(contact.normal);
		Mathe::Transform(deltaVelocity_W, contact.body2->rigidbody.inverseInertiaTensor);
		deltaVelocity_W = deltaVelocity_W.VectorProduct(relContactBodyPosition);

		deltaSpeed += deltaVelocity_W.ScalarProduct(contact.normal);
		deltaSpeed += contact.body1->rigidbody.inverseMass;
	}

	return deltaSpeed;
}
