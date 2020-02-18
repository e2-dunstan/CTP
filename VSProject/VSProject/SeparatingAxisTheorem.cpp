#include "SeparatingAxisTheorem.h"
#include "Primitive.h"
#include "CollisionData.h"

void SAT::Test(Primitive* box1, Primitive* box2)
{
	Vector3 toCentre = box2->collisionVolume.centre - box1->collisionVolume.centre;
	float smallestPenetration = 1000;
	int smallestIndex = -1;

	CollisionVolume* cv1 = &box1->collisionVolume;
	CollisionVolume* cv2 = &box2->collisionVolume;

	//Early-outs if any axes are found to be not overlapping.
	//FACES
	//Box1 axes
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(0, cv1->axisMat), 0, smallestPenetration, smallestIndex)) return; //1x
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(1, cv1->axisMat), 1, smallestPenetration, smallestIndex)) return; //1y
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(2, cv1->axisMat), 2, smallestPenetration, smallestIndex)) return; //1z
	//Box2 axes																													 
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(0, cv2->axisMat), 3, smallestPenetration, smallestIndex)) return; //2x
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(1, cv2->axisMat), 4, smallestPenetration, smallestIndex)) return; //2y
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(2, cv2->axisMat), 5, smallestPenetration, smallestIndex)) return; //2z

	int singleSmallestIndex = smallestIndex;
	//EDGES
	//Cross product axes
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(0, cv1->axisMat).VectorProduct(Mathe::GetAxis(0, cv2->axisMat)), 6, smallestPenetration, smallestIndex)) return; // 1x 2x
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(0, cv1->axisMat).VectorProduct(Mathe::GetAxis(1, cv2->axisMat)), 7, smallestPenetration, smallestIndex)) return; // 1x 2y
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(0, cv1->axisMat).VectorProduct(Mathe::GetAxis(2, cv2->axisMat)), 8, smallestPenetration, smallestIndex)) return; // 1x 2z
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(1, cv1->axisMat).VectorProduct(Mathe::GetAxis(0, cv2->axisMat)), 9, smallestPenetration, smallestIndex)) return; // 1y 2x
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(1, cv1->axisMat).VectorProduct(Mathe::GetAxis(1, cv2->axisMat)), 10, smallestPenetration, smallestIndex)) return; // 1y 2y
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(1, cv1->axisMat).VectorProduct(Mathe::GetAxis(2, cv2->axisMat)), 11, smallestPenetration, smallestIndex)) return; // 1y 2z
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(2, cv1->axisMat).VectorProduct(Mathe::GetAxis(0, cv2->axisMat)), 12, smallestPenetration, smallestIndex)) return; // 1z 2x
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(2, cv1->axisMat).VectorProduct(Mathe::GetAxis(1, cv2->axisMat)), 13, smallestPenetration, smallestIndex)) return; // 1z 2y
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(2, cv1->axisMat).VectorProduct(Mathe::GetAxis(2, cv2->axisMat)), 14, smallestPenetration, smallestIndex)) return; // 1z 2z

	GetContactData(smallestIndex, box1, box2, toCentre, smallestPenetration, singleSmallestIndex);


	//CalculateSmallestAxesAndPenetrations(smallestPenetration);
	
	//for (unsigned i = 0; i < penetrations.size(); i++)
	//{
	//	GetContactData(minimumAxes[i], box1, box2, toCentre, penetrations[i], singleSmallestIndex);
	//}
	//
	//Contact contact(box1, box2);
	//for (unsigned i = 0; i < tempContacts.size(); i++)
	//{
	//	contact.point += tempContacts[i].point;
	//	contact.penetrationDepth += tempContacts[i].penetrationDepth;
	//	contact.normal += tempContacts[i].normal;
	//}
	//contact.point /= tempContacts.size();
	//contact.penetrationDepth /= tempContacts.size();
	//contact.normal = (contact.normal / tempContacts.size()).Normalise();
	//contacts.push_back(contact);
}

