#include "Collisions.h"

void Collisions::DetectCoarse(BoundingVolume& vol1, BoundingVolume& vol2)
{
	if (coarse->Overlapping(vol1, vol2))
	{
		potentialContacts.push_back(PotentialContact(&vol1.primitive, &vol2.primitive));
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

	resolution2->ResolveContacts(data->contacts);

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
