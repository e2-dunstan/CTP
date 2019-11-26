#pragma once
#include <vector>
#include "Contact.h"

struct CollisionData
{
	CollisionData() = default;
	~CollisionData() = default;

	//Collision response will reference contacts.
	std::vector<Contact> contacts = std::vector<Contact>();
	int contactsLeft = 5;
	float tolerance = 0.5f;
};