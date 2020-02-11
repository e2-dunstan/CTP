#include "CollisionFine.h"
#include "Primitive.h"

void CollisionFine::DetectContacts(Primitive* prim1, Primitive* prim2)
{
	if (prim1->type == Primitive::Type::PLANE && prim2->type == Primitive::Type::SPHERE)
	{
		SphereAndPlane(prim2, prim1,
			prim2->collisionVolume.centre, prim2->collisionVolume.radius,
			prim1->collisionVolume.centre, prim1->collisionVolume.normal);
	}
	else if (prim1->type == Primitive::Type::PLANE && prim2->type == Primitive::Type::BOX)
	{
		BoxAndPlane(prim2, prim1, prim1->collisionVolume.centre, prim1->collisionVolume.normal);
	}
	else if (prim1->type == Primitive::Type::PLANE && prim2->type == Primitive::Type::CYLINDER)
	{
		CylinderAndPlane(prim2, prim1);
	}
	else if (prim1->type == Primitive::Type::PLANE && prim2->type == Primitive::Type::CAPSULE)
	{
		CapsuleAndPlane(prim2, prim1);
	}
	else if (prim1->type == Primitive::Type::SPHERE && prim2->type == Primitive::Type::SPHERE)
	{
		SphereAndSphere(prim1, prim2,
			prim1->collisionVolume.centre, prim1->collisionVolume.radius,
			prim2->collisionVolume.centre, prim2->collisionVolume.radius);
	}
	//else if (prim1->type == Primitive::Type::SPHERE && prim2->type == Primitive::Type::PLANE)
	//{
	//	SphereAndPlane(prim1, prim2,
	//		prim1->collisionVolume.centre, prim1->collisionVolume.radius,
	//		prim2->collisionVolume.centre, prim2->collisionVolume.normal);
	//}
	else if (prim1->type == Primitive::Type::SPHERE && prim2->type == Primitive::Type::BOX)
	{
		SphereAndBox(prim1, prim2, prim1->collisionVolume.centre, prim1->collisionVolume.radius);
	}
	else if (prim1->type == Primitive::Type::SPHERE && prim2->type == Primitive::Type::CYLINDER)
	{
		CylinderAndSphere(prim2, prim1);
	}
	else if (prim1->type == Primitive::Type::SPHERE && prim2->type == Primitive::Type::CAPSULE)
	{
		CapsuleAndSphere(prim2, prim1);
	}
	//else if (prim1->type == Primitive::Type::BOX && prim2->type == Primitive::Type::PLANE)
	//{
	//	BoxAndPlane(prim1, prim2, prim2->collisionVolume.centre, prim2->collisionVolume.normal);
	//}
	else if (prim1->type == Primitive::Type::BOX && prim2->type == Primitive::Type::SPHERE)
	{
		SphereAndBox(prim2, prim1, prim2->collisionVolume.centre, prim2->collisionVolume.radius);
	}
	//else if (prim1->type == Primitive::Type::BOX && prim2->type == Primitive::Type::BOX)
	//{
		//BoxAndBox(prim1, prim2);
	//}
	else if (prim1->type == Primitive::Type::BOX && prim2->type == Primitive::Type::CYLINDER)
	{
		//CylinderAndBox(prim2, prim1);
	}
	else if (prim1->type == Primitive::Type::BOX && prim2->type == Primitive::Type::CAPSULE)
	{

	}
	else if (prim1->type == Primitive::Type::CYLINDER && prim2->type == Primitive::Type::CYLINDER)
	{
		//CylinderAndCylinder(prim1, prim2);
	}
	//else if (prim1->type == Primitive::Type::CYLINDER && prim2->type == Primitive::Type::PLANE) 
	//{
	//	CylinderAndPlane(prim1, prim2);
	//}
	else if (prim1->type == Primitive::Type::CYLINDER && prim2->type == Primitive::Type::SPHERE)
	{
		CylinderAndSphere(prim1, prim2);
	}
	else if (prim1->type == Primitive::Type::CYLINDER && prim2->type == Primitive::Type::BOX)
	{
		//CylinderAndBox(prim1, prim2);
	}
	else if (prim1->type == Primitive::Type::CYLINDER && prim2->type == Primitive::Type::CAPSULE)
	{
		
	}
	else if (prim1->type == Primitive::Type::CAPSULE && prim2->type == Primitive::Type::CAPSULE)
	{

	}
	else if (prim1->type == Primitive::Type::CAPSULE && prim2->type == Primitive::Type::CYLINDER)
	{
		
	}
	//else if (prim1->type == Primitive::Type::CAPSULE && prim2->type == Primitive::Type::PLANE)
	//{
	//	CapsuleAndPlane(prim1, prim2);
	//}
	else if (prim1->type == Primitive::Type::CAPSULE && prim2->type == Primitive::Type::BOX)
	{

	}
	else if (prim1->type == Primitive::Type::CAPSULE && prim2->type == Primitive::Type::SPHERE)
	{
		CapsuleAndSphere(prim1, prim2);
	}
}

