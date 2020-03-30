#pragma once
#include "Contact.h"

struct CollisionData
{
	CollisionData() = default;
	~CollisionData() = default;
	
	//Collision response will reference contacts.
	std::vector<Contact> contacts = std::vector<Contact>();
	float tolerance = 0.5f;

	void SortContactsByPenetration()
	{
		std::sort(contacts.begin(), contacts.end(),
			[](Contact const& a, Contact const& b) -> bool
			{ return a.penetrationDepth > b.penetrationDepth; });
	}
	void SortContactsByVelocityMag()
	{
		std::sort(contacts.begin(), contacts.end(),
			[](Contact const& a, Contact const& b) -> bool
			{ return a.desiredDeltaVelocity > b.desiredDeltaVelocity; });
	}
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