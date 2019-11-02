#pragma once
#include "Primitive.h"
#include "Contact.h"

struct CollisionData
{
	std::unique_ptr<Contact> contacts = std::make_unique<Contact>();
	int contactsLeft;
};


class Collisions
{
public:
	Collisions() = default;
	~Collisions() = default;

	//void DetectCoarse(RigidBody* rb);
	//void DetectFine(RigidBody* rb);

	//void Response(RigidBody* rb);

	void DetectContacts(Primitive* rb1, Primitive* rb2, CollisionData* data);

private:

	void SphereAndSphere(Primitive* rb1, Primitive* rb2, Vector3 position1, float radius1, Vector3 position2, float radius2);
	void SphereAndPlane();

	std::unique_ptr<CollisionData> data = std::make_unique<CollisionData>();

};