void CollisionFine::SphereAndSphere(Primitive* prim1, Primitive* prim2, const Vector3& position1, float radius1, const Vector3& position2, float radius2)
{
	Vector3 midline = position1 - position2;
	float size = (float)midline.Magnitude();

	if (size <= 0.0f || size >= radius1 + radius2) return;

	Vector3 normal = midline * 1.0f / size;

	Contact contact(prim1, prim2);
	contact.normal = normal;
	contact.point = position1 + midline * 0.5f;
	contact.penetrationDepth = radius1 + radius2 - size;
	contacts.push_back(contact);
}

void CollisionFine::SphereAndPlane(Primitive* sphere, Primitive* plane, const Vector3& spherePosition, float radius, const Vector3& planePosition, const Vector3& normal)
{
	float distance = (float)normal.ScalarProduct(spherePosition) - radius - (float)normal.ScalarProduct(planePosition);

	if (distance >= 0) return;

	Contact contact(sphere, plane);
	contact.normal = normal;
	contact.penetrationDepth = -distance;
	contact.point = spherePosition - normal * (distance + radius);
	contacts.push_back(contact);
}

void CollisionFine::SphereAndBox(Primitive* sphere, Primitive* box, Vector3& spherePosition, float radius)
{
	Vector3 halfSize = box->collisionVolume.halfSize;
	Vector3 relCentre = Mathe::MatrixInverse(box->collisionVolume.axisMat, spherePosition);

	if (abs(relCentre.x) - radius > (float)halfSize.x
		|| abs(relCentre.y) - radius > (float)halfSize.y
		|| abs(relCentre.z) - radius > (float)halfSize.z)
		return; //not in contact

	Vector3 closestPoint;
	double distance;

	for (unsigned i = 0; i < 3; i++)
	{
		distance = relCentre[i];
		if (distance > halfSize[i]) distance = (float)halfSize[i];
		if (distance < -halfSize[i]) distance = (float)-halfSize[i];
		closestPoint[i] = distance;
	}

	distance = (closestPoint - relCentre).Magnitude();
	if (distance > (double)radius * (double)radius) return;	//not in contact
	
	Contact contact(sphere, box);
	contact.normal = (spherePosition - closestPoint).Normalise();
	contact.point = closestPoint;
	contact.penetrationDepth = radius - (float)sqrt(distance);
	contacts.push_back(contact);	
}

double CollisionFine::PositionOnAxis(const Primitive* box, const Vector3& axis)
{
	return
		box->collisionVolume.halfSize.x * abs(axis.ScalarProduct(Mathe::GetAxis(0, box->collisionVolume.axisMat)))
		+ box->collisionVolume.halfSize.y * abs(axis.ScalarProduct(Mathe::GetAxis(1, box->collisionVolume.axisMat)))
		+ box->collisionVolume.halfSize.z * abs(axis.ScalarProduct(Mathe::GetAxis(2, box->collisionVolume.axisMat)));
}

