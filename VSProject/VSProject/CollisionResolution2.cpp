#include "CollisionResolution2.h"

void CollisionResolution2::PenetrationResolution(std::vector<Contact>& contacts)
{
	numContacts = contacts.size();

	if (Global::writeContactDataToFile) CreateCSVFile();

	for (unsigned i = 0; i < numContacts * penetrationIterations; i++)
	{
		unsigned contactIndex = i % numContacts; //to allow for multiple iterations

		if (Global::writeContactDataToFile) WriteToFile(contacts[contactIndex].penetrationDepth, contactIndex, i / numContacts);

		contacts[contactIndex].ResolvePenetration();

		Vector3 deltaPosition;
		//Check other contacts for effects from previous resolution
		for (unsigned i = 0; i < numContacts; i++)
		{
			if (contacts[i].body1->type != PrimitiveType::PLANE)
			{
				if (contacts[i].body1 == contacts[contactIndex].body1)
				{
					deltaPosition = contacts[contactIndex].linearChange[0]
						+ contacts[contactIndex].angularChange[0].VectorProduct(contacts[i].relContactPos1);
					contacts[i].penetrationDepth -= (float)deltaPosition.ScalarProduct(contacts[i].normal);
				}
				if (contacts[i].body1 == contacts[contactIndex].body2)
				{
					deltaPosition = contacts[contactIndex].linearChange[1]
						+ contacts[contactIndex].angularChange[1].VectorProduct(contacts[i].relContactPos1);
					contacts[i].penetrationDepth -= (float)deltaPosition.ScalarProduct(contacts[i].normal);
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

	if (Global::writeContactDataToFile)
	{
		file.close();
		Global::writeContactDataToFile = false;
	}
}

void CollisionResolution2::VelocityResolution(std::vector<Contact>& contacts)
{
	numContacts = contacts.size();

	for (unsigned i = 0; i < numContacts * velocityIterations; i++)
	{
		unsigned contactIndex = i % numContacts;

		contacts[contactIndex].ResolveVelocity();

		//Match RB awake states
		if (contacts[contactIndex].body2->type != PrimitiveType::PLANE
			&& contacts[contactIndex].body1->rigidbody.GetMotion() > contacts[contactIndex].body1->rigidbody.sleepThreshold
			&& contacts[contactIndex].body2->rigidbody.GetMotion() > contacts[contactIndex].body2->rigidbody.sleepThreshold)
		{
			contacts[contactIndex].MatchRigidbodyAwakeStates();
		}

		//continue;

		for (unsigned i = 0; i < numContacts; i++)
		{
			if (contacts[i].body1->type != PrimitiveType::PLANE)
			{
				if (contacts[i].body1 == contacts[contactIndex].body1)
				{
					AdjustDeltaVelocity(&contacts[contactIndex], &contacts[i], 0, contacts[i].relContactPos1, true);
				}
				if (contacts[i].body1 == contacts[contactIndex].body2)
				{
					AdjustDeltaVelocity(&contacts[contactIndex], &contacts[i], 1, contacts[i].relContactPos1, true);
				}
			}
			if (contacts[i].body2->type != PrimitiveType::PLANE)
			{
				if (contacts[i].body2 == contacts[contactIndex].body1)
				{
					AdjustDeltaVelocity(&contacts[contactIndex], &contacts[i], 0, contacts[i].relContactPos2, false);
				}
				if (contacts[i].body2 == contacts[contactIndex].body2)
				{
					AdjustDeltaVelocity(&contacts[contactIndex], &contacts[i], 1, contacts[i].relContactPos2, false);
				}
			}
		}		
	}
}

void CollisionResolution2::CreateCSVFile()
{
	file.open("penetration.csv");
	file.clear();
	file << "Contact,Iteration,Value\n";
	std::cout << "Created CSV file" << std::endl;
}

void CollisionResolution2::WriteToFile(float value, unsigned int obj, unsigned int iter)
{
	file << std::to_string(obj) << "," + std::to_string(iter) << "," << std::to_string(value) << "\n";
}

void CollisionResolution2::AdjustDeltaVelocity(Contact* thisContact, Contact* otherContact, const unsigned int bt, const Vector3& rcp, bool sign)
{
	Vector3 deltaVelocity = thisContact->velocityChange[bt]
		+ thisContact->rotationChange[bt].VectorProduct(rcp);
	Mathe::Transform(deltaVelocity, otherContact->worldToContact);
	otherContact->closingVelocity += deltaVelocity * (sign ? 1.0 : -1.0);
	otherContact->CalculateDesiredDeltaVelocity();
}
