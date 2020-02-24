#pragma once
#include "CollisionData.h"

//currently implemented only to work with boxes

class SAT
{
public:
	//SAT() = default;
	SAT(std::vector<Contact> & _contacts) : contacts(_contacts) {};
	~SAT() = default;

	void Test(Primitive* box1, Primitive* box2);

	void GetContactData(int& smallestIndex, Primitive* box1, Primitive* box2, const Vector3& toCentre, float smallestPenetration, int singleSmallestIndex);

private:
	std::vector<Contact>& contacts;

	double GetPositionOnAxis(const CollisionVolume* box, const Vector3& axis);
	bool BoxesOverlapOnAxis(const CollisionVolume* box1, const CollisionVolume* box2, const Vector3& toCentre, Vector3 axis,
		int index, float& smallestPenetration, int& smallestIndex);

	void PointFaceCollision(Primitive* box1, Primitive* box2, const Vector3& toCentre, int smallest, float penetration);
	Vector3 GetEdgeContactPoint(const Vector3& edgePoint1, const Vector3& edgePoint2, Vector3& axisOne, Vector3& axisTwo, float halfSize1, float halfSize2, bool useOneMidpoint);

	bool SetReferenceVertices(const Vector3& normal, Vector3* planes, const Vector3& halfSize);
	void SetReferenceMinMax(const Vector3& normal, const Vector3& halfSize, Vector3& min, Vector3& max);

	Vector3 CalculateIntersection(const Vector3& v1, const Vector3& v2, const unsigned axes[2], const Vector3& clippingMin, const Vector3& clippingMax);

	void SutherlandHodgmanBoxes(std::vector<Vector3>& clipped, const Vector3& normal, const Vector3* polyVertices, const Vector3* clippingVertices);
	bool InsideEdge(double px, double py, double edgeMaxX, double edgeMaxY, double edgeMinX, double edgeMinY);
};

//Types of box collision:
// - point face
// - point edge
// - point point (VERY unlikely)
// - face face (replace with 2 edge edge and 2 point face)
// - face edge (replace with point face and edge edge)
// - edge edge