//bool CollisionFine::BoxesOverlapOnAxis(const Primitive* box1, const Primitive* box2, 
//	const Vector3& toCentre, Vector3 axis, 
//	int index, float& smallestPenetration, int& smallestIndex)
//{
//	if (axis.SquaredMagnitude() < 0.0001) return true;
//	axis = axis.Normalise();
//
//	float box1AxisPos = (float)PositionOnAxis(box1, axis) / 2.0f;
//	float box2AxisPos = (float)PositionOnAxis(box2, axis) / 2.0f;
//
//	float distance = (float)abs(toCentre.ScalarProduct(axis));// -tolerance;
//
//	float penetration = abs(box1AxisPos + box2AxisPos - distance);// +tolerance;
//	if (penetration < 0) return false;
//	if (penetration < smallestPenetration)
//	{
//		smallestPenetration = penetration;
//		smallestIndex = index;
//	}
//
//	return true;
//}

//Separating axis theorem
//void CollisionFine::BoxAndBox(Primitive* box1, Primitive* box2)
//{
//	sat->Test(box1, box2);
	//Contact* satContacts[4] = { sat->Test(contacts, box1, box2) };
	//for (unsigned c = 0; c < 4; c++)
	//{
	//	if ((satContacts[c]) != nullptr)
	//	{
	//		contacts.push_back(*(satContacts[c]));
	//	}
	//}
	//if (contact != nullptr) contacts.push_back(*contact);
//
	/*
	Vector3 toCentre = box2->collisionVolume.centre - box1->collisionVolume.centre;
	float smallestPenetration = 1000;
	int smallestIndex = 0;

	//Early-outs if any axes are found to be not overlapping.
	if (!BoxesOverlapOnAxis(box1, box2, toCentre, Mathe::GetAxis(0, box1->collisionVolume.axisMat), 0, smallestPenetration, smallestIndex)) return;
	if (!BoxesOverlapOnAxis(box1, box2, toCentre, Mathe::GetAxis(1, box1->collisionVolume.axisMat), 1, smallestPenetration, smallestIndex)) return;
	if (!BoxesOverlapOnAxis(box1, box2, toCentre, Mathe::GetAxis(2, box1->collisionVolume.axisMat), 2, smallestPenetration, smallestIndex)) return;

	if (!BoxesOverlapOnAxis(box1, box2, toCentre, Mathe::GetAxis(0, box2->collisionVolume.axisMat), 3, smallestPenetration, smallestIndex)) return;
	if (!BoxesOverlapOnAxis(box1, box2, toCentre, Mathe::GetAxis(1, box2->collisionVolume.axisMat), 4, smallestPenetration, smallestIndex)) return;
	if (!BoxesOverlapOnAxis(box1, box2, toCentre, Mathe::GetAxis(2, box2->collisionVolume.axisMat), 5, smallestPenetration, smallestIndex)) return;

	int singleSmallestIndex = smallestIndex;

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
		PointFaceCollision(box1, box2, toCentre, smallestIndex, smallestPenetration);
		//std::cout << "POINT-FACE" << std::endl;
		return;
	}
	else if (smallestIndex < 6)
	{
		PointFaceCollision(box2, box1, toCentre * -1.0f, smallestIndex - 3, smallestPenetration);
		//std::cout << "POINT-FACE" << std::endl;
		return;
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

		Vector3 ptOnOneEdge = box1->collisionVolume.halfSize;
		Vector3 ptOnTwoEdge = box2->collisionVolume.halfSize;

		for (int i = 0; i < 3; i++)
		{
			if (box1AxisIndex == i) ptOnOneEdge[i] = 0;
			else if (Mathe::GetAxis(i, box1->collisionVolume.axisMat).ScalarProduct(axis) > 0) ptOnOneEdge[i] = -ptOnOneEdge[i];
			if (box2AxisIndex == i) ptOnTwoEdge[i] = 0;
			else if (Mathe::GetAxis(i, box2->collisionVolume.axisMat).ScalarProduct(axis) < 0) ptOnTwoEdge[i] = -ptOnTwoEdge[i];
		}

		Mathe::Transform(ptOnOneEdge, box1->transform);
		Mathe::Transform(ptOnTwoEdge, box2->transform);

		Vector3 vertex = GetEdgeContactPoint(ptOnOneEdge, ptOnTwoEdge, box1Axis, box2Axis, 
			(float)box1->collisionVolume.halfSize[box1AxisIndex], (float)box2->collisionVolume.halfSize[box2AxisIndex],
			singleSmallestIndex > 2);

		Contact contact(box1, box2);
		contact.penetrationDepth = smallestPenetration;
		contact.normal = axis;
		contact.point = vertex;
		contacts.push_back(contact);

		return;
	}

	//Debug
	outputStr += "BOX and BOX \t";
}*/

