#include "Collisions.h"

Collisions::Collisions()
{
	fine = std::make_unique<CollisionFine>(data->contacts);
	sat = std::make_unique<SAT>(data->contacts);
}

void Collisions::DetectCoarse(Primitive* prim1, Primitive* prim2)
{
	if (coarse->Overlapping(prim1->boundingVolume, prim2->boundingVolume))
	{
		potentialContacts.push_back(PotentialContact(prim1, prim2));
	}
}

void Collisions::DetectFine()
{
	for (unsigned i = 0; i < potentialContacts.size(); i++)
	{
		if (potentialContacts[i].prim1->type == Primitive::Type::BOX && potentialContacts[i].prim2->type == Primitive::Type::BOX)
			sat->Test(potentialContacts[i].prim1, potentialContacts[i].prim2);
		else
			fine->DetectContacts(potentialContacts[i].prim1, potentialContacts[i].prim2);
	}
	potentialContacts.clear();
}

void Collisions::Resolution()
{
	if (data->contacts.size() <= 0) return;

	for (unsigned i = 0; i < data->contacts.size(); i++)
	{
		data->contacts[i].PrepareResolution();
	}
	resolution2->PenetrationResolution(data->contacts);
	resolution2->VelocityResolution(data->contacts);

	data->contacts.clear();
}
