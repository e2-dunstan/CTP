#pragma once
#include "CollisionVolume.h"
#include "Contact.h"


struct CollisionData
{
	std::vector<Contact*> contacts;
	int contactsLeft = 1;
	float tolerance = 0.5f;
};

class CollisionFine
{
public:
	CollisionFine() = default;
	~CollisionFine() = default;

	void DetectContacts(Primitive* prim1, Primitive* prim2);

private:

	void SphereAndSphere(Primitive* prim1, Primitive* prim2, Vector3 position1, float radius1, Vector3 position2, float radius2);
	void SphereAndPlane(Primitive* sphere, Primitive* plane, Vector3 spherePosition, float radius, Vector3 planePosition, Vector3 normal);
	void BoxAndPlane(Primitive* box, Primitive* plane, Vector3 planePosition, Vector3 normal);

	std::unique_ptr<CollisionData> data = std::make_unique<CollisionData>();
};