#include "CollisionFine.h"
#include "Primitive.h"

void CollisionFine::DetectContacts(Primitive* prim1, Primitive* prim2)
{
	if (prim1->type == Primitive::Type::SPHERE && prim2->type == Primitive::Type::SPHERE)
	{
		SphereAndSphere(prim1, prim2,
			prim1->collisionVolume.centre, prim1->collisionVolume.radius,
			prim2->collisionVolume.centre, prim2->collisionVolume.radius);
	}
	if (prim1->type == Primitive::Type::SPHERE && prim2->type == Primitive::Type::PLANE)
	{
		SphereAndPlane(prim1, prim2,
			prim1->collisionVolume.centre, prim1->collisionVolume.radius,
			prim2->collisionVolume.centre, prim2->collisionVolume.normal);
	}
	if (prim1->type == Primitive::Type::PLANE && prim2->type == Primitive::Type::SPHERE)
	{
		SphereAndPlane(prim2, prim1,
			prim2->collisionVolume.centre, prim2->collisionVolume.radius,
			prim1->collisionVolume.centre, prim1->collisionVolume.normal);
	}
	if (prim1->type == Primitive::Type::BOX && prim2->type == Primitive::Type::PLANE)
	{
		BoxAndPlane(prim1, prim2, prim2->collisionVolume.centre, prim2->collisionVolume.normal);
	}
	if (prim1->type == Primitive::Type::PLANE && prim2->type == Primitive::Type::BOX)
	{
		BoxAndPlane(prim2, prim1, prim1->collisionVolume.centre, prim1->collisionVolume.normal);
	}
	if (prim1->type == Primitive::Type::SPHERE && prim2->type == Primitive::Type::BOX)
	{
		SphereAndBox(prim1, prim2, prim1->collisionVolume.centre, prim1->collisionVolume.radius);
	}
	if (prim1->type == Primitive::Type::BOX && prim2->type == Primitive::Type::SPHERE)
	{
		SphereAndBox(prim2, prim1, prim2->collisionVolume.centre, prim2->collisionVolume.radius);
	}
	if (prim1->type == Primitive::Type::BOX && prim2->type == Primitive::Type::BOX)
	{
		BoxAndBox(prim1, prim2);
	}
}

void CollisionFine::SphereAndSphere(Primitive* prim1, Primitive* prim2, const Vector3& position1, float radius1, const Vector3& position2, float radius2)
{
	//if (data->contactsLeft <= 0) return;

	Vector3 midline = position1 - position2;
	float size = (float)midline.Magnitude();

	if (size <= 0.0f || size >= radius1 + radius2) return;

	Vector3 normal = midline * 1.0f / size;

	Contact contact(prim1, prim2);
	contact.normal = normal;
	contact.point = position1 + midline * 0.5f;
	contact.penetrationDepth = radius1 + radius2 - size;

	/*data->*/contacts.push_back(contact);
	outputStr += "SPHERE and SPHERE \t";
}

void CollisionFine::SphereAndPlane(Primitive* sphere, Primitive* plane, const Vector3& spherePosition, float radius, const Vector3& planePosition, const Vector3& normal)
{
	//if (data->contactsLeft <= 0) return;
	
	float distance = normal.ScalarProduct(spherePosition) - radius - normal.ScalarProduct(planePosition);

	if (distance >= 0) return;

	Contact contact(sphere, plane);
	contact.normal = normal;
	contact.penetrationDepth = -distance;
	contact.point = spherePosition - normal * (distance + radius);

	/*data->*/contacts.push_back(contact);
	outputStr += "SPHERE and PLANE \t";
}

