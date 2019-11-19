#pragma once
#include "CollisionVolume.h"
#include "Contact.h"


struct CollisionData
{
	//Collision response will reference contacts.
	std::vector<Contact> contacts;
	int contactsLeft = 5;
	float tolerance = 0.5f;
};

class CollisionFine
{
public:
	CollisionFine() = default;
	~CollisionFine() = default;

	void DetectContacts(Primitive* prim1, Primitive* prim2);

	std::string outputStr;

private:
	//Separating axis theorem
	float PositionOnAxis(const Primitive* box, const Vector3& axis);
	bool BoxesOverlapOnAxis(const Primitive* box1, const Primitive* box2, const Vector3& toCentre, const Vector3& axis);
	void BoxAndBox(const Primitive* box1, const Primitive* box2);
	//std::vector<const Vector3&> GetBoxAxes(const Primitive* box);

	void SphereAndBox(Primitive* sphere, Primitive* box, Vector3& spherePosition, float radius);
	void SphereAndSphere(Primitive* prim1, Primitive* prim2, const Vector3& position1, float radius1, const Vector3& position2, float radius2);
	void SphereAndPlane(Primitive* sphere, Primitive* plane, const Vector3& spherePosition, float radius, const Vector3& planePosition, const Vector3& normal);
	void BoxAndPlane(Primitive* box, Primitive* plane, const Vector3& planePosition, const Vector3& normal);

	std::unique_ptr<CollisionData> data = std::make_unique<CollisionData>();
};