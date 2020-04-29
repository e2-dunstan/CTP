#pragma once
#include "Contact.h"


/*struct Batch
{
	std::vector<Contact> contacts;
};*/

struct CollisionData
{
	CollisionData() = default;
	~CollisionData() = default;
	
	std::vector<Contact> contacts = std::vector<Contact>();

	/*void BatchContacts() 
	{
		std::vector<Contact> unsorted = std::vector<Contact>(contacts);

		while (unsorted.size() > 0)
		{
			Batch batch;
			Contact thisContact = unsorted[0];
			unsorted.erase(unsorted.begin());
			batch.contacts.push_back(thisContact);

			unsigned int iter = 0;
			while (iter < unsorted.size())
			{
				Contact contactToCompare = unsorted[iter];
				if ((contactToCompare.body1 == thisContact.body1 && contactToCompare.body2 == thisContact.body2)
					|| (contactToCompare.body1 == thisContact.body2 && contactToCompare.body2 == thisContact.body1))
				{
					batch.contacts.push_back(contactToCompare);
					unsorted.erase(unsorted.begin() + iter);
				}
				else
					iter++;
			}
			batchedContacts.push_back(batch);
		}
		unsorted.clear();
		contacts.clear();
	}
	//Collision response will reference contacts.
	//std::vector<Batch> batchedContacts = std::vector<Batch>();*/

};

struct PotentialContact
{
	PotentialContact() = default;
	PotentialContact(Primitive* _p1, Primitive* _p2)
		: prim1(_p1), prim2(_p2) {};
	~PotentialContact() = default;

	Primitive* prim1 = nullptr;
	Primitive* prim2 = nullptr;
};