void CollisionFine::SphereAndBox(Primitive* sphere, Primitive* box, Vector3& spherePosition, float radius)
{
	Vector3 halfSize = box->collisionVolume.halfSize;
	Vector3 relCentre = Mathe::matrixInverse(box->collisionVolume.axisMat, spherePosition);

	double x = abs(relCentre.x) - radius;
	double y = abs(relCentre.y) - radius;
	double z = abs(relCentre.z) - radius;

	if (abs(relCentre.x) - radius > (float)halfSize.x
		|| abs(relCentre.y) - radius > (float)halfSize.y
		|| abs(relCentre.z) - radius > (float)halfSize.z)
		return; //not in contact

	Vector3 closestPoint;
	float distance;

	distance = relCentre.x;
	if (distance > halfSize.x) distance = (float)halfSize.x;
	if (distance < -halfSize.x) distance = (float)-halfSize.x;
	closestPoint.x = distance;

	distance = relCentre.y;
	if (distance > halfSize.y) distance = (float)halfSize.y;
	if (distance < -halfSize.y) distance = (float)-halfSize.y;
	closestPoint.y = distance;

	distance = relCentre.z;
	if (distance > halfSize.z) distance = (float)halfSize.z;
	if (distance < -halfSize.z) distance = (float)-halfSize.z;
	closestPoint.z = distance;

	distance = (closestPoint - relCentre).Magnitude();
	if (distance > radius* radius) return;	//not in contact
	
	Contact contact(sphere, box);
	contact.normal = (spherePosition - closestPoint).Normalise();
	contact.point = closestPoint;
	contact.penetrationDepth = radius - sqrt(distance);

	/*data->*/contacts.push_back(contact);	
	outputStr += "SPHERE and BOX \t";
}

float CollisionFine::PositionOnAxis(const Primitive* box, const Vector3& axis)
{
	return
		box->collisionVolume.halfSize.x * abs(axis.ScalarProduct(Mathe::GetAxis(0, box->collisionVolume.axisMat)))
		+ box->collisionVolume.halfSize.y * abs(axis.ScalarProduct(Mathe::GetAxis(1, box->collisionVolume.axisMat)))
		+ box->collisionVolume.halfSize.z * abs(axis.ScalarProduct(Mathe::GetAxis(2, box->collisionVolume.axisMat)));
}

bool CollisionFine::BoxesOverlapOnAxis(const Primitive* box1, const Primitive* box2, 
	const Vector3& toCentre, const Vector3& axis, 
	int index, float& smallestPenetration, int& smallestIndex)
{
	float box1AxisPos = PositionOnAxis(box1, axis);
	float box2AxisPos = PositionOnAxis(box2, axis);

	float distance = abs(toCentre.ScalarProduct(axis));// -data->tolerance;

	float penetration = box1AxisPos + box2AxisPos - distance;
	if (penetration < smallestPenetration)
	{
		smallestPenetration = penetration;
		smallestIndex = index;
	}

	return (distance <= box1AxisPos + box2AxisPos);
}

