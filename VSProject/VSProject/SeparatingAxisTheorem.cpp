#include "SeparatingAxisTheorem.h"
#include "Primitive.h"
#include "CollisionData.h"

Contact* SAT::Test(Primitive* box1, Primitive* box2)
{
	Vector3 toCentre = box2->collisionVolume.centre - box1->collisionVolume.centre;
	float smallestPenetration = 1000;
	int smallestIndex = 0;

	CollisionVolume* cv1 = &box1->collisionVolume;
	CollisionVolume* cv2 = &box2->collisionVolume;

	//Early-outs if any axes are found to be not overlapping.
	//Box1 axes
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(0, cv1->axisMat), 0, smallestPenetration, smallestIndex)) return nullptr;
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(1, cv1->axisMat), 1, smallestPenetration, smallestIndex)) return nullptr;
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(2, cv1->axisMat), 2, smallestPenetration, smallestIndex)) return nullptr;
	//Box2 axes																														    
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(0, cv2->axisMat), 3, smallestPenetration, smallestIndex)) return nullptr;
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(1, cv2->axisMat), 4, smallestPenetration, smallestIndex)) return nullptr;
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(2, cv2->axisMat), 5, smallestPenetration, smallestIndex)) return nullptr;

	int singleSmallestIndex = smallestIndex;
	//Cross product axes
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(0, cv1->axisMat).VectorProduct(Mathe::GetAxis(0, cv2->axisMat)), 6, smallestPenetration, smallestIndex)) return nullptr;
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(0, cv1->axisMat).VectorProduct(Mathe::GetAxis(1, cv2->axisMat)), 7, smallestPenetration, smallestIndex)) return nullptr;
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(0, cv1->axisMat).VectorProduct(Mathe::GetAxis(2, cv2->axisMat)), 8, smallestPenetration, smallestIndex)) return nullptr;
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(1, cv1->axisMat).VectorProduct(Mathe::GetAxis(0, cv2->axisMat)), 9, smallestPenetration, smallestIndex)) return nullptr;
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(1, cv1->axisMat).VectorProduct(Mathe::GetAxis(1, cv2->axisMat)), 10, smallestPenetration, smallestIndex)) return nullptr;
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(1, cv1->axisMat).VectorProduct(Mathe::GetAxis(2, cv2->axisMat)), 11, smallestPenetration, smallestIndex)) return nullptr;
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(2, cv1->axisMat).VectorProduct(Mathe::GetAxis(0, cv2->axisMat)), 12, smallestPenetration, smallestIndex)) return nullptr;
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(2, cv1->axisMat).VectorProduct(Mathe::GetAxis(1, cv2->axisMat)), 13, smallestPenetration, smallestIndex)) return nullptr;
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(2, cv1->axisMat).VectorProduct(Mathe::GetAxis(2, cv2->axisMat)), 14, smallestPenetration, smallestIndex)) return nullptr;

	//Point-face collision
	if (smallestIndex < 3)
	{
		//std::cout << "point face index < 3" << std::endl;
		return PointFaceCollision(box1, box2, toCentre, smallestIndex, smallestPenetration);
	}
	else if (smallestIndex < 6)
	{
		//std::cout << "point face index < 6" << std::endl;
		return PointFaceCollision(box2, box1, toCentre * -1.0f, smallestIndex - 3, smallestPenetration);
	}
	//Edge-edge collision
	else
	{
		//std::cout << "edge edge" << std::endl;

		smallestIndex -= 6;
		unsigned box1AxisIndex = smallestIndex / 3;
		unsigned box2AxisIndex = smallestIndex % 3;
		Vector3 box1Axis = Mathe::GetAxis(box1AxisIndex, box1->collisionVolume.axisMat);
		Vector3 box2Axis = Mathe::GetAxis(box2AxisIndex, box2->collisionVolume.axisMat);
		Vector3 axis = box1Axis.VectorProduct(box2Axis);
		axis.Normalise();

		if (toCentre.ScalarProduct(axis) > 0)
			axis *= -1.0f;

		Vector3 ptOnOneEdge = box1->collisionVolume.halfSize;
		Vector3 ptOnTwoEdge = box2->collisionVolume.halfSize;

		for (int i = 0; i < 3; i++)
		{
			if (box1AxisIndex == i) 
				ptOnOneEdge[i] = 0;
			else if (Mathe::GetAxis(i, box1->collisionVolume.axisMat).ScalarProduct(axis) > 0) 
				ptOnOneEdge[i] = -ptOnOneEdge[i];

			if (box2AxisIndex == i) 
				ptOnTwoEdge[i] = 0;
			else if (Mathe::GetAxis(i, box2->collisionVolume.axisMat).ScalarProduct(axis) < 0) 
				ptOnTwoEdge[i] = -ptOnTwoEdge[i];
		}

		Mathe::Transform(ptOnOneEdge, box1->transform);
		Mathe::Transform(ptOnTwoEdge, box2->transform);

		Vector3 vertex = GetContactPoint(ptOnOneEdge, ptOnTwoEdge, box1Axis, box2Axis,
			(float)box1->collisionVolume.halfSize[box1AxisIndex], (float)box2->collisionVolume.halfSize[box2AxisIndex],
			singleSmallestIndex > 2);

		//GENERATE CONTACT
		Contact contact(box1, box2);
		contact.penetrationDepth = smallestPenetration;
		contact.normal = axis;
		contact.point = vertex;
		return new Contact(contact); 
	}

}

