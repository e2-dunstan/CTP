#pragma once
#include "CollisionVolume.h"
#include "CollisionData.h"
#include "SeparatingAxisTheorem.h"


class CollisionFine
{
public:
	CollisionFine() = default;
	CollisionFine(std::vector<Contact>& _contacts) : contacts(_contacts) {};
	~CollisionFine() = default;

	void DetectContacts(Primitive* prim1, Primitive* prim2);

	std::string outputStr;

private:

	std::unique_ptr<SAT> sat = std::make_unique<SAT>();

	double PositionOnAxis(const Primitive* box, const Vector3& axis);
	//bool BoxesOverlapOnAxis(const Primitive* box1, const Primitive* box2, const Vector3& toCentre, Vector3 axis, 
	//	int index, float& smallestPenetration, int& smallestIndex);
	//void PointFaceCollision(Primitive* box1, Primitive* box2, const Vector3& toCentre, int smallest, float penetration);
	//Vector3 GetContactPoint(const Vector3& edgePoint1, const Vector3& edgePoint2, Vector3& axisOne, Vector3& axisTwo, float halfSize1, float halfSize2, bool useOneMidpoint);

	//Type checks
	/* TYPE CHECKS: (total of 24 if statements)
	//Plane and sphere x2
	//Plane and box x2
	//Plane and cylinder x2
	//Plane and capsule x2
	//Box and box
	//Box and sphere x2
	//Box and cylinder x2
	//Box and capsule x2
	//Sphere and sphere
	//Sphere and cylinder x2
	//Sphere and capsule x2
	//Cylinder and cylinder
	//Cylinder and capsule x2
	//Capsule and capsule
	*/
	void SphereAndSphere(Primitive* prim1, Primitive* prim2, const Vector3& position1, float radius1, const Vector3& position2, float radius2);
	void SphereAndPlane(Primitive* sphere, Primitive* plane, const Vector3& spherePosition, float radius, const Vector3& planePosition, const Vector3& normal);
	void SphereAndBox(Primitive* sphere, Primitive* box, Vector3& spherePosition, float radius);
	
	void BoxAndBox(Primitive* box1, Primitive* box2);
	void BoxAndPlane(Primitive* box, Primitive* plane, const Vector3& planePosition, const Vector3& normal);
	
	//void CylinderAndCylinder(Primitive* prim1, Primitive* prim2); //TOO COMPLEX AT THE MOMENT
	//void CylinderAndCapsule(Primitive* cyl, Primitive* cap);
	void CylinderAndPlane(Primitive* cyl, Primitive* plane);
	void CylinderAndSphere(Primitive* cyl, Primitive* sphere);
	
	void CapsuleAndCapsule(Primitive* prim1, Primitive* prim2);
	void CapsuleAndBox(Primitive* cap, Primitive* box);
	void CapsuleAndPlane(Primitive* cap, Primitive* plane);
	void CapsuleAndSphere(Primitive* cap, Primitive* sphere);

	//CYLINDER COLLISION DETECTION IS DIFFICULT FOR SOME PRIMIIVES, THEREFORE
	//THE COLLISION VOLUME IS A CAPSULE FOR THESE DIFFICULT ONES INSTEAD

	//std::unique_ptr<CollisionData> data = std::make_unique<CollisionData>();
	std::vector<Contact>& contacts;

	float tolerance = 0.5f;
};