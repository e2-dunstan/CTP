#include "CollisionData.h"

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

void Contact::CalculateResolutionValues()
{
	CalculateContactBasisMatrices();

	relContactPos1 = point - body1->translation;
	relContactPos2 = point - body2->translation;

	CalculateClosingVelocities();
	CalculateDesiredDeltaVelocity();
}

void Contact::CalculateClosingVelocities()
{
	//Body 1
	closingVelocity = body1->rotation.VectorProduct(relContactPos1);
	closingVelocity += body1->rigidbody.velocity;
	Mathe::Transform(closingVelocity, worldToContact);

	Vector3 prevVelocity1 = body1->rigidbody.GetPreviousVelocity();
	Mathe::Transform(prevVelocity1, worldToContact);
	prevVelocity1.x = 0; //not interested in normal dir

	closingVelocity += prevVelocity1;

	//Body2
	if (body2->type != Primitive::Type::PLANE)
	{
		Vector3 closingVelocity2 = body2->rotation.VectorProduct(relContactPos2);
		closingVelocity2 += body2->rigidbody.velocity;
		Mathe::Transform(closingVelocity2, worldToContact);

		Vector3 prevVelocity2 = body2->rigidbody.GetPreviousVelocity();
		Mathe::Transform(prevVelocity2, worldToContact);
		prevVelocity2.x = 0; //not interested in normal dir

		closingVelocity2 += prevVelocity2;

		closingVelocity -= closingVelocity2;
	}
}

void Contact::CalculateDesiredDeltaVelocity()
{
	const float velLimit = 0.25f;

	float bodiesVelocity = 0;
	bodiesVelocity += body1->rigidbody.GetPreviousVelocity().ScalarProduct(normal);
	if (body2->type != Primitive::Type::PLANE)
	{
		bodiesVelocity -= body2->rigidbody.GetPreviousVelocity().ScalarProduct(normal);
	}

	float thisRestitution = restitution;
	if (abs(closingVelocity.x) < velLimit) thisRestitution = 0.0f;

	desiredDeltaVelocty = -closingVelocity.x - thisRestitution * (closingVelocity.x - bodiesVelocity);
}
