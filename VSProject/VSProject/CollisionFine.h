#pragma once
#include "CollisionVolume.h"
#include "CollisionData.h"


class CollisionFine
{
public:
	CollisionFine() = default;
	CollisionFine(std::vector<Contact>& _contacts) : contacts(_contacts) {};
	~CollisionFine() = default;

	void DetectContacts(Primitive* prim1, Primitive* prim2);

private:

	struct ContactPoint
	{
		Vector3 point = Vector3();
		double penetration = 0.0;

		double weighting = 1.0;
	};

	std::vector<Contact>& contacts;

	double PositionOnAxis(const Box* box, const Vector3& axis);

	//PrimitiveType checks
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

	void SphereAndSphere(Sphere* prim1, Sphere* prim2, const Vector3& position1, float radius1, const Vector3& position2, float radius2);
	void SphereAndPlane(Sphere* sphere, Plane* plane, const Vector3& spherePosition, float radius, const Vector3& planePosition, const Vector3& normal);
	void SphereAndBox(Sphere* sphere, Box* box, Vector3& spherePosition, float radius);
	
	//void BoxAndBox(Primitive* box1, Primitive* box2);
	void BoxAndPlane(Box* box, Plane* plane, const Vector3& planePosition, const Vector3& normal);
	
	//void CylinderAndCylinder(Primitive* prim1, Primitive* prim2); //TOO COMPLEX AT THE MOMENT
	//void CylinderAndCapsule(Primitive* cyl, Primitive* cap);
	//void CylinderAndPlane(Primitive* cyl, Primitive* plane);
	//void CylinderAndSphere(Primitive* cyl, Primitive* sphere);
	
	//void CapsuleAndCapsule(Primitive* prim1, Primitive* prim2);
	//void CapsuleAndBox(Primitive* cap, Primitive* box);
	//void CapsuleAndPlane(Primitive* cap, Primitive* plane);
	//void CapsuleAndSphere(Primitive* cap, Primitive* sphere);

	//CYLINDER COLLISION DETECTION IS DIFFICULT FOR SOME PRIMIIVES, THEREFORE
	//THE COLLISION VOLUME IS A CAPSULE FOR THESE DIFFICULT ONES INSTEAD

	float tolerance = 0.5f;
};