//void CollisionFine::PointFaceCollision(Primitive* box1, Primitive* box2, const Vector3& toCentre, int smallest, float penetration)
//{
//	Contact contact(box1, box2);
//
//	Vector3 normal = Mathe::GetAxis(smallest, box1->collisionVolume.axisMat);
//	if (normal.ScalarProduct(toCentre) > 0)
//	{
//		normal *= -1.0f;
//	}
//
//	Vector3 vertex = box2->collisionVolume.halfSize;
//	if (Mathe::GetAxis(0, box2->collisionVolume.axisMat).ScalarProduct(normal) < 0) vertex.x = -vertex.x;
//	if (Mathe::GetAxis(1, box2->collisionVolume.axisMat).ScalarProduct(normal) < 0) vertex.y = -vertex.y;
//	if (Mathe::GetAxis(2, box2->collisionVolume.axisMat).ScalarProduct(normal) < 0) vertex.z = -vertex.z;
//
//	contact.normal = normal;
//	contact.penetrationDepth = penetration;
//	Mathe::Transform(vertex, box2->collisionVolume.axisMat);
//	contact.point = vertex;
//	contacts.push_back(contact);
//}
//
//Vector3 CollisionFine::GetEdgeContactPoint(const Vector3& edgePoint1, const Vector3& edgePoint2, Vector3& axisOne, Vector3& axisTwo, float halfSize1, float halfSize2, bool useOneMidpoint)
//{
//	Vector3 contact1, contact2;
//
//	float squareMag1 = (float)axisOne.SquaredMagnitude();
//	float squareMag2 = (float)axisTwo.SquaredMagnitude();
//	float dotProdAxisOneTwo = (float)axisTwo.ScalarProduct(axisOne);
//
//	Vector3 toStart = edgePoint1 - edgePoint2;
//	float dotProduct1 = (float)axisOne.ScalarProduct(toStart);
//	float dotProduct2 = (float)axisTwo.ScalarProduct(toStart);
//
//	float denominator = (squareMag1 * squareMag2) - (dotProduct1 * dotProduct2);
//
//	//parallel edges
//	if (abs(denominator) < 0.0001f)
//	{
//		return useOneMidpoint ? edgePoint1 : edgePoint2;
//	}
//
//	float edgeNearestA = ((dotProdAxisOneTwo * dotProduct2) - (squareMag2 * dotProduct1)) / denominator;
//	float edgeNearestB = ((squareMag1 * dotProduct2) - (dotProdAxisOneTwo * dotProduct1)) / denominator;
//
//	if (edgeNearestA > halfSize1 || edgeNearestA < -halfSize1
//		|| edgeNearestB > halfSize2 || edgeNearestB < -halfSize2)
//	{
//		return useOneMidpoint ? edgePoint1 : edgePoint2;
//	}
//	else
//	{
//		contact1 = edgePoint1 + (axisOne * edgeNearestA);
//		contact2 = edgePoint2 + (axisTwo * edgeNearestB);
//		return (contact1 * 0.5) + (contact2 * 0.5);
//	}
//}

