#pragma once
#include "Primitive.h"

class CollisionResolution2;

struct Contact
{
	friend class CollisionResolution2;

public:
	Contact() = default;
	Contact(Primitive* prim1, Primitive* prim2)
		: body1(prim1), body2(prim2) {};
	~Contact()
	{
		//if (body1)
		//{
		//	delete body1;
		//}
		//if (body2)
		//{
		//	delete body2;
		//}
	};

	Vector3 point;
	Vector3 normal;
	float penetrationDepth = 0;

	Primitive* body1;
	Primitive* body2;

	//default value for rubber ball as per book = 0.4
	float restitution = 0.0f;
	float friction = 0.0f;

protected:
	void CalculateContactBasisMatrices();
	void CalculateResolutionValues();
	void CalculateDesiredDeltaVelocity();
	void CalculateClosingVelocities();

	void ApplyPositionChange();
	void ApplyAngularMoveLimit(float& linear, float& angular, const float objMag);

	void ApplyVelocityChange();
	Vector3 FrictionlessImpulse();
	Vector3 FrictionImpulse();

	//func to make RBs awake here

	Matrix contactToWorld = Matrix(4,4);
	Matrix worldToContact = Matrix(4,4);

	Vector3 closingVelocity = Vector3();

	float desiredDeltaVelocty = 0;

	Vector3 relContactPos1 = Vector3();
	Vector3 relContactPos2 = Vector3();

	Vector3 linearChange[2] = { Vector3(), Vector3() };
	Vector3 angularChange[2] = { Vector3(), Vector3() };

	Vector3 velocityChange[2] = { Vector3(), Vector3() };
	Vector3 rotationChange[2] = { Vector3(), Vector3() };
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
		//if (prim1)
		//{
		//	delete prim1;
		//}
		//if (prim2)
		//{
		//	delete prim2;
		//}
	};

	Primitive* prim1;
	Primitive* prim2;
};