void SAT::GetContactData(int& smallestIndex, Primitive* box1, Primitive* box2, const Vector3& toCentre, float smallestPenetration, int singleSmallestIndex)
{	   	 
	//Point-face collision
	if (smallestIndex < 3)
	{
		//reference face from box 1, incident face from box 2
		PointFaceCollision(box1, box2, toCentre, smallestIndex, smallestPenetration);
		return;
	}
	else if (smallestIndex < 6)
	{
		//reference face from box 2, incident face from box 1
		PointFaceCollision(box2, box1, toCentre * -1.0f, smallestIndex - 3, smallestPenetration);
		return;
	}
	//Edge-edge collision
	else
	{
		smallestIndex -= 6;
		unsigned box1AxisIndex = smallestIndex / 3;
		unsigned box2AxisIndex = smallestIndex % 3;
		Vector3 box1Axis = Mathe::GetAxis(box1AxisIndex, box1->collisionVolume.axisMat);
		Vector3 box2Axis = Mathe::GetAxis(box2AxisIndex, box2->collisionVolume.axisMat);
		Vector3 normal = box1Axis.VectorProduct(box2Axis);
		normal.Normalise();

		if (toCentre.ScalarProduct(normal) > 0)
			normal *= -1.0f;

		Vector3 ptOnOneEdge = box1->collisionVolume.halfSize;// * 2.0;
		Vector3 ptOnTwoEdge = box2->collisionVolume.halfSize;// * 2.0;

		for (int i = 0; i < 3; i++)
		{
			if (box1AxisIndex == i)
				ptOnOneEdge[i] = 0;
			else if (Mathe::GetAxis(i, box1->collisionVolume.axisMat).ScalarProduct(normal) > 0)
				ptOnOneEdge[i] = -ptOnOneEdge[i];

			if (box2AxisIndex == i)
				ptOnTwoEdge[i] = 0;
			else if (Mathe::GetAxis(i, box2->collisionVolume.axisMat).ScalarProduct(normal) < 0)
				ptOnTwoEdge[i] = -ptOnTwoEdge[i];
		}

		Mathe::Transform(ptOnOneEdge, box1->collisionVolume.axisMat);
		Mathe::Transform(ptOnTwoEdge, box2->collisionVolume.axisMat);

		bool useOnesMidpoint = singleSmallestIndex > 2;
		//if (minimumAxes[0] < 6) useOnesMidpoint = true;

		Vector3 vertex = GetEdgeContactPoint(ptOnOneEdge, ptOnTwoEdge, box1Axis, box2Axis,
			(float)box1->collisionVolume.halfSize[box1AxisIndex], (float)box2->collisionVolume.halfSize[box2AxisIndex],
			useOnesMidpoint);

		//GENERATE CONTACT
		Contact contact(box1, box2);
		contact.penetrationDepth = smallestPenetration;
		contact.normal = normal;
		contact.point = vertex;
		contacts.push_back(contact);
		return;
	}
}

double SAT::GetPositionOnAxis(const CollisionVolume* box, const Vector3& axis)
{
	Matrix axisMat = box->axisMat;

	return
		box->halfSize.x * abs(axis.ScalarProduct(Mathe::GetAxis(0, axisMat)))
		+ box->halfSize.y * abs(axis.ScalarProduct(Mathe::GetAxis(1, axisMat)))
		+ box->halfSize.z * abs(axis.ScalarProduct(Mathe::GetAxis(2, axisMat)));
}

