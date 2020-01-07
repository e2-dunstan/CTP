#pragma once
#include "Primitive.h"

struct Contact
{
	Contact() = default;
	Contact(Primitive* prim1, Primitive* prim2)
		: body1(prim1), body2(prim2) {};
	~Contact()
	{
		/*
		if (body1 != nullptr)
		{
			delete body1;
			body1 = nullptr;
		}
		if (body2 != nullptr)
		{
			delete body2;
			body2 = nullptr;
		}
		*/
	};

	Vector3 point;
	Vector3 normal;
	float penetrationDepth = 0;

	Primitive* body1;
	Primitive* body2;

	//default value for rubber ball as per book = 0.4
	float restitution = 0.4;
};

struct CollisionData
{
	CollisionData() = default;
	~CollisionData() = default;
	
	//Collision response will reference contacts.
	std::vector<Contact> contacts = std::vector<Contact>();
	int contactsLeft = 5;
	float tolerance = 0.5f;
};

struct PotentialContact
{
	PotentialContact() = default;
	PotentialContact(Primitive* _p1, Primitive* _p2)
		: prim1(_p1), prim2(_p2) {};
	~PotentialContact()
	{
		/*
		if (prim1 != nullptr)
		{
			delete prim1;
			prim1 = nullptr;
		}
		if (prim2 != nullptr)
		{
			delete prim2;
			prim2 = nullptr;
		}
		*/
	};

	Primitive* prim1;
	Primitive* prim2;
};