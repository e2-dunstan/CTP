#include "CollisionResolution2.h"

void CollisionResolution2::ResolveContacts(std::vector<Contact>& contacts)
{
	InitialiseContacts(contacts);
	CalculatePenetrationResolution(contacts);
	CalculateVelocityResolution(contacts);
}

void CollisionResolution2::InitialiseContacts(std::vector<Contact>& contacts)
{
	for (int i = 0; i < contacts.size(); i++)
	{
		if (contacts[i].body1->type != Primitive::Type::PLANE) contacts[i].body1->colliding = true;
		if (contacts[i].body2->type != Primitive::Type::PLANE) contacts[i].body2->colliding = true;
		contacts[i].CalculateResolutionValues();
	}
}

void CollisionResolution2::CalculatePenetrationResolution(std::vector<Contact>& contacts)
{
	Vector3 deltaPosition;

	unsigned iterations = 0;
	float largestPenetration = 0;

	const unsigned numContacts = contacts.size();

	while (iterations < 1)
	{
		//Find the contact with the largest penetration therefore
		//the most important one to resolve
		largestPenetration = minPositionDelta;
		unsigned cIndex = numContacts;
		for (unsigned i = 0; i < numContacts; i++)
		{
			if (contacts[i].penetrationDepth > largestPenetration)
			{
				largestPenetration = contacts[i].penetrationDepth;
				cIndex = i;
			}
		}
		if (cIndex == numContacts) break;

		//Match RB awake states
		if (contacts[cIndex].body2->type != Primitive::Type::PLANE)
		{
			bool body1Awake = contacts[cIndex].body1->rigidbody.isAwake;
			bool body2Awake = contacts[cIndex].body2->rigidbody.isAwake;

			if (body1Awake ^ body2Awake)
			{
				if (body1Awake) contacts[cIndex].body2->rigidbody.SetAwake(true);
				else contacts[cIndex].body1->rigidbody.SetAwake(true);
			}
		}

		contacts[cIndex].ApplyPositionChange();

		//Check other contacts for effects from previous resolution
		for (unsigned i = 0; i < numContacts; i++)
		{
			if (contacts[i].body1->type != Primitive::Type::PLANE)
			{
				if (contacts[i].body1 == contacts[cIndex].body1)
				{
					deltaPosition = contacts[cIndex].linearChange[0]
						+ contacts[cIndex].angularChange[0].VectorProduct(contacts[i].relContactPos1);
					contacts[i].penetrationDepth += deltaPosition.ScalarProduct(contacts[i].normal);
				}
				if (contacts[i].body1 == contacts[cIndex].body2)
				{
					deltaPosition = contacts[cIndex].linearChange[1]
						+ contacts[cIndex].angularChange[1].VectorProduct(contacts[i].relContactPos1);
					contacts[i].penetrationDepth += deltaPosition.ScalarProduct(contacts[i].normal);
				}
			}
			if (contacts[i].body2->type != Primitive::Type::PLANE)
			{
				if (contacts[i].body2 == contacts[cIndex].body1)
				{
					deltaPosition = contacts[cIndex].linearChange[0]
						+ contacts[cIndex].angularChange[0].VectorProduct(contacts[i].relContactPos2);
					contacts[i].penetrationDepth -= deltaPosition.ScalarProduct(contacts[i].normal);
				}
				if (contacts[i].body2 == contacts[cIndex].body2)
				{
					deltaPosition = contacts[cIndex].linearChange[1]
						+ contacts[cIndex].angularChange[1].VectorProduct(contacts[i].relContactPos2);
					contacts[i].penetrationDepth += deltaPosition.ScalarProduct(contacts[i].normal);
				}
			}
		}
		iterations++;
	}
}

void CollisionResolution2::CalculateVelocityResolution(std::vector<Contact>& contacts)
{
	Vector3 deltaVelocity;

	unsigned iterations = 0;
	const unsigned numContacts = contacts.size();

	float largestVelocityMag = 0;

	while (iterations < 1)
	{
		largestVelocityMag = minVelocityDelta;
		unsigned cIndex = numContacts;
		for (unsigned i = 0; i < numContacts; i++)
		{
			if (contacts[i].desiredDeltaVelocty > largestVelocityMag)
			{
				largestVelocityMag = contacts[i].desiredDeltaVelocty;
				cIndex = i;
			}
		}
		if (cIndex == numContacts) break;

		//Match RB awake states
		if (contacts[cIndex].body2->type != Primitive::Type::PLANE)
		{
			bool body1Awake = contacts[cIndex].body1->rigidbody.isAwake;
			bool body2Awake = contacts[cIndex].body2->rigidbody.isAwake;

			if (body1Awake ^ body2Awake)
			{
				if (body1Awake) contacts[cIndex].body2->rigidbody.SetAwake(true);
				else contacts[cIndex].body1->rigidbody.SetAwake(true);
			}
		}

		contacts[cIndex].ApplyVelocityChange();

		for (unsigned i = 0; i < numContacts; i++)
		{
			if (contacts[i].body1->type != Primitive::Type::PLANE)
			{
				if (contacts[i].body1 == contacts[cIndex].body1)
				{
					deltaVelocity = contacts[cIndex].velocityChange[0]
						+ contacts[cIndex].rotationChange[0].VectorProduct(contacts[i].relContactPos1);
					Vector3 additionalVel = deltaVelocity;
					Mathe::Transform(additionalVel, contacts[i].worldToContact);
					contacts[i].closingVelocity -= additionalVel;
					contacts[i].CalculateDesiredDeltaVelocity();
				}
				if (contacts[i].body1 == contacts[cIndex].body2)
				{
					deltaVelocity = contacts[cIndex].velocityChange[1]
						+ contacts[cIndex].rotationChange[1].VectorProduct(contacts[i].relContactPos1);
					Vector3 additionalVel = deltaVelocity;
					Mathe::Transform(additionalVel, contacts[i].worldToContact);
					contacts[i].closingVelocity -= additionalVel;
					contacts[i].CalculateDesiredDeltaVelocity();
				}
			}
			if (contacts[i].body2->type != Primitive::Type::PLANE)
			{
				if (contacts[i].body2 == contacts[cIndex].body1)
				{
					deltaVelocity = contacts[cIndex].velocityChange[1]
						+ contacts[cIndex].rotationChange[1].VectorProduct(contacts[i].relContactPos1);
					Vector3 additionalVel = deltaVelocity;
					Mathe::Transform(additionalVel, contacts[i].worldToContact);
					contacts[i].closingVelocity += additionalVel;
					contacts[i].CalculateDesiredDeltaVelocity();
				}
				if (contacts[i].body2 == contacts[cIndex].body2)
				{
					deltaVelocity = contacts[cIndex].velocityChange[1]
						+ contacts[cIndex].rotationChange[1].VectorProduct(contacts[i].relContactPos2);
					Vector3 additionalVel = deltaVelocity;
					Mathe::Transform(additionalVel, contacts[i].worldToContact);
					contacts[i].closingVelocity += additionalVel;
					contacts[i].CalculateDesiredDeltaVelocity();
				}
			}
		}
		iterations++;
	}
}