void CollisionFine::BoxAndPlane(Primitive* box, Primitive* plane, const Vector3& planePosition, const Vector3& normal)
{
	float boxDistance = (float)(normal * box->collisionVolume.centre).Magnitude() - PositionOnAxis(box, normal);
	float planeOffset = (float)planePosition.ScalarProduct(normal);

	if (boxDistance > planeOffset) return;

	std::vector<Contact> boxPlaneContacts;
	//Contact* boxPlaneContacts[4] = { nullptr };
	unsigned numContacts = 0;

	for (unsigned v = 0; v < 8; v++)
	{
		//distance from vertex to plane
		float distance = (float)box->collisionVolume.vertices[v].ScalarProduct(normal);

		if (distance <= planeOffset)
		{
			Contact contact(box, plane);
			contact.point = normal;
			contact.point *= distance - (float)(planePosition * normal).Magnitude();
			contact.point += box->collisionVolume.vertices[v];
			contact.normal = normal;
			contact.penetrationDepth = (float)abs((planePosition * normal).Magnitude() - distance);
			boxPlaneContacts.push_back(contact);
			//boxPlaneContacts[numContacts] = contact;
			//contacts.push_back(contact);

			numContacts++;
		}
	}
	if (numContacts < 4)
	{
		box->rigidbody.SetAwake(true);
	}

	//Merge plane contact points for more realistic resolution
	//unsigned vecSize = boxPlaneContacts.size();

	if (numContacts <= 0) return;
	else if (numContacts == 1)
	{
		contacts.push_back(boxPlaneContacts[0]);
		return;
	}

	Contact mergedContact(box, plane);
	mergedContact.normal = normal;

	for (unsigned i = 0; i < numContacts; i++)
	{
		mergedContact.point += boxPlaneContacts[i].point;
		mergedContact.penetrationDepth += boxPlaneContacts[i].penetrationDepth;
	}
	mergedContact.point /= numContacts;
	mergedContact.penetrationDepth /= numContacts;
	contacts.push_back(mergedContact);

	/*if (vecSize == 0) return;
	if (vecSize == 1)
	{
		contacts.push_back(boxPlaneContacts[0]);
		return;
	}
	if (vecSize > 1)
	{
		Contact contact(box, plane);
		contact.normal = normal;
		
		for (unsigned c = 0; c < vecSize; c++)
		{
			contact.point += boxPlaneContacts[c].point;
			contact.penetrationDepth += boxPlaneContacts[c].penetrationDepth;
		}
		contact.point /= vecSize;
		contact.penetrationDepth /= vecSize;
		contacts.push_back(contact);
		return;
	}*/
}

void CollisionFine::CylinderAndPlane(Primitive* cyl, Primitive* plane)
{
	//Top and bottom centre points
	Vector3 cylEndT = cyl->collisionVolume.centre + (cyl->upDir * cyl->collisionVolume.length);
	Vector3 cylEndB = cyl->collisionVolume.centre - (cyl->upDir * cyl->collisionVolume.length);

	float planePosition = (float)plane->collisionVolume.centre.ScalarProduct(plane->collisionVolume.normal);

	Vector3 cylToPlane = (plane->collisionVolume.normal.VectorProduct(cyl->upDir)).VectorProduct(cyl->upDir);

	Vector3 points[] = {
		cylEndT + (cylToPlane * cyl->radius),
		cylEndT - (cylToPlane * cyl->radius),
		cylEndB + (cylToPlane * cyl->radius),
		cylEndB - (cylToPlane * cyl->radius)
	};
	float scalarProducts[] ={
		(float)points[0].ScalarProduct(plane->collisionVolume.normal),
		(float)points[1].ScalarProduct(plane->collisionVolume.normal),
		(float)points[2].ScalarProduct(plane->collisionVolume.normal),
		(float)points[3].ScalarProduct(plane->collisionVolume.normal)
	};

	if (scalarProducts[0] < planePosition
		|| scalarProducts[1] < planePosition
		|| scalarProducts[2] < planePosition
		|| scalarProducts[3] < planePosition)
	{
		//COLLIDING

		float dirScalarProduct = (float)plane->collisionVolume.normal.ScalarProduct(cyl->upDir);

		//STANDING LIKE A COLUMN
		if (abs(dirScalarProduct) < 1.01f && abs(dirScalarProduct) > 0.99f)	
		{
			//Three contacts required, each 120 degrees away from eachother about the centre
			//Arbritrary points calculated
			//THIS WILL INTRODUCE PROBLEMS IF THE PLANE'S NORMAL IS EVER NOT 0,1,0
			Vector3 centre(cyl->collisionVolume.centre.x, planePosition, cyl->collisionVolume.centre.z);
			float PI = 3.14159265359f;

			Contact contact(cyl, plane);
			contact.penetrationDepth = 0.00000001f;
			contact.normal = plane->collisionVolume.normal;

			contact.point = centre + Vector3((double)cyl->radius * (double)cos(0), 0, (double)cyl->radius * (double)sin(0));
			contacts.push_back(contact);

			contact.point = centre + Vector3((double)cyl->radius * (double)cos((2.0f * PI) / 3.0f), 0, (double)cyl->radius * (double)sin((2.0f * PI) / 3.0f));
			contacts.push_back(contact);

			contact.point = centre + Vector3((double)cyl->radius * (double)cos(-(2.0f * PI) / 3.0f), 0, (double)cyl->radius * (double)sin(-(2.0f * PI) / 3.0f));
			contacts.push_back(contact);

			return;
		}
		else
		{
			//FIRST CONTACT
			float smallestDistance1 = 1000;
			int closestIndex1 = 0;

			for (int i = 0; i < 4; i++)
			{
				if ((float)abs(scalarProducts[i] - plane->collisionVolume.normal.Magnitude()) < smallestDistance1)
				{
					smallestDistance1 = (float)abs(scalarProducts[i] - plane->collisionVolume.normal.Magnitude());
					closestIndex1 = i;
				}
			}
			Contact contact(cyl, plane);
			contact.point = points[closestIndex1];
			contact.penetrationDepth = smallestDistance1;
			contact.normal = plane->collisionVolume.normal;
			contacts.push_back(contact);


			//PARALLEL (rolling)
			if (abs(dirScalarProduct) < 0.01f)
			{
				float smallestDistance2 = 1000;
				int closestIndex2 = 0;

				for (int i = 0; i < 4; i++)
				{
					if (abs(scalarProducts[i] - plane->collisionVolume.normal.Magnitude()) < smallestDistance2
						&& i != closestIndex1)
					{
						smallestDistance2 = (float)abs(scalarProducts[i] - plane->collisionVolume.normal.Magnitude());
						closestIndex2 = i;
					}
				}
				Contact contact(cyl, plane);
				contact.point = points[closestIndex2];
				contact.penetrationDepth = smallestDistance2;
				contact.normal = plane->collisionVolume.normal;
				contacts.push_back(contact);
			}

		}
	}
	else
	{
		return;	//No contact
	}
}