bool SAT::BoxesOverlapOnAxis(const CollisionVolume* box1, const CollisionVolume* box2, const Vector3& toCentre, Vector3 axis, int index, float& smallestPenetration, int& smallestIndex)
{
	if (axis.SquaredMagnitude() < 0.0001) return true;
	axis = axis.Normalise();

	float box1AxisPos = (float)GetPositionOnAxis(box1, axis);
	float box2AxisPos = (float)GetPositionOnAxis(box2, axis);

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

void SAT::PointFaceCollision(Primitive* box1, Primitive* box2, const Vector3& toCentre, int smallest, float penetration)
{
	//convert coord space to box 1 so box 1 pos is 0
	Vector3 normal = Mathe::GetAxis(smallest, box1->collisionVolume.axisMat);
	if (normal.ScalarProduct(toCentre) < 0)
	{
		normal *= -1.0;
	}
	Vector3 referenceNormal = normal;
	Mathe::TransformTranspose(referenceNormal, box1->collisionVolume.axisMat);
	referenceNormal = referenceNormal.Normalise();

	//normal (referenceNormal) should now be something like (0, 1, 0), easier to work with
	//and position is 0, but plane position is normal * half size

	//could get box2 on box1 axis using inverse but inversing a matrix is bad so
	//convert to dir and work with it like the normal

	//Vector3 box2position = toCentre; //relative to the normal
	Vector3 box2Pos = toCentre;
	box2Pos = box2Pos.Normalise();
	Mathe::TransformTranspose(box2Pos, box1->collisionVolume.axisMat);

	Vector3 incidentNormal = Vector3();
	float smallestFace = 1000.0f;
	for (unsigned f = 0; f < 6; f++)
	{
		float dist = referenceNormal.ScalarProduct(box2->collisionVolume.normals[f]);
		if (dist < smallestFace)
		{
			smallestFace = dist;
			incidentNormal = box2->collisionVolume.normals[f];
		}
	}
	if (incidentNormal.ScalarProduct(toCentre) > 0)
	{
		incidentNormal *= -1.0;
	}
	
	Vector3 referenceVertices[4] = { Vector3() };
	SetReferenceVertices(referenceNormal, referenceVertices, box1->collisionVolume.halfSize);
	//Vector3 referenceMin = Vector3(1000, 1000, 1000);
	//Vector3 referenceMax = Vector3();
	//SetReferenceMinMax(referenceNormal, box1->collisionVolume.halfSize, referenceMin, referenceMax);

	Vector3 incidentVertices[4] = { Vector3() };

	//Get local incident normal to get vertices in the correct order

	Vector3 localIncidentNormal = incidentNormal;
	Mathe::TransformTranspose(localIncidentNormal, box2->collisionVolume.axisMat);
	localIncidentNormal = localIncidentNormal.Normalise();
	SetReferenceVertices(localIncidentNormal, incidentVertices, box2->collisionVolume.halfSize);
	//Transform back to box 1 space
	for (unsigned v = 0; v < 4; v++)
	{
		Mathe::Transform(incidentVertices[v], box2->collisionVolume.axisMat);
		incidentVertices[v] -= box1->collisionVolume.centre;
	}


	/* This returns vertices in the wrong order which is not ideal for SH
	//WRT world coords
	Vector3 incidentWorldPoint = box2->collisionVolume.centre + box2->collisionVolume.halfSize * incidentNormal;
	unsigned index = 0;
	for (unsigned v = 0; v < 8; v++)
	{
		//if scalar(A - B, P) = 0, then A lies on plane where B is known point and P is normal
		if (abs((box2->collisionVolume.vertices[v] - incidentWorldPoint).ScalarProduct(incidentNormal)) < 0.001f)
		{
			//WRT box1
			incidentVertices[index] = box2->collisionVolume.vertices[v] - box1->collisionVolume.centre;
			//incidentVertices[index] = incidentVertices[index].Normalise();
			//Mathe::TransformTranspose(incidentVertices[index], box1->collisionVolume.axisMat);
			index++;
		}
	}*/

	std::vector<Vector3> clippedVertices;
	//index to ignore is dependent on what ref axis is 0
	SutherlandHodgmanBoxes(clippedVertices, referenceNormal, incidentVertices, referenceVertices);

	Vector3 contactPoint = Vector3();
	unsigned numContactPoints = 0;
	//only keep vertices below the reference plane
	for (unsigned v = 0; v < clippedVertices.size(); v++)
	{
		if (clippedVertices[v].ScalarProduct(referenceNormal) < (box1->collisionVolume.centre + box1->collisionVolume.halfSize).ScalarProduct(referenceNormal))
		{
			contactPoint += clippedVertices[v];
			numContactPoints++;
		}
	}
	if (contactPoint.x != 0) contactPoint.x /= (double)numContactPoints;
	if (contactPoint.y != 0) contactPoint.y /= (double)numContactPoints;
	if (contactPoint.z != 0) contactPoint.z /= (double)numContactPoints;
	//transform to world coordinates
	Mathe::Transform(contactPoint, box1->collisionVolume.axisMat);

	Contact contact(box2, box1);
	contact.normal = normal;
	contact.penetrationDepth = penetration;
	contact.point = contactPoint;
	contacts.push_back(contact);

	//Order of finding all collision points:
	// [x] 1. Get the normal from box1
	// [x] 2. Find the incident face on box 2
	// [x] 3. Find the four vertices on the normal's plane
	// [x] 4. Find the four vertices on the incident face
	// [x] 5. Sutherland Hodgman algorithm
	// [x] 6. Cull points which are above the normal plane
	// [x] 7. Average remaining points
}

Vector3 SAT::GetEdgeContactPoint(const Vector3& edgePoint1, const Vector3& edgePoint2, Vector3& axisOne, Vector3& axisTwo, float halfSize1, float halfSize2, bool useOneMidpoint)
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

	float edgeNearestA = (((scaleOneTwo * scaleTwo) - (squareMag2 * scaleOne)) / denominator);// * 2.0f;
	float edgeNearestB = (((squareMag1 * scaleTwo) - (scaleOneTwo * scaleOne)) / denominator);// * 2.0f;

	if (edgeNearestA > halfSize1 || edgeNearestA < -halfSize1
		|| edgeNearestB > halfSize2 || edgeNearestB < -halfSize2)
	{
		return useOneMidpoint ? edgePoint1 : edgePoint2;
	}
	else
	{
		contact1 = edgePoint1 + (axisOne * edgeNearestA);
		contact2 = edgePoint2 + (axisTwo * edgeNearestB);
		return (contact1 + contact2) * 0.5;
	}
}

