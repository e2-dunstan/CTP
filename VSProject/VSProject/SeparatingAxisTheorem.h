#pragma once
#include "CollisionData.h"

//currently implemented only to work with boxes

class SAT
{
public:
	SAT(std::vector<Contact>& _contacts) : contacts(_contacts) {};
	~SAT() = default;

	void Test(Primitive* box1, Primitive* box2);

	void GetContactData(int& smallestIndex, Box* box1, Box* box2, const Vector3& toCentre, float smallestPenetration, int singleSmallestIndex);

private:

	//plane struct here
	struct SATPlane
	{
		Vector3 normal;
		Vector3 points[4];
		Matrix4 matrix;
	};


	std::vector<Contact>& contacts;

	bool simpleTest = false;

	float GetPositionOnAxis(const BoxCV& box, const Vector3& axis);
	bool BoxesOverlapOnAxis(const BoxCV& box1, const BoxCV& box2, const Vector3& toCentre, Vector3 axis,
		int index, float& smallestPenetration, int& smallestIndex);

	void PointFaceCollisionSimple(Box* box1, Box* box2, const Vector3& toCentre, int smallest, float penetration);
	void PointFaceCollision(Box* box1, Box* box2, const Vector3& toCentre, int smallest, float penetration);
	Vector3 GetEdgeContactPoint(const Vector3& edgePoint1, const Vector3& edgePoint2, Vector3& axisOne, Vector3& axisTwo, float halfSize1, float halfSize2, bool useOneMidpoint);

	bool SetReferenceVertices(const Vector3& normal, Vector3* planes, const Vector3& halfSize);
	void SetReferenceMinMax(const Vector3& normal, const Vector3& halfSize, Vector3& min, Vector3& max);

	Vector3 CalculateIntersection(const Vector3& v1, const Vector3& v2, const uint8_t axes[2], const Vector3& clippingMin, const Vector3& clippingMax);

	void SutherlandHodgman(std::vector<Vector3>& clipped, const Vector3& normal, const Vector3* polyVertices, const Vector3* clippingVertices);
	bool InsideEdge(float px, float py, float edgeMaxX, float edgeMaxY, float edgeMinX, float edgeMinY);
	void VerifyVertex(std::vector<Vector3>& _clipped, const Vector3& vec, const Vector3& max, const Vector3& min, const uint8_t axes[]);
};

//Types of box collision:
// - point face
// - point edge
// - point point (VERY unlikely)
// - face face (replace with 2 edge edge and 2 point face)
// - face edge (replace with point face and edge edge)
// - edge edge