void CollisionFine::CylinderAndSphere(Primitive* cyl, Primitive* sphere)
{
	//Convert to cylinder basis (transform)
	Vector3 relativeSpherePos = (sphere->collisionVolume.centre - cyl->collisionVolume.centre);// .Normalise();
	Matrix cylTranspose = cyl->collisionVolume.axisMat.Transpose();
	Mathe::Transform(relativeSpherePos, cylTranspose);

	//NOT COLLIDING - actually calculated in AABB stage so sort of unnecessary here
	if ((relativeSpherePos.y - sphere->collisionVolume.radius) > (cyl->collisionVolume.length / 2)
		|| (relativeSpherePos.y + sphere->collisionVolume.radius) < -(cyl->collisionVolume.length / 2))
	{
		return;
	}

	//Y is along the centre line of the cyl therefore null
	float pow1 = (float)pow(relativeSpherePos.x, 2);
	float pow2 = (float)pow(relativeSpherePos.z, 2);
	float distanceToCentreLine = pow1 + pow2;

	//Could be colliding
	if (distanceToCentreLine < pow(cyl->collisionVolume.radius + sphere->collisionVolume.radius, 2))
	{
		//COLLIDING ON CURVED FACE, therefore treat cylinder as a sphere
		if (relativeSpherePos.y < (cyl->collisionVolume.length / 2)
			&& relativeSpherePos.y > -(cyl->collisionVolume.length / 2))
		{
			Vector3 cylSphereRepresentation(0, relativeSpherePos.y, 0);
			Mathe::Transform(cylSphereRepresentation, cyl->collisionVolume.axisMat);
			SphereAndSphere(cyl, sphere, cylSphereRepresentation, cyl->collisionVolume.radius, sphere->collisionVolume.centre, sphere->collisionVolume.radius);
			return;
		}
		//COLLIDING ON FLAT PLANES
		else if ((relativeSpherePos.y - sphere->collisionVolume.radius) < (cyl->collisionVolume.length / 2)
			&& (relativeSpherePos.y + sphere->collisionVolume.radius) > -(cyl->collisionVolume.length / 2))
		{
			double closestEndpoint = (abs(relativeSpherePos.y - cyl->collisionVolume.length) >
				abs(relativeSpherePos.y + cyl->collisionVolume.length))
				? cyl->collisionVolume.length
				: -cyl->collisionVolume.length;

			//if sphere x and z magnitude is greater than cyl radius,
			//it is a edge collision, else it's colliding with the face

			Vector3 contactPoint = Vector3(relativeSpherePos.x, closestEndpoint, relativeSpherePos.z);

			if (distanceToCentreLine > pow(cyl->collisionVolume.radius, 2))
			{
				contactPoint = contactPoint.Normalise() * cyl->collisionVolume.radius;
				Mathe::Transform(contactPoint, cyl->collisionVolume.axisMat); //back to world space
				Vector3 normal = (sphere->collisionVolume.centre - contactPoint).Normalise();

				Contact contact(cyl, sphere);
				contact.point = contactPoint;
				contact.normal = normal;
				contact.penetrationDepth = 0;
				contacts.push_back(contact);
				return;
			}
			else
			{
				Mathe::Transform(contactPoint, cyl->collisionVolume.axisMat); //back to world space

				Contact contact(cyl, sphere);
				contact.point = contactPoint;
				contact.normal = cyl->upDir;
				contact.penetrationDepth = 0;
				contacts.push_back(contact);
				return;
			}
		}
	}
	//NOT COLLIDING
	return;
}