void SAT::SetReferenceVertices(const Vector3& normal, Vector3* planes, const Vector3& halfSize)
{
	if (abs(normal.x) == 1)
	{
		planes[0] = Vector3(0, 1, 1) * halfSize;
		planes[1] = Vector3(0, 1, -1) * halfSize;
		planes[2] = Vector3(0, -1, -1) * halfSize;
		planes[3] = Vector3(0, -1, 1) * halfSize;
	}
	else if (abs(normal.y) == 1)
	{
		planes[0] = Vector3(1, 0, 1) * halfSize;
		planes[1] = Vector3(1, 0, -1) * halfSize;
		planes[2] = Vector3(-1, 0, -1) * halfSize;
		planes[3] = Vector3(-1, 0, 1) * halfSize;
	}
	else if (abs(normal.z) == 1)
	{
		planes[0] = Vector3(1, 1, 0) * halfSize;
		planes[1] = Vector3(1, -1, 0) * halfSize;
		planes[2] = Vector3(-1, -1, 0) * halfSize;
		planes[3] = Vector3(-1, 1, 0) * halfSize;
	}
	else
	{
		std::cout << "WARNING: reference vertices not set because normal is not properly set up!" << std::endl;
	}
}

void SAT::SetReferenceMinMax(const Vector3& normal, const Vector3& halfSize, Vector3& min, Vector3& max)
{
	//Modified only to get minmax
	Vector3 planes[4] = { Vector3() };

	if (abs(normal.x) == 1)
	{
		planes[0] = Vector3(0, 1, 0).VectorProduct(halfSize);
		planes[1] = Vector3(0, -1, 0).VectorProduct(halfSize);
		planes[2] = Vector3(0, 0, 1).VectorProduct(halfSize);
		planes[3] = Vector3(0, 0, -1).VectorProduct(halfSize);
	}
	else if (abs(normal.y) == 1)
	{
		planes[0] = Vector3(1, 0, 0).VectorProduct(halfSize);
		planes[1] = Vector3(-1, 0, 0).VectorProduct(halfSize);
		planes[2] = Vector3(0, 0, 1).VectorProduct(halfSize);
		planes[3] = Vector3(0, 0, -1).VectorProduct(halfSize);
	}
	else if (abs(normal.z) == 1)
	{
		planes[0] = Vector3(1, 0, 0).VectorProduct(halfSize);
		planes[1] = Vector3(-1, 0, 0).VectorProduct(halfSize);
		planes[2] = Vector3(0, 1, 0).VectorProduct(halfSize);
		planes[3] = Vector3(0, -1, 0).VectorProduct(halfSize);
	}

	for (unsigned v = 0; v < 4; v++)
	{
		if (max.SumComponents() < planes[v].SumComponents())
		{
			max = planes[v];
		}
		if (min.SumComponents() > planes[v].SumComponents())
		{
			min = planes[v];
		}
	}
}

