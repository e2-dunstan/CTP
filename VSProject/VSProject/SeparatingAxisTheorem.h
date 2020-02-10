#pragma once

struct Primitive;
struct Vector3;
struct CollisionVolume;
struct Contact;

class SAT
{
	friend class CollisionFine;
public:
	SAT() = default;
	~SAT() = default;

	Contact* Test(Primitive* box1, Primitive* box2);

protected:

	//Separating axis theorem
	double GetPositionOnAxis(const CollisionVolume* box, const Vector3& axis);
	bool BoxesOverlapOnAxis(const CollisionVolume* box1, const CollisionVolume* box2, const Vector3& toCentre, Vector3 axis,
		int index, float& smallestPenetration, int& smallestIndex);
	Contact* PointFaceCollision(Primitive* box1, Primitive* box2, const Vector3& toCentre, int smallest, float penetration);
	Vector3 GetContactPoint(const Vector3& edgePoint1, const Vector3& edgePoint2, Vector3& axisOne, Vector3& axisTwo, float halfSize1, float halfSize2, bool useOneMidpoint);
};