void CollisionFine::CapsuleAndPlane(Primitive* cap, Primitive* plane)
{
	//VERY similar to cylinder plane, but checks for end points also necessary

	//Top and bottom centre points
	Vector3 capEndT = cap->collisionVolume.centre + (cap->upDir * cap->collisionVolume.length);
	Vector3 capEndB = cap->collisionVolume.centre - (cap->upDir * cap->collisionVolume.length);

	float planePosition = (float)plane->collisionVolume.centre.ScalarProduct(plane->collisionVolume.normal);

	Vector3 capToPlane = (plane->collisionVolume.normal.VectorProduct(cap->upDir)).VectorProduct(cap->upDir);

	Vector3 points[] = {
		capEndT + (capToPlane * cap->radius),
		capEndT - (capToPlane * cap->radius),
		capEndB + (capToPlane * cap->radius),
		capEndB - (capToPlane * cap->radius),
		capEndT + (cap->upDir * cap->radius), 
		capEndB - (cap->upDir * cap->radius)
	};
	float scalarProducts[] = {
		(float)points[0].ScalarProduct(plane->collisionVolume.normal),
		(float)points[1].ScalarProduct(plane->collisionVolume.normal),
		(float)points[2].ScalarProduct(plane->collisionVolume.normal),
		(float)points[3].ScalarProduct(plane->collisionVolume.normal),
		(float)points[4].ScalarProduct(plane->collisionVolume.normal),
		(float)points[5].ScalarProduct(plane->collisionVolume.normal)
	};

	if (scalarProducts[0] < planePosition
		|| scalarProducts[1] < planePosition
		|| scalarProducts[2] < planePosition
		|| scalarProducts[3] < planePosition
		|| scalarProducts[4] < planePosition
		|| scalarProducts[5] < planePosition)
	{
		//COLLIDING

		float dirScalarProduct = (float)plane->collisionVolume.normal.ScalarProduct(cap->upDir);

		//STANDING LIKE A COLUMN
		if (abs(dirScalarProduct) < 1.01f && abs(dirScalarProduct) > 0.99f)
		{
			//One contact point on a half-sphere
			Vector3 centre(cap->collisionVolume.centre.x, planePosition, cap->collisionVolume.centre.z);

			Contact contact(cap, plane);
			contact.penetrationDepth = 0.00000001f;
			contact.normal = plane->collisionVolume.normal;
			contact.point = centre;
			contacts.push_back(contact);

			return;
		}
		//PARALLEL THEREFORE ROLLING
		else if (abs(dirScalarProduct) < 0.01f)
		{
			float smallestDistance1 = 1000;
			int closestIndex1 = 0;

			for (int i = 0; i < 4; i++)
			{
				if ((float)abs(scalarProducts[i] - plane->collisionVolume.normal.Magnitude()) < smallestDistance1)
				{
					smallestDistance1 = (float)abs(scalarProducts[i] - plane->collisionVolume.normal.Magnitude());
					closestIndex1 = i;
				}
			}
			Contact contact(cap, plane);
			contact.point = points[closestIndex1];
			contact.penetrationDepth = smallestDistance1;
			contact.normal = plane->collisionVolume.normal;
			contacts.push_back(contact);

			float smallestDistance2 = 1000;
			int closestIndex2 = 0;

			for (int i = 0; i < 4; i++)
			{
				if ((float)abs(scalarProducts[i] - plane->collisionVolume.normal.Magnitude()) < smallestDistance2
					&& i != closestIndex1)
				{
					smallestDistance2 = (float)abs(scalarProducts[i] - plane->collisionVolume.normal.Magnitude());
					closestIndex2 = i;
				}
			}
			contact.point = points[closestIndex2];
			contact.penetrationDepth = smallestDistance2;
			contacts.push_back(contact);
		}
		else
		{
			Vector3 closestPoint = (abs(scalarProducts[5] - planePosition) > abs(scalarProducts[4] - planePosition))
								 ? capEndT : capEndB;
			closestPoint += capToPlane * cap->radius;
			closestPoint.y = planePosition;
			Contact contact(cap, plane);
			contact.point = closestPoint;
			contact.normal = plane->collisionVolume.normal;
			contact.penetrationDepth = 0.000001f;
			contacts.push_back(contact);
		}
	}
	else
	{
		return;	//No contact
	}
}

void CollisionFine::CapsuleAndSphere(Primitive* cap, Primitive* sphere)
{
	//Convert to capsule basis (transform)
	Vector3 relativeSpherePos = (sphere->collisionVolume.centre - cap->collisionVolume.centre);
	Matrix capTranspose = cap->collisionVolume.axisMat.Transpose();
	Mathe::Transform(relativeSpherePos, capTranspose);

	//NOT COLLIDING - actually calculated in AABB stage so sort of unnecessary here
	//if (((float)relativeSpherePos.y - sphere->collisionVolume.radius) > (cap->collisionVolume.length / 2.0f) + cap->radius
	//	|| ((float)relativeSpherePos.y + sphere->collisionVolume.radius) < -(cap->collisionVolume.length / 2.0f) - cap->radius)
	//{
	//	return;
	//}

	//Y is along the centre line of the cap therefore null
	float pow1 = (float)pow(relativeSpherePos.x, 2);
	float pow2 = (float)pow(relativeSpherePos.z, 2);
	float distanceToCentreLine = pow1 + pow2;

	//Could be colliding
	if (distanceToCentreLine < pow(cap->collisionVolume.radius + cap->collisionVolume.radius + sphere->collisionVolume.radius, 2))
	{
		//COLLIDING ON CURVED FACE, therefore treat cylinder as a sphere
		if (relativeSpherePos.z < (cap->collisionVolume.length / 2)
			&& relativeSpherePos.z > -(cap->collisionVolume.length / 2))
		{
			Vector3 capSphereRepresentation(0, relativeSpherePos.y, 0);
			Mathe::Transform(capSphereRepresentation, cap->collisionVolume.axisMat);
			SphereAndSphere(cap, sphere, 
				capSphereRepresentation, cap->collisionVolume.radius, 
				sphere->collisionVolume.centre, sphere->collisionVolume.radius);
			return;
		}
		//COLLIDING ON DOME
		else
		{
			double closestEndpoint = (abs(relativeSpherePos.y - cap->collisionVolume.length) >
				abs(relativeSpherePos.y + cap->collisionVolume.length))
				? cap->collisionVolume.length / 2
				: -cap->collisionVolume.length / 2;

			Vector3 capSphereOrigin = Vector3(relativeSpherePos.x, closestEndpoint, relativeSpherePos.z);
			SphereAndSphere(cap, sphere, 
				capSphereOrigin, cap->collisionVolume.radius, 
				sphere->collisionVolume.centre, sphere->collisionVolume.radius);
		}
	}
	else
	{
		//NOT COLLIDING
		return;
	}
}
