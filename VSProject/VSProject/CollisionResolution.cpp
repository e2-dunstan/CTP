#include "CollisionResolution.h"

void CollisionResolution::Resolve(Contact& contact)
{
	//Matrix contactToWorld = CalculateContactBasis(contact.normal);
	//Matrix worldToContact = contactToWorld.Transpose();

	if (contact.body1->type == Primitive::Type::SPHERE)
	{
		contact.body1->rigidbody.AddImpulse(contact.normal, contact.restitution);
	}
	if (contact.body2->type == Primitive::Type::SPHERE)
	{
		contact.body2->rigidbody.AddImpulse(contact.normal, contact.restitution);
	}

	//Debugging
	if (contact.body1->type != Primitive::Type::PLANE) contact.body1->colliding = true;
	if (contact.body2->type != Primitive::Type::PLANE) contact.body2->colliding = true;

}

Matrix CollisionResolution::CalculateContactBasis(const Vector3& contactNormal)
{
	Vector3 contactTangents[2];

	//Check if Z axis neraer to the X or Y axis
	if (abs(contactNormal.x) > abs(contactNormal.y))
	{
		//scaling factor to ensure normalisation
		const float scalingFactor = 1.0f / (float)sqrt((contactNormal.z * contactNormal.z) + (contactNormal.x * contactNormal.x));

		//new X axis is at right angles to the world Y axis
		contactTangents[0].x = contactNormal.z * scalingFactor;
		contactTangents[0].y = 0;
		contactTangents[0].z = -contactNormal.x * scalingFactor;

		contactTangents[1].x = contactNormal.y * contactTangents[0].x;
		contactTangents[1].y = (contactNormal.z * contactTangents[0].x)
			- (contactNormal.x * contactTangents[0].z);
		contactTangents[1].z = -contactNormal.y * contactTangents[0].x;
	}
	else
	{
		const float scalingFactor = 1.0f / (float)sqrt((contactNormal.z * contactNormal.z) + (contactNormal.y * contactNormal.y));

		//new X axis is at right angles to the world X axis
		contactTangents[0].x = 0;
		contactTangents[0].y = -contactNormal.z * scalingFactor;
		contactTangents[0].z = contactNormal.y * scalingFactor;

		contactTangents[1].x = (contactNormal.y * contactTangents[0].z)
			- (contactNormal.z * contactTangents[0].y);
		contactTangents[1].y = -contactNormal.x * contactNormal.z;
		contactTangents[1].z = contactNormal.x * contactTangents[0].y;
	}
	double matVals[16] = { contactNormal.x, contactTangents[0].x, contactTangents[1].x, 0,
					contactNormal.y, contactTangents[0].y, contactTangents[1].y, 0,
					contactNormal.z, contactTangents[0].z, contactTangents[1].z, 0 };

	return Matrix(matVals); //this should be a 3x3...
}
