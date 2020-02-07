#include "CollisionResolution2.h"


void CollisionResolution2::PenetrationResolution(std::vector<Contact>& contacts)
{
	bool finished = false;
	numContacts = contacts.size();

	while (!finished)
	{
		//Find the contact with the largest penetration therefore
		//the most important one to resolve
		unsigned contactIndex = GetContactWithLargestPenetration(contacts);
		if (contactIndex == numContacts) break; //all contacts resolved

		//Match RB awake states
		if (contacts[contactIndex].body2->type != Primitive::Type::PLANE)
		{
			contacts[contactIndex].MatchRigidbodyAwakeStates();
		}

		contacts[contactIndex].ResolveContactPenetration();

		Vector3 deltaPosition;
		//Check other contacts for effects from previous resolution
		for (unsigned i = 0; i < numContacts; i++)
		{
			if (contacts[i].body1->type != Primitive::Type::PLANE)
			{
				if (contacts[i].body1 == contacts[contactIndex].body1)
				{
					deltaPosition = contacts[contactIndex].linearChange[0]
						+ contacts[contactIndex].angularChange[0].VectorProduct(contacts[i].relContactPos1);
					contacts[i].penetrationDepth += (float)deltaPosition.ScalarProduct(contacts[i].normal);
				}
				if (contacts[i].body1 == contacts[contactIndex].body2)
				{
					deltaPosition = contacts[contactIndex].linearChange[1]
						+ contacts[contactIndex].angularChange[1].VectorProduct(contacts[i].relContactPos1);
					contacts[i].penetrationDepth += (float)deltaPosition.ScalarProduct(contacts[i].normal);
				}
			}
			if (contacts[i].body2->type != Primitive::Type::PLANE)
			{
				if (contacts[i].body2 == contacts[contactIndex].body1)
				{
					deltaPosition = contacts[contactIndex].linearChange[0]
						+ contacts[contactIndex].angularChange[0].VectorProduct(contacts[i].relContactPos2);
					contacts[i].penetrationDepth -= (float)deltaPosition.ScalarProduct(contacts[i].normal);
				}
				if (contacts[i].body2 == contacts[contactIndex].body2)
				{
					deltaPosition = contacts[contactIndex].linearChange[1]
						+ contacts[contactIndex].angularChange[1].VectorProduct(contacts[i].relContactPos2);
					contacts[i].penetrationDepth += (float)deltaPosition.ScalarProduct(contacts[i].normal);
				}
			}
		}
		finished = true;
	}
}

void CollisionResolution2::VelocityResolution(std::vector<Contact>& contacts)
{
	bool finished = false;
	numContacts = contacts.size();

	while (!finished)
	{
		unsigned contactIndex = GetContactWithLargestVelocityMagnitude(contacts);
		if (contactIndex == numContacts) break; //all contacts resolved

		//Match RB awake states
		if (contacts[contactIndex].body2->type != Primitive::Type::PLANE)
		{
			contacts[contactIndex].MatchRigidbodyAwakeStates();
		}

		contacts[contactIndex].ResolveContactVelocity();

		Vector3 deltaVelocity;
		for (unsigned i = 0; i < numContacts; i++)
		{
			if (contacts[i].body1->type != Primitive::Type::PLANE)
			{
				if (contacts[i].body1 == contacts[contactIndex].body1)
				{
					deltaVelocity = contacts[contactIndex].velocityChange[0]
						+ contacts[contactIndex].rotationChange[0].VectorProduct(contacts[i].relContactPos1);
					Vector3 additionalVel = deltaVelocity;
					Mathe::Transform(additionalVel, contacts[i].worldToContact);
					contacts[i].closingVelocity -= additionalVel;
					contacts[i].CalculateDesiredDeltaVelocity();
				}
				if (contacts[i].body1 == contacts[contactIndex].body2)
				{
					deltaVelocity = contacts[contactIndex].velocityChange[1]
						+ contacts[contactIndex].rotationChange[1].VectorProduct(contacts[i].relContactPos1);
					Vector3 additionalVel = deltaVelocity;
					Mathe::Transform(additionalVel, contacts[i].worldToContact);
					contacts[i].closingVelocity -= additionalVel;
					contacts[i].CalculateDesiredDeltaVelocity();
				}
			}
			if (contacts[i].body2->type != Primitive::Type::PLANE)
			{
				if (contacts[i].body2 == contacts[contactIndex].body1)
				{
					deltaVelocity = contacts[contactIndex].velocityChange[1]
						+ contacts[contactIndex].rotationChange[1].VectorProduct(contacts[i].relContactPos1);
					Vector3 additionalVel = deltaVelocity;
					Mathe::Transform(additionalVel, contacts[i].worldToContact);
					contacts[i].closingVelocity += additionalVel;
					contacts[i].CalculateDesiredDeltaVelocity();
				}
				if (contacts[i].body2 == contacts[contactIndex].body2)
				{
					deltaVelocity = contacts[contactIndex].velocityChange[1]
						+ contacts[contactIndex].rotationChange[1].VectorProduct(contacts[i].relContactPos2);
					Vector3 additionalVel = deltaVelocity;
					Mathe::Transform(additionalVel, contacts[i].worldToContact);
					contacts[i].closingVelocity += additionalVel;
					contacts[i].CalculateDesiredDeltaVelocity();
				}
			}
		}
		finished = true;
	}
}

unsigned CollisionResolution2::GetContactWithLargestPenetration(std::vector<Contact>& contacts)
{
	float largestPenetration = minPositionChange;
	unsigned contactIndex = numContacts;
	for (unsigned i = 0; i < numContacts; i++)
	{
		if (contacts[i].penetrationDepth > largestPenetration)
		{
			largestPenetration = contacts[i].penetrationDepth;
			contactIndex = i;
		}
	}
	return contactIndex;
}

unsigned CollisionResolution2::GetContactWithLargestVelocityMagnitude(std::vector<Contact>& contacts)
{
	float largestVelocityMag = minVelocityChange;
	unsigned contactIndex = numContacts;
	for (unsigned i = 0; i < numContacts; i++)
	{
		if (contacts[i].desiredDeltaVelocty > largestVelocityMag)
		{
			largestVelocityMag = contacts[i].desiredDeltaVelocty;
			contactIndex = i;
		}
	}
	return contactIndex;
}