double SAT::GetPositionOnAxis(const CollisionVolume* box, const Vector3& axis)
{
	return
		box->halfSize.x * abs(axis.ScalarProduct(Mathe::GetAxis(0, box->axisMat)))
		+ box->halfSize.y * abs(axis.ScalarProduct(Mathe::GetAxis(1, box->axisMat)))
		+ box->halfSize.z * abs(axis.ScalarProduct(Mathe::GetAxis(2, box->axisMat)));
}

bool SAT::BoxesOverlapOnAxis(const CollisionVolume* box1, const CollisionVolume* box2, const Vector3& toCentre, Vector3 axis, int index, float& smallestPenetration, int& smallestIndex)
{
	if (axis.SquaredMagnitude() < 0.0001) return true;
	axis = axis.Normalise();

	float box1AxisPos = (float)GetPositionOnAxis(box1, axis);// / 2.0f;
	float box2AxisPos = (float)GetPositionOnAxis(box2, axis);// / 2.0f;

	float distance = (float)abs(toCentre.ScalarProduct(axis));

	float penetration = box1AxisPos + box2AxisPos - distance;
	if (penetration < 0) return false;
	if (penetration < smallestPenetration)
	{
		smallestPenetration = penetration;
		smallestIndex = index;
	}
	return true;
}

Contact* SAT::PointFaceCollision(Primitive* box1, Primitive* box2, const Vector3& toCentre, int smallest, float penetration)
{



	Vector3 normal = Mathe::GetAxis(smallest, box1->collisionVolume.axisMat);
	if (normal.ScalarProduct(toCentre) > 0)
	{
		normal *= -1.0f;
	}

	Vector3 vertex = box2->collisionVolume.halfSize * 2.0;
	//if less than zero, then switch vertex sign
	if (Mathe::GetAxis(0, box2->collisionVolume.axisMat).ScalarProduct(normal) < 0) vertex.x = -vertex.x;
	if (Mathe::GetAxis(1, box2->collisionVolume.axisMat).ScalarProduct(normal) < 0) vertex.y = -vertex.y;
	if (Mathe::GetAxis(2, box2->collisionVolume.axisMat).ScalarProduct(normal) < 0) vertex.z = -vertex.z;

	Contact contact(box1, box2);
	contact.normal = normal;
	contact.penetrationDepth = penetration * 2.0;
	Mathe::Transform(vertex, box2->collisionVolume.axisMat);
	contact.point = vertex;
	return new Contact(contact);
}

Vector3 SAT::GetContactPoint(const Vector3& edgePoint1, const Vector3& edgePoint2, Vector3& axisOne, Vector3& axisTwo, float halfSize1, float halfSize2, bool useOneMidpoint)
{
	Vector3 contact1, contact2;

	float squareMag1 = (float)axisOne.SquaredMagnitude();
	float squareMag2 = (float)axisTwo.SquaredMagnitude();
	float scaleOneTwo = (float)axisTwo.ScalarProduct(axisOne);

	Vector3 toStart = edgePoint1 - edgePoint2;
	float scaleOne = (float)axisOne.ScalarProduct(toStart);
	float scaleTwo = (float)axisTwo.ScalarProduct(toStart);

	float denominator = (squareMag1 * squareMag2) - (scaleOne * scaleTwo);

	//parallel edges
	if (abs(denominator) < 0.0001f)
	{
		return useOneMidpoint ? edgePoint1 : edgePoint2;
	}

	float edgeNearestA = ((scaleOneTwo * scaleTwo) - (squareMag2 * scaleOne)) / denominator;
	float edgeNearestB = ((squareMag1 * scaleTwo) - (scaleOneTwo * scaleOne)) / denominator;

	if (edgeNearestA > halfSize1 || edgeNearestA < -halfSize1
		|| edgeNearestB > halfSize2 || edgeNearestB < -halfSize2)
	{
		return useOneMidpoint ? edgePoint1 : edgePoint2;
	}
	else
	{
		contact1 = edgePoint1 + (axisOne * edgeNearestA);
		contact2 = edgePoint2 + (axisTwo * edgeNearestB);
		return (contact1 * 0.5) + (contact2 * 0.5);
	}
}