Vector3 SAT::CalculateIntersection(const Vector3& v1, const Vector3& v2, const unsigned axes[2], const Vector3& clippingMin, const Vector3& clippingMax)
{
	double dc[2] = { clippingMin[axes[0]] - clippingMax[axes[0]], clippingMin[axes[1]] - clippingMax[axes[1]] };
	double dp[2] = { v1[axes[0]] - v2[axes[0]], v1[axes[1]] - v2[axes[1]] };

	double n1 = clippingMin[axes[0]] * clippingMax[axes[1]] - clippingMin[axes[1]] * clippingMax[axes[0]];
	double n2 = v1[axes[0]] * v2[axes[1]] - v1[axes[1]] * v2[axes[0]];
	double n3 = 1.0 / (dc[0] * dp[1] - dc[1] * dp[0]);

	Vector3 intersection = Vector3();
	intersection[axes[0]] = (n1 * dp[axes[0]] - n2 * dc[axes[0]]) * n3;
	intersection[axes[1]] = (n1 * dp[axes[1]] - n2 * dc[axes[1]]) * n3;

	//Get the value of the remaining axis
	double percAcrossLine = (v1[axes[0]] + intersection[axes[0]]) / v2[axes[0]];
	unsigned i = 0;
	if (axes[0] == 0 && axes[1] == 1) i = 2;
	else if (axes[0] == 0 && axes[1] == 2) i = 1;

	intersection[i] = v1[i] + (v2[i] - v1[i]) * percAcrossLine;

	return intersection;
}

void SAT::SutherlandHodgmanBoxes(std::vector<Vector3>& clipped, const Vector3& normal, const Vector3* polyVertices, const Vector3* clippingVertices)
{
	const unsigned num = 4;
	const float epsilon = 0.001f;
	unsigned axes[2] = { 0 };
	if (normal.x == 1)
	{
		axes[0] = 1;
		axes[1] = 2;
	}
	else if (normal.y == 1)
	{
		axes[0] = 0;
		axes[1] = 2;
	}
	else if (normal.z == 1)
	{
		axes[0] = 0;
		axes[1] = 1;
	}

	for (unsigned edge = 0; edge < num; edge++)
	{
		//vec prod(points and plane normal)
		for (unsigned v = 0; v < num; v++)
		{
			Vector3 v1 = polyVertices[v].VectorProduct(normal) + polyVertices[v] * normal;
			Vector3 v2 = polyVertices[(v == 3) ? 0 : v + 1].VectorProduct(normal) + polyVertices[v + 1] * normal;
			Vector3 edgeMin = clippingVertices[edge];
			Vector3 edgeMax = clippingVertices[edge + 1];

			if (InsideEdge(v1[axes[0]], v1[axes[1]], edgeMax[axes[0]], edgeMax[axes[1]], edgeMin[axes[0]], edgeMin[axes[1]]))
			{
				if (InsideEdge(v2[axes[0]], v2[axes[1]], edgeMax[axes[0]], edgeMax[axes[1]], edgeMin[axes[0]], edgeMin[axes[1]]))
					clipped.push_back(v2);
				else
					clipped.push_back(CalculateIntersection(v1, v2, axes, edgeMin, edgeMax));
			}
			else if (InsideEdge(v2[axes[0]], v2[axes[1]], edgeMax[axes[0]], edgeMax[axes[1]], edgeMin[axes[0]], edgeMin[axes[1]]))
			{
				clipped.push_back(CalculateIntersection(v1, v2, axes, edgeMin, edgeMax));
				clipped.push_back(v2);
			}
		}
	}
}

bool SAT::InsideEdge(double px, double py, double edgeMaxX, double edgeMaxY, double edgeMinX, double edgeMinY)
{
	return (edgeMaxY - edgeMinY) * px + (edgeMinX - edgeMaxX) * py + (edgeMaxX * edgeMinY - edgeMinX * edgeMaxY) < 0;
}
