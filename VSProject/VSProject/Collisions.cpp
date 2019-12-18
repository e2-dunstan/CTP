#include "Collisions.h"

void Collisions::DetectCoarse(Primitive* prim1, Primitive* prim2, bool output)
{
	if (coarse->Overlapping(prim1->boundingVolume, prim2->boundingVolume))
	{
		potentialContacts.push_back(PotentialContact(prim1, prim2));
	}
}

void Collisions::DetectFine(bool output)
{
	for (auto potentialContact : potentialContacts)
	{
		fine->DetectContacts(potentialContact.prim1, potentialContact.prim2);
	}
}

void Collisions::Resolution()
{
	if (data->contacts.size() <= 0) return;

	for (auto contact : data->contacts)
	{
		resolution->Resolve(contact);
	}

	data->contacts.clear();
}
