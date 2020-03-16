#pragma once
#include "Primitive.h"

class Collisions;
class CollisionResolution2;

enum class BounceCombineType
{
	MAXIMUM, MULTIPLY, MINIMUM, AVERAGE
};

struct Contact
{
	friend struct CollisionData;
	friend class Collisions;
	friend class CollisionResolution2;

public:
	//Contact() = default;
	Contact(Primitive* prim1, Primitive* prim2)
		: body1(prim1), body2(prim2) 
	{
		if (body1->type != PrimitiveType::PLANE) body1->colliding = true;
		if (body2->type != PrimitiveType::PLANE) body2->colliding = true;
	};
	~Contact() = default;

	Vector3 point;
	Vector3 normal;
	float penetrationDepth = 0;

	Primitive* body1;
	Primitive* body2;

	//default value for rubber ball as per book = 0.4
	float restitution = 0.0f;
	float friction = 0.0f;


protected:
	void PrepareResolution();

	void ResolveContactPenetration();
	void ApplyAngularMoveLimit(float& linear, float& angular, const float projection/*const float objMag*/);

	void ResolveContactVelocity();
	Vector3 FrictionlessImpulse();
	Vector3 FrictionImpulse();

	void MatchRigidbodyAwakeStates();

	Matrix3 worldToContact = Matrix3();

	Vector3 closingVelocity = Vector3();


	Vector3 relContactPos1 = Vector3();
	Vector3 relContactPos2 = Vector3();

	Vector3 linearChange[2] = { Vector3(), Vector3() };
	Vector3 angularChange[2] = { Vector3(), Vector3() };

	Vector3 velocityChange[2] = { Vector3(), Vector3() };
	Vector3 rotationChange[2] = { Vector3(), Vector3() };

private:
	void CalculateContactBasisMatrices();
	void CalculateDesiredDeltaVelocity();
	void CalculateClosingVelocities();

	float desiredDeltaVelocity = 0;
	Matrix3 contactToWorld = Matrix3();

	BounceCombineType bounceCombineType = BounceCombineType::AVERAGE;

};

struct CollisionData
{
	CollisionData() = default;
	~CollisionData() = default;
	
	//Collision response will reference contacts.
	std::vector<Contact> contacts = std::vector<Contact>();
	float tolerance = 0.5f;

	void SortContactsByPenetration();
	void SortContactsByVelocityMag();
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