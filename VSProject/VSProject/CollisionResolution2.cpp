#include "CollisionResolution2.h"


void CollisionResolution2::PenetrationResolution(std::vector<Contact>& contacts)
{
	numContacts = contacts.size();

	for (unsigned i = 0; i < numContacts; i++)
	{
		unsigned contactIndex = i % numContacts; //to allow for multiple iterations
		//Match RB awake states
		if (contacts[contactIndex].body2->type != PrimitiveType::PLANE)
		{
			contacts[contactIndex].MatchRigidbodyAwakeStates();
		}

		contacts[contactIndex].ResolveContactPenetration();

		Vector3 deltaPosition;
		//Check other contacts for effects from previous resolution
		for (unsigned i = 0; i < numContacts; i++)
		{
			if (i == contactIndex) continue;
			if (contacts[i].body1->type != PrimitiveType::PLANE)
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
			if (contacts[i].body2->type != PrimitiveType::PLANE)
			{
				if (contacts[i].body2 == contacts[contactIndex].body1)
				{
					deltaPosition = contacts[contactIndex].linearChange[0]
						+ contacts[contactIndex].angularChange[0].VectorProduct(contacts[i].relContactPos2);
					contacts[i].penetrationDepth += (float)deltaPosition.ScalarProduct(contacts[i].normal);
				}
				if (contacts[i].body2 == contacts[contactIndex].body2)
				{
					deltaPosition = contacts[contactIndex].linearChange[1]
						+ contacts[contactIndex].angularChange[1].VectorProduct(contacts[i].relContactPos2);
					contacts[i].penetrationDepth += (float)deltaPosition.ScalarProduct(contacts[i].normal);
				}
			}
		}
	}
}

void CollisionResolution2::VelocityResolution(std::vector<Contact>& contacts)
{
	numContacts = contacts.size();

	for (unsigned i = 0; i < numContacts; i++)
	{
		unsigned contactIndex = i % numContacts;
		//Match RB awake states
		if (contacts[contactIndex].body2->type != PrimitiveType::PLANE)
		{
			contacts[contactIndex].MatchRigidbodyAwakeStates();
		}

		contacts[contactIndex].ResolveContactVelocity();

		//continue;
		Vector3 deltaVelocity;
		for (unsigned i = 0; i < numContacts; i++)
		{
			if (i == contactIndex) continue;
			if (contacts[i].body1->type != PrimitiveType::PLANE)
			{
				if (contacts[i].body1 == contacts[contactIndex].body1)
				{
					AdjustDeltaVelocity(&contacts[contactIndex], &contacts[i], 0, contacts[i].relContactPos1);
					//deltaVelocity = contacts[contactIndex].velocityChange[0]
					//	+ contacts[contactIndex].rotationChange[0].VectorProduct(contacts[i].relContactPos1);
					////Vector3 additionalVel = deltaVelocity;
					//Mathe::Transform(deltaVelocity, contacts[i].worldToContact);
					//contacts[i].closingVelocity += deltaVelocity;
					//contacts[i].CalculateDesiredDeltaVelocity();
				}
				if (contacts[i].body1 == contacts[contactIndex].body2)
				{
					AdjustDeltaVelocity(&contacts[contactIndex], &contacts[i], 1, contacts[i].relContactPos1);
					//deltaVelocity = contacts[contactIndex].velocityChange[1]
					//	+ contacts[contactIndex].rotationChange[1].VectorProduct(contacts[i].relContactPos1);
					////Vector3 additionalVel = deltaVelocity;
					//Mathe::Transform(deltaVelocity, contacts[i].worldToContact);
					//contacts[i].closingVelocity += deltaVelocity;
					//contacts[i].CalculateDesiredDeltaVelocity();
				}
			}
			if (contacts[i].body2->type != PrimitiveType::PLANE)
			{
				if (contacts[i].body2 == contacts[contactIndex].body1)
				{
					AdjustDeltaVelocity(&contacts[contactIndex], &contacts[i], 0, contacts[i].relContactPos2);
					//deltaVelocity = contacts[contactIndex].velocityChange[0]
					//	+ contacts[contactIndex].rotationChange[0].VectorProduct(contacts[i].relContactPos2);
					////Vector3 additionalVel = deltaVelocity;
					//Mathe::Transform(deltaVelocity, contacts[i].worldToContact);
					//contacts[i].closingVelocity += deltaVelocity;
					//contacts[i].CalculateDesiredDeltaVelocity();
				}
				if (contacts[i].body2 == contacts[contactIndex].body2)
				{
					AdjustDeltaVelocity(&contacts[contactIndex], &contacts[i], 1, contacts[i].relContactPos2);
					//deltaVelocity = contacts[contactIndex].velocityChange[1]
					//	+ contacts[contactIndex].rotationChange[1].VectorProduct(contacts[i].relContactPos2);
					////Vector3 additionalVel = deltaVelocity;
					//Mathe::Transform(deltaVelocity, contacts[i].worldToContact);
					//contacts[i].closingVelocity += deltaVelocity;
					//contacts[i].CalculateDesiredDeltaVelocity();
				}
			}
		}		
	}
}

void CollisionResolution2::AdjustDeltaVelocity(Contact* thisContact, Contact* otherContact, const unsigned int bt, const Vector3& rcp)
{
	Vector3 deltaVelocity = thisContact->velocityChange[bt]
		+ thisContact->rotationChange[bt].VectorProduct(rcp);
	//Vector3 additionalVel = deltaVelocity;
	Mathe::Transform(deltaVelocity, otherContact->worldToContact);
	otherContact->closingVelocity += deltaVelocity;
	otherContact->CalculateDesiredDeltaVelocity();
}

//Now handled by collision data
//unsigned CollisionResolution2::GetContactWithLargestPenetration(std::vector<Contact>& contacts)
//{
//	float largestPenetration = minPositionChange;
//	unsigned contactIndex = numContacts;
//	for (unsigned i = 0; i < numContacts; i++)
//	{
//		if (contacts[i].penetrationDepth > largestPenetration)
//		{
//			largestPenetration = contacts[i].penetrationDepth;
//			contactIndex = i;
//		}
//	}
//	return contactIndex;
//}
//
//unsigned CollisionResolution2::GetContactWithLargestVelocityMagnitude(std::vector<Contact>& contacts)
//{
//	float largestVelocityMag = minVelocityChange;
//	unsigned contactIndex = numContacts;
//	for (unsigned i = 0; i < numContacts; i++)
//	{
//		if (contacts[i].desiredDeltaVelocity > largestVelocityMag)
//		{
//			largestVelocityMag = contacts[i].desiredDeltaVelocity;
//			contactIndex = i;
//		}
//	}
//	return contactIndex;
//}
