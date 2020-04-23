#include "CollisionResolution.h"

void CollisionResolution::PenetrationResolution(std::vector<Contact>& contacts)
{
	const float penetrationEpsilon = 0.00001f;
	numContacts = contacts.size();

	if (Global::writeContactDataToFile) CreateCSVFile();

	bool iterationsComplete = false;

	for (unsigned iter = 0; iter < maxPenetrationIterations; iter++)
	{
		if (iterationsComplete)
		{
			//std::cout << "Penetration iterations finished early " << iter << std::endl;
			break; //early out
		}
		iterationsComplete = true;

		//This is good but expensive
		//SortContactsByPenetration(contacts);

		for (unsigned int i = 0; i < numContacts; i++)
		{
			unsigned contactIndex = i;

			if (contacts[contactIndex].IsPenetrationResolved(penetrationEpsilon)) continue;
			else iterationsComplete = false;

			if (Global::writeContactDataToFile) WriteToFile(contacts[contactIndex].penetrationDepth, contactIndex, i / numContacts);

			contacts[contactIndex].ResolvePenetration();

			Vector3 deltaPosition;
			//Check other contacts for effects from previous resolution
			for (unsigned i = 0; i < numContacts; i++)
			{
				if (!contacts[i].body1->isStatic)
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
				if (!contacts[i].body2->isStatic)
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

	if (Global::writeContactDataToFile)
	{
		file.close();
		Global::writeContactDataToFile = false;
	}
}

void CollisionResolution::VelocityResolution(std::vector<Contact>& contacts)
{
	const float velocityEpsilon = 0.01f;
	numContacts = contacts.size();

	bool iterationsComplete = false;

	for (unsigned int iter = 0; iter < maxVelocityIterations; iter++)
	{
		if (iterationsComplete)
		{
			//std::cout << "Velocity iterations finished early " << iter << std::endl;
			break; //early out
		}
		iterationsComplete = true;

		for (unsigned i = 0; i < numContacts; i++)
		{
			unsigned contactIndex = i;

			if (contacts[contactIndex].IsVelocityResolved(velocityEpsilon)) continue;
			else iterationsComplete = false;

			contacts[contactIndex].ResolveVelocity();

			//Match RB awake states
			if (!contacts[contactIndex].body1->isStatic && !contacts[contactIndex].body2->isStatic
				&& contacts[contactIndex].body1->rigidbody.GetMotion() > contacts[contactIndex].body1->rigidbody.sleepThreshold
				&& contacts[contactIndex].body2->rigidbody.GetMotion() > contacts[contactIndex].body2->rigidbody.sleepThreshold)
			{
				contacts[contactIndex].MatchRigidbodyAwakeStates();
			}

			//Note to self: the signs are correct! Don't touch them!

			for (unsigned i = 0; i < numContacts; i++)
			{
				if (!contacts[i].body1->isStatic)
				{
					if (contacts[i].body1 == contacts[contactIndex].body1)
					{
						AdjustDeltaVelocity(contacts[contactIndex], contacts[i], 0, contacts[i].relContactPos1, true);
					}
					if (contacts[i].body1 == contacts[contactIndex].body2)
					{
						AdjustDeltaVelocity(contacts[contactIndex], contacts[i], 1, contacts[i].relContactPos1, true);
					}
				}
				if (!contacts[i].body2->isStatic)
				{
					if (contacts[i].body2 == contacts[contactIndex].body1)
					{
						AdjustDeltaVelocity(contacts[contactIndex], contacts[i], 0, contacts[i].relContactPos2, false);
					}
					if (contacts[i].body2 == contacts[contactIndex].body2)
					{
						AdjustDeltaVelocity(contacts[contactIndex], contacts[i], 1, contacts[i].relContactPos2, false);
					}
				}
			}
		}
	}
}

void CollisionResolution::CreateCSVFile()
{
	file.open("penetration.csv");
	file.clear();
	file << "Contact,Iteration,Value\n";
	std::cout << "Created CSV file" << std::endl;
}

void CollisionResolution::WriteToFile(float value, unsigned int obj, unsigned int iter)
{
	file << std::to_string(obj) << "," + std::to_string(iter) << "," << std::to_string(value) << "\n";
}

void CollisionResolution::AdjustDeltaVelocity(Contact& thisContact, Contact& otherContact, const unsigned int bt, const Vector3& rcp, bool sign)
{
	Vector3 deltaVelocity = thisContact.velocityChange[bt]
		+ thisContact.rotationChange[bt].VectorProduct(rcp);
	Mathe::Transform(deltaVelocity, otherContact.worldToContact);
	otherContact.closingVelocity += deltaVelocity * (sign ? 1.0 : -1.0);
	otherContact.CalculateDesiredDeltaVelocity();
}

void CollisionResolution::SortContactsByPenetration(std::vector<Contact>& contacts)
{
	std::sort(contacts.begin(), contacts.end(),
		[](Contact const& a, Contact const& b) -> bool
		{ return a.penetrationDepth > b.penetrationDepth; });
}

void CollisionResolution::SortContactsByVelocityMag(std::vector<Contact>& contacts)
{
	std::sort(contacts.begin(), contacts.end(),
		[](Contact const& a, Contact const& b) -> bool
		{ return a.desiredDeltaVelocity > b.desiredDeltaVelocity; });
}