//Separating axis theorem
void CollisionFine::BoxAndBox(Primitive* box1, Primitive* box2)
{
	Vector3 toCentre = box2->collisionVolume.centre - box1->collisionVolume.centre;
	float smallestPenetration = 100;
	int smallestIndex = 0;

	//Early-outs if any axes are found to be not overlapping.
	if (!BoxesOverlapOnAxis(box1, box2, toCentre, Mathe::GetAxis(0, box1->collisionVolume.axisMat), 0, smallestPenetration, smallestIndex)) return;
	if (!BoxesOverlapOnAxis(box1, box2, toCentre, Mathe::GetAxis(1, box1->collisionVolume.axisMat), 1, smallestPenetration, smallestIndex)) return;
	if (!BoxesOverlapOnAxis(box1, box2, toCentre, Mathe::GetAxis(2, box1->collisionVolume.axisMat), 2, smallestPenetration, smallestIndex)) return;

	if (!BoxesOverlapOnAxis(box1, box2, toCentre, Mathe::GetAxis(0, box2->collisionVolume.axisMat), 3, smallestPenetration, smallestIndex)) return;
	if (!BoxesOverlapOnAxis(box1, box2, toCentre, Mathe::GetAxis(1, box2->collisionVolume.axisMat), 4, smallestPenetration, smallestIndex)) return;
	if (!BoxesOverlapOnAxis(box1, box2, toCentre, Mathe::GetAxis(2, box2->collisionVolume.axisMat), 5, smallestPenetration, smallestIndex)) return;

	if (!BoxesOverlapOnAxis(box1, box2, toCentre, Mathe::GetAxis(0, box1->collisionVolume.axisMat) % Mathe::GetAxis(0, box2->collisionVolume.axisMat), 6, smallestPenetration, smallestIndex)) return;
	if (!BoxesOverlapOnAxis(box1, box2, toCentre, Mathe::GetAxis(0, box1->collisionVolume.axisMat) % Mathe::GetAxis(1, box2->collisionVolume.axisMat), 7, smallestPenetration, smallestIndex)) return;
	if (!BoxesOverlapOnAxis(box1, box2, toCentre, Mathe::GetAxis(0, box1->collisionVolume.axisMat) % Mathe::GetAxis(2, box2->collisionVolume.axisMat), 8, smallestPenetration, smallestIndex)) return;
	if (!BoxesOverlapOnAxis(box1, box2, toCentre, Mathe::GetAxis(1, box1->collisionVolume.axisMat) % Mathe::GetAxis(0, box2->collisionVolume.axisMat), 9, smallestPenetration, smallestIndex)) return;
	if (!BoxesOverlapOnAxis(box1, box2, toCentre, Mathe::GetAxis(1, box1->collisionVolume.axisMat) % Mathe::GetAxis(1, box2->collisionVolume.axisMat), 10, smallestPenetration, smallestIndex)) return;
	if (!BoxesOverlapOnAxis(box1, box2, toCentre, Mathe::GetAxis(1, box1->collisionVolume.axisMat) % Mathe::GetAxis(2, box2->collisionVolume.axisMat), 11, smallestPenetration, smallestIndex)) return;
	if (!BoxesOverlapOnAxis(box1, box2, toCentre, Mathe::GetAxis(2, box1->collisionVolume.axisMat) % Mathe::GetAxis(0, box2->collisionVolume.axisMat), 12, smallestPenetration, smallestIndex)) return;
	if (!BoxesOverlapOnAxis(box1, box2, toCentre, Mathe::GetAxis(2, box1->collisionVolume.axisMat) % Mathe::GetAxis(1, box2->collisionVolume.axisMat), 13, smallestPenetration, smallestIndex)) return;
	if (!BoxesOverlapOnAxis(box1, box2, toCentre, Mathe::GetAxis(2, box1->collisionVolume.axisMat) % Mathe::GetAxis(2, box2->collisionVolume.axisMat), 14, smallestPenetration, smallestIndex)) return;

	//for (int n1 = 0; n1 < 3; n1++)
	//{
	//	//Test primitive axes.
	//	if (!BoxesOverlapOnAxis(box1, box2, toCentre, Mathe::GetAxis(n1, box1->collisionVolume.axisMat))
	//		|| !BoxesOverlapOnAxis(box1, box2, toCentre, Mathe::GetAxis(n1, box2->collisionVolume.axisMat)))
	//	{
	//		return; //Early-out
	//	}
	//	for (int n2 = 0; n2 < 3; n2++)
	//	{
	//		//Test cross products of primitive's axes.
	//		Vector3 vecProd = Mathe::GetAxis(n1, box1->collisionVolume.axisMat) % Mathe::GetAxis(n2, box2->collisionVolume.axisMat);
	//		if (!BoxesOverlapOnAxis(box1, box2, toCentre, vecProd))
	//		{
	//			return; //Early-out
	//		}
	//	}
	//}
	
	//OVERLAP DETECTED

	//Point-face collision
	if (smallestIndex < 3)
	{

	}
	else if (smallestIndex < 6)
	{

	}
	//Edge-edge collision
	else
	{
		smallestIndex -= 6;
		int box1AxisIndex = smallestIndex / 3;
		int box2AxisIndex = smallestIndex % 3;
		Vector3 box1Axis = Mathe::GetAxis(box1AxisIndex, box1->collisionVolume.axisMat);
		Vector3 box2Axis = Mathe::GetAxis(box2AxisIndex, box2->collisionVolume.axisMat);
		Vector3 axis = box1Axis % box2Axis;
		axis.Normalise();

		if (toCentre.ScalarProduct(axis) > 0) axis = axis * -1.0f;

		// We have the axes, but not the edges: each axis has 4 edges parallel
		// to it, we need to find which of the 4 for each object. We do
		// that by finding the point in the centre of the edge. We know
		// its component in the direction of the box's collision axis is zero
		// (its a mid-point) and we determine which of the extremes in each
		// of the other axes is closest.
		Vector3 ptOnOneEdge = box1->collisionVolume.halfSize;
		Vector3 ptOnTwoEdge = box2->collisionVolume.halfSize;
		for (int i = 0; i < 3; i++)
		{
			if (i == box1AxisIndex) ptOnOneEdge[i] = 0;
			else if (Mathe::GetAxis(i, box1->collisionVolume.axisMat) * axis > 0) ptOnOneEdge[i] = -ptOnOneEdge[i];

			if (i == box2AxisIndex) ptOnTwoEdge[i] = 0;
			else if (Mathe::GetAxis(i, box2->collisionVolume.axisMat) * axis < 0) ptOnTwoEdge[i] = -ptOnTwoEdge[i];
		}

		// Move them into world coordinates (they are already oriented
		// correctly, since they have been derived from the axes).
		ptOnOneEdge = box1->transform * ptOnOneEdge;
		ptOnTwoEdge = box2->transform * ptOnTwoEdge;

		// So we have a point and a direction for the colliding edges.
		// We need to find out point of closest approach of the two
		// line-segments.
		Vector3 vertex = contactPoint(
			ptOnOneEdge, box1Axis, one.halfSize[box1AxisIndex],
			ptOnTwoEdge, box2Axis, two.halfSize[box2AxisIndex],
			bestSingleAxis > 2
		);

		// We can fill the contact.
		Contact contact(box1, box2);

		contact.penetrationDepth = pen;
		contact.normal = axis;
		contact.point = vertex;
		contacts.push_back(contact);
	}

	//Debug
	outputStr += "BOX and BOX \t";
}

