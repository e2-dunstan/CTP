#include "Collisions.h"

void Collisions::DetectCoarse(Primitive* prim1, Primitive* prim2)
{
	if (coarse->Overlapping(prim1->boundingVolume, prim2->boundingVolume))
	{
		potentialContacts.push_back(PotentialContact(prim1, prim2));
	}
}

void Collisions::DetectFine()
{
	for (auto potentialContact : potentialContacts)
	{
		fine->DetectContacts(potentialContact.prim1, potentialContact.prim2);
	}
	potentialContacts.clear();
}

void Collisions::Resolution()
{
	if (data->contacts.size() <= 0) return;

	//for (auto contact : data->contacts)
	//{
	//	//if (contact.body1->rigidbody.IsAtRest()
	//	//	&& contact.body2->rigidbody.IsAtRest())
	//	//{
	//	//	continue;
	//	//}
	//	resolution->ResolveCollision(contact);
	//}

	data->contacts.clear();
}
