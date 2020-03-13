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
		contact.normal = normal.Normalise();
		contact.point = vertex;
		contacts.push_back(contact);
		return;
	}
}

double SAT::GetPositionOnAxis(const CollisionVolume* box, const Vector3& axis)
{
	Matrix4 axisMat = box->axisMat;

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
	float comparison1 = (box1->collisionVolume.centre + normal).ScalarProduct(box2->collisionVolume.centre);
	float comparison2 = (box1->collisionVolume.centre - normal).ScalarProduct(box2->collisionVolume.centre);
	if (comparison1 < comparison2)
	{
		normal *= -1.0;
	}
	Vector3 referenceNormal = normal;
	Mathe::TransformTranspose(referenceNormal, box1->collisionVolume.axisMat);
	referenceNormal = referenceNormal.Normalise();

	//normal (referenceNormal) should now be something like (0, 1, 0), easier to work with
	//and position is 0, but plane position is normal * half size

	//could get box2 on box1 axis using inverse but inversing a matrix is
	//expensive so convert to dir and work with it like the normal

	//Vector3 box2Pos = toCentre;
	//box2Pos = box2Pos.Normalise();
	//Mathe::TransformTranspose(box2Pos, box1->collisionVolume.axisMat);

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
	//if (incidentNormal.ScalarProduct(toCentre) > 0)
	//{
	//	incidentNormal *= -1.0;
	//}
	
	Vector3 referenceVertices[4] = { Vector3() };
	if (!SetReferenceVertices(referenceNormal, referenceVertices, box1->collisionVolume.halfSize)) return;
	//Vector3 referenceMin = Vector3(1000, 1000, 1000);
	//Vector3 referenceMax = Vector3();
	//SetReferenceMinMax(referenceNormal, box1->collisionVolume.halfSize, referenceMin, referenceMax);

	bool box1Above2 = box1->collisionVolume.centre.ScalarProduct(referenceNormal) > box2->collisionVolume.centre.ScalarProduct(referenceNormal);

	Vector3 incidentVertices[4] = { Vector3() };

	//Get local incident normal to get vertices in the correct order
	Vector3 localIncidentNormal = incidentNormal;
	Mathe::TransformTranspose(localIncidentNormal, box2->collisionVolume.axisMat);
	localIncidentNormal = localIncidentNormal.Normalise();
	if (!SetReferenceVertices(localIncidentNormal, incidentVertices, box2->collisionVolume.halfSize)) return;
	//Transform back to box 1 space
	for (unsigned v = 0; v < 4; v++)
	{
		Mathe::Transform(incidentVertices[v], box2->collisionVolume.axisMat);
		incidentVertices[v] += localIncidentNormal * box2->collisionVolume.halfSize;//box1->collisionVolume.centre / box1->collisionVolume.halfSize) * referenceNormal;
		incidentVertices[v] -= box1->collisionVolume.centre;// *(localIncidentNormal.SumComponents() < 0 ? -1.0 : 1.0);
	}

	std::vector<Vector3> clippedVertices;
	//index to ignore is dependent on what ref axis is 0
	SutherlandHodgman(clippedVertices, referenceNormal, incidentVertices, referenceVertices);

	Vector3 contactPoint = Vector3();
	unsigned numContactPoints = 0;

	bool mergeContacts = false;
	normal = normal * (normal.ScalarProduct(toCentre) > 0 ? -1.0 : 1.0);

	//only keep vertices below the reference plane
	float relBox = abs((box1->collisionVolume.centre + box1->collisionVolume.halfSize).ScalarProduct(referenceNormal));
	float pen = 0;
	for (unsigned v = 0; v < clippedVertices.size(); v++)
	{
		Mathe::Transform(clippedVertices[v], box1->collisionVolume.axisMat);
		float clipped = abs(clippedVertices[v].ScalarProduct(referenceNormal));

		if (clipped < relBox) 
		{
			if (!mergeContacts)
			{
				Contact contact(box1, box2);
				contact.normal = normal;
				contact.penetrationDepth = (box1Above2 ? clipped : (relBox - clipped));// clippedVertices[v].ScalarProduct(normal);
				contact.point = clippedVertices[v];
				contacts.push_back(contact);
			}
			else
			{
				pen += (box1Above2 ? clipped : (relBox - clipped));
				contactPoint += clippedVertices[v];
				numContactPoints++;
			}
		}
	}
	if (!mergeContacts) return;

	//Previous implementation using only one contact point

	if (contactPoint.x != 0) contactPoint.x /= (double)numContactPoints;
	if (contactPoint.y != 0) contactPoint.y /= (double)numContactPoints;
	if (contactPoint.z != 0) contactPoint.z /= (double)numContactPoints;
	if (pen != 0) pen /= (float)numContactPoints;
	//transform to world coordinates
	//Mathe::Transform(contactPoint, box1->collisionVolume.axisMat);
	//contactPoint += box1->collisionVolume.centre;

	Contact contact(box1, box2);
	contact.penetrationDepth = pen;
	contact.normal = normal;
	contact.point = contactPoint;// +box1->collisionVolume.centre;// +(contact.normal * box1->collisionVolume.halfSize * (box1Above2 ? 1.0 : -1.0));
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

bool SAT::SetReferenceVertices(const Vector3& normal, Vector3* planes, const Vector3& halfSize)
{
	if (abs(abs(normal.x) - 1.0) < 0.001)
	{
		planes[0] = Vector3(0, 1, 1) * halfSize;
		planes[1] = Vector3(0, 1, -1) * halfSize;
		planes[2] = Vector3(0, -1, -1) * halfSize;
		planes[3] = Vector3(0, -1, 1) * halfSize;
	}
	else if (abs(abs(normal.y) - 1.0) < 0.001)
	{
		planes[0] = Vector3(1, 0, 1) * halfSize;
		planes[1] = Vector3(1, 0, -1) * halfSize;
		planes[2] = Vector3(-1, 0, -1) * halfSize;
		planes[3] = Vector3(-1, 0, 1) * halfSize;
	}
	else if (abs(abs(normal.z) - 1.0) < 0.001)
	{
		planes[0] = Vector3(1, 1, 0) * halfSize;
		planes[1] = Vector3(1, -1, 0) * halfSize;
		planes[2] = Vector3(-1, -1, 0) * halfSize;
		planes[3] = Vector3(-1, 1, 0) * halfSize;
	}
	else
	{
		std::cout << "WARNING: reference vertices not set because normal is not properly set up! Cancelling contact generation." << std::endl;
		std::cout << "Normal: " << normal.x << ", " << normal.y << ", " << normal.z << std::endl;
		return false;
	}
	return true;
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
	//intersection = (v1 + m(v2 - v1)) = (clippingMin + n(clippingMax - clippingMin))
	//ix = (v1x + m(v2x - v1x)) = (clippingMinx + n(clippingMaxx - clippingMinx))

	double x1 = clippingMin[axes[0]];
	double x2 = clippingMax[axes[0]];
	double x3 = v1[axes[0]];
	double x4 = v2[axes[0]];
	double y1 = clippingMin[axes[1]];
	double y2 = clippingMax[axes[1]];
	double y3 = v1[axes[1]];
	double y4 = v2[axes[1]];

	//x intersect
	double num = (x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4);
	double den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	double X = num / den;

	//y intersect
	num = (x1 * y2 - y1 * x2) * (y3 - y4) -	(y1 - y2) * (x3 * y4 - y3 * x4);
	den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	double Y = num / den;

	Vector3 intersection;
	intersection[axes[0]] = X;
	intersection[axes[1]] = Y;

	//solve for m or n
	//double m1 =	(v1[axes[0]] - clippingMin[axes[0]]) * (clippingMin[axes[1]] - clippingMax[axes[1]])
	//	- (v1[axes[1]] - clippingMin[axes[1]]) * (clippingMin[axes[0]] - clippingMax[axes[0]]);
	//double m2 = (v1[axes[0]] - v2[axes[0]]) * (clippingMin[axes[1]] - clippingMax[axes[1]])
	//	- (v1[axes[1]] - v2[axes[1]]) * (clippingMin[axes[0]] - clippingMax[axes[0]]);
	//double m = m1 / m2;
	//Vector3 v2v1 = v2 - v1;
	//Vector3 intersection = v1 + (v2v1 * m);
	// *(v2v1.SumComponents() < 0 ? -1.0 : 1.0));
	//if (m < 0)
	//{
	//	m1 = (v1[axes[0]] - v1[axes[0]]) * (v1[axes[1]] - clippingMin[axes[1]])
	//		- (v1[axes[1]] - v2[axes[1]]) * (v1[axes[0]] - clippingMin[axes[0]]);
	//	m = m1 / m2;
	//	intersection = clippingMin + ((clippingMax - clippingMin) * m);
	//}

	//Get the value of the remaining axis
	double percAcrossLine = 1;
	if (x2 != 0)
		percAcrossLine = (x1 + X) / x2;
	else if (y2 != 0)
		percAcrossLine = (y1 + Y) / y2;

	unsigned i = 0;
	if (axes[0] == 0)
	{
		if (axes[1] == 1)
			i = 2;
		else if (axes[1] == 2)
			i = 1;
	}

	intersection[i] = v1[i] + (v2[i] - v1[i]) * percAcrossLine;

	return intersection;
}

//Vector3 SAT::CalculateIntersection(const Vector3& v1, const Vector3& v2, const unsigned axes[2], const Vector3& clippingMin, const Vector3& clippingMax)
//{
//	//https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection#Given_two_points_on_each_line
//
//	double dc[2] = { clippingMin[axes[0]] - clippingMax[axes[0]], clippingMin[axes[1]] - clippingMax[axes[1]] };
//	double dp[2] = { v1[axes[0]] - v2[axes[0]], v1[axes[1]] - v2[axes[1]] };
//
//	double n1 = clippingMin[axes[0]] * clippingMax[axes[1]] - clippingMin[axes[1]] * clippingMax[axes[0]];
//	double n2 = v1[axes[0]] * v2[axes[1]] - v1[axes[1]] * v2[axes[0]];
//	double div = dc[0] * dp[1] - dc[1] * dp[0];
//	double n3 = div == 0 ? 0 : (1.0 / div);
//	//double n3 = (dc[0] * dp[1] - dc[1] * dp[0]);
//
//	Vector3 intersection = Vector3();
//	intersection[axes[0]] = (n1 * dp[0] - n2 * dc[0]) * n3;
//	intersection[axes[1]] = (n1 * dp[1] - n2 * dc[1]) * n3;
//
//	//Get the value of the remaining axis
//	double percAcrossLine = (v1[axes[0]] + intersection[axes[0]]) / v2[axes[0]];
//	unsigned i = 0;
//	if (axes[0] == 0)
//	{
//		if (axes[1] == 1)
//			i = 2;
//		else if (axes[1] == 2)
//			i = 1;
//	}
//
//	intersection[i] = v1[i] + (v2[i] - v1[i]) * percAcrossLine;
//
//	return intersection;
//}

void SAT::SutherlandHodgman(std::vector<Vector3>& _clipped, const Vector3& normal, const Vector3* polyVertices, const Vector3* clippingVertices)
{
	const unsigned numEdges = sizeof(clippingVertices);
	const unsigned polyVertSize = sizeof(polyVertices);
	unsigned numVertices = polyVertSize;
	const float epsilon = 0.001f;

	unsigned axes[2] = { 0, 0 };
	if (abs(abs(normal.x) - 1.0) < epsilon)
	{
		axes[0] = 1;
		axes[1] = 2;
	}
	else if (abs(abs(normal.y) - 1.0) < epsilon)
	{
		axes[0] = 0;
		axes[1] = 2;
	}
	else if (abs(abs(normal.z) - 1.0) < epsilon)
	{
		axes[0] = 0;
		axes[1] = 1;
	}

	Vector3 min = clippingVertices[0];
	Vector3 max = clippingVertices[1];
	for (unsigned i = 0; i < numVertices; i++)
	{
		double sum = clippingVertices[i].SumComponents();
		if (min.SumComponents() > sum)
			min = clippingVertices[i];
		if (max.SumComponents() < sum)
			max = clippingVertices[i];
	}

	const unsigned maxPoints = polyVertSize * polyVertSize * 2;
	Vector3 newPoints[maxPoints];
	for (unsigned i = 0; i < numVertices; i++)
	{
		newPoints[i] = polyVertices[i];
	}
	unsigned newSize = 4;

	for (unsigned edge = 0; edge < numEdges; edge++) //for each clipping edge
	{
		Vector3 edgeMin = clippingVertices[edge];
		Vector3 edgeMax = clippingVertices[(edge + 1) % numEdges];
		//if (edgeMin.SumComponents() < edgeMax.SumComponents())
		//{
		//	Vector3 temp = edgeMin;
		//	edgeMin = edgeMax;
		//	edgeMax = temp;
		//}

		for (unsigned v = 0; v < numVertices; v++) //for each input vertex
		{
			Vector3 v1 = newPoints[v];
			Vector3 v2 = newPoints[(v + 1) % numVertices];
			if (v1 == v2 ||
				(v1[axes[0]] != v2[axes[0]]
				&& (v1[axes[1]] != v2[axes[1]]))) 
				continue;

			bool insideEdge1 = InsideEdge(v1[axes[0]], v1[axes[1]], edgeMax[axes[0]], edgeMax[axes[1]], edgeMin[axes[0]], edgeMin[axes[1]]);
			bool insideEdge2 = InsideEdge(v2[axes[0]], v2[axes[1]], edgeMax[axes[0]], edgeMax[axes[1]], edgeMin[axes[0]], edgeMin[axes[1]]);

			if (insideEdge1 && insideEdge2)
			{
				//VerifyVertex(_clipped, v2, max, min, axes);
				//newPoints[newSize] = v1;
				//newSize++;
				if (std::find(std::begin(newPoints), std::end(newPoints), v2) == std::end(newPoints))
				{
					newPoints[newSize] = v2;
					newSize++;
				}
			}
			else if (!insideEdge1 && insideEdge2)
			{
				//VerifyVertex(_clipped, CalculateIntersection(v1, v2, axes, edgeMin, edgeMax), max, min, axes);
				//VerifyVertex(_clipped, v2, max, min, axes);
				Vector3 vec = CalculateIntersection(v1, v2, axes, edgeMin, edgeMax);
				if (std::find(std::begin(newPoints), std::end(newPoints), vec) == std::end(newPoints))
				{
					newPoints[newSize] = vec;
					newSize++;
				}
				if (std::find(std::begin(newPoints), std::end(newPoints), v2) == std::end(newPoints))
				{
					newPoints[newSize] = v2;
					newSize++;
				}
			}
			else if (insideEdge1 && !insideEdge2)
			{
				//VerifyVertex(_clipped, CalculateIntersection(v1, v2, axes, edgeMin, edgeMax), max, min, axes);
				Vector3 vec = CalculateIntersection(v1, v2, axes, edgeMin, edgeMax);
				if (std::find(std::begin(newPoints), std::end(newPoints), vec) == std::end(newPoints))
				{
					newPoints[newSize] = vec;
					newSize++;
				}
			}
			if (newSize >= maxPoints)
				break;
		}
		numVertices = (newSize < 4) ? 4 : newSize;
		for (unsigned i = 0; i < numVertices; i++)
		{
			VerifyVertex(_clipped, newPoints[i], max, min, axes);
		}
	}
}

bool SAT::InsideEdge(double px, double py, double edgeMaxX, double edgeMaxY, double edgeMinX, double edgeMinY)
{
	double one = (edgeMaxX - edgeMinX) * (py - edgeMinY);
	double two = (edgeMaxY - edgeMinY) * (px - edgeMinX);
	return (one - two) < 0;

	//return (edgeMaxX - edgeMinX) * (py - edgeMinY) - (edgeMaxY - edgeMinY) * (px - edgeMinX) < 0;
}

void SAT::VerifyVertex(std::vector<Vector3>& clipped, const Vector3& vec, const Vector3& max, const Vector3& min, const unsigned axes[])
{
	if (vec[axes[0]] <= max[axes[0]] && vec[axes[0]] >= min[axes[0]]
		&& vec[axes[1]] <= max[axes[1]] && vec[axes[1]] >= min[axes[1]])
	{
		if (std::find(clipped.begin(), clipped.end(), vec) == clipped.end())
			clipped.push_back(vec);
	}
}