void CollisionFine::BoxAndPlane(Primitive* box, Primitive* plane, const Vector3& planePosition, const Vector3& normal)
{
	//Intersection test
	Vector3 boxCentre = box->collisionVolume.centre;
	int projectedRadius = (boxCentre.x * normal.x + boxCentre.y * normal.y + boxCentre.z * normal.z) * 10;
	//Work out how far the box is from the origin
	int boxDistance = ((normal * boxCentre).Magnitude() * 10) - projectedRadius;
	float planeOffset = planePosition.x * normal.x + planePosition.y * normal.y + planePosition.z * normal.z;

	if (((float)boxDistance / 10) > planeOffset) return;

	for (int v = 0; v < 8; v++)
	{
		//distance from vertex to plane
		float distance = (box->collisionVolume.vertices[v] * normal).Magnitude();

		if (distance <= planeOffset + /*data->*/tolerance)
		{
			Contact contact(box, plane);
			contact.point = normal;
			contact.point *= distance - (planePosition * normal).Magnitude();
			contact.point += box->collisionVolume.vertices[v];
			contact.normal = normal;
			contact.penetrationDepth = (planePosition * normal).Magnitude() - distance + /*data->*/tolerance;

			/*data->*/contacts.push_back(contact);
			outputStr += "BOX and PLANE \t";
		}
	}
}
