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
	//Box1 axes
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(0, cv1->axisMat), 0, smallestPenetration, smallestIndex)) return; //1 x
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(1, cv1->axisMat), 1, smallestPenetration, smallestIndex)) return; //1 y
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(2, cv1->axisMat), 2, smallestPenetration, smallestIndex)) return; //1 z
	//Box2 axes																													 
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(0, cv2->axisMat), 3, smallestPenetration, smallestIndex)) return; //2 x
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(1, cv2->axisMat), 4, smallestPenetration, smallestIndex)) return; //2 y
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(2, cv2->axisMat), 5, smallestPenetration, smallestIndex)) return; //2 z

	int singleSmallestIndex = smallestIndex;

	//Cross product axes
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(0, cv1->axisMat).VectorProduct(Mathe::GetAxis(0, cv2->axisMat)), 6, smallestPenetration, smallestIndex)) return;
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(0, cv1->axisMat).VectorProduct(Mathe::GetAxis(1, cv2->axisMat)), 7, smallestPenetration, smallestIndex)) return;
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(0, cv1->axisMat).VectorProduct(Mathe::GetAxis(2, cv2->axisMat)), 8, smallestPenetration, smallestIndex)) return;
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(1, cv1->axisMat).VectorProduct(Mathe::GetAxis(0, cv2->axisMat)), 9, smallestPenetration, smallestIndex)) return;
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(1, cv1->axisMat).VectorProduct(Mathe::GetAxis(1, cv2->axisMat)), 10, smallestPenetration, smallestIndex)) return;
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(1, cv1->axisMat).VectorProduct(Mathe::GetAxis(2, cv2->axisMat)), 11, smallestPenetration, smallestIndex)) return;
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(2, cv1->axisMat).VectorProduct(Mathe::GetAxis(0, cv2->axisMat)), 12, smallestPenetration, smallestIndex)) return;
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(2, cv1->axisMat).VectorProduct(Mathe::GetAxis(1, cv2->axisMat)), 13, smallestPenetration, smallestIndex)) return;
	if (!BoxesOverlapOnAxis(cv1, cv2, toCentre, Mathe::GetAxis(2, cv1->axisMat).VectorProduct(Mathe::GetAxis(2, cv2->axisMat)), 14, smallestPenetration, smallestIndex)) return;

	GetContactData(smallestIndex, box1, box2, toCentre, smallestPenetration, singleSmallestIndex);
}

void SAT::GetContactData(int& smallestIndex, Primitive* box1, Primitive* box2, const Vector3& toCentre, float smallestPenetration, int singleSmallestIndex)
{	   	 
	//Point-face collision
	if (smallestIndex < 3)
	{
		PointFaceCollision(box1, box2, toCentre, smallestIndex, smallestPenetration);
		return;
	}
	else if (smallestIndex < 6)
	{
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
		Vector3 axis = box1Axis.VectorProduct(box2Axis);
		axis.Normalise();

		if (toCentre.ScalarProduct(axis) > 0)
			axis *= -1.0f;

		Vector3 ptOnOneEdge = box1->collisionVolume.halfSize;// * 2.0;
		Vector3 ptOnTwoEdge = box2->collisionVolume.halfSize;// * 2.0;

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

		Mathe::Transform(ptOnOneEdge, box1->collisionVolume.axisMat);
		Mathe::Transform(ptOnTwoEdge, box2->collisionVolume.axisMat);

		Vector3 vertex = GetEdgeContactPoint(ptOnOneEdge, ptOnTwoEdge, box1Axis, box2Axis,
			(float)box1->collisionVolume.halfSize[box1AxisIndex], (float)box2->collisionVolume.halfSize[box2AxisIndex],
			singleSmallestIndex > 2);

		//GENERATE CONTACT
		Contact contact(box1, box2);
		contact.penetrationDepth = smallestPenetration;
		contact.normal = axis;
		contact.point = vertex;
		contacts.push_back(contact);
		return;
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

void SAT::PointFaceCollision(Primitive* box1, Primitive* box2, const Vector3& toCentre, int smallest, float penetration)
{
	/*std::vector<Contact> pointFaceContacts;
	unsigned numContacts = 0;

	for (unsigned v = 0; v < 8; v++)
	{
		Vector3 relativePoint = box1->collisionVolume.vertices[v];
		Matrix mat = box2->collisionVolume.axisMat;
		mat.Inverse4x4();
		Mathe::Transform(relativePoint, mat);
		//Mathe::Transform(relativePoint, box2->collisionVolume.axisMat);
		//Mathe::MatrixInverse(mat, relativePoint);

		//would look nicer in a loop but not necessary

		double minDepth = box2->collisionVolume.halfSize.x - abs(relativePoint.x);
		if (minDepth < 0) continue;
		Vector3 normal = Mathe::GetAxis(0, box2->collisionVolume.axisMat) * ((relativePoint.x < 0) ? -1.0f : 1.0f);

		double depth = box2->collisionVolume.halfSize.y - abs(relativePoint.y);
		if (depth < 0) continue;
		else if (depth < minDepth)
		{
			minDepth = depth;
			normal = Mathe::GetAxis(1, box2->collisionVolume.axisMat) * ((relativePoint.y < 0) ? -1.0f : 1.0f);
		}
		depth = box2->collisionVolume.halfSize.z - abs(relativePoint.z);
		if (depth < 0) continue;
		else if (depth < minDepth)
		{
			minDepth = depth;
			normal = Mathe::GetAxis(2, box2->collisionVolume.axisMat) * ((relativePoint.z < 0) ? -1.0f : 1.0f);
		}

		Contact contact(box1, box2);
		contact.normal = normal;
		contact.point = box1->collisionVolume.vertices[v];
		contact.penetrationDepth = minDepth;
		pointFaceContacts.push_back(contact);
		numContacts++;
		//if (numContacts >= 4) break;
	}

	if (numContacts <= 0) return;
	else if (numContacts == 1)
	{
		contacts.push_back(pointFaceContacts[0]);
		return;
	}

	Contact mergedContact(box1, box2);
	for (unsigned i = 0; i < numContacts; i++)
	{
		mergedContact.point += pointFaceContacts[i].point;
		mergedContact.penetrationDepth += pointFaceContacts[i].penetrationDepth;
	}
	mergedContact.point /= (float)numContacts;
	mergedContact.penetrationDepth /= (float)numContacts;
	contacts.push_back(mergedContact);
	//return mergedContact;
	*/

	Vector3 normal = Mathe::GetAxis(smallest, box1->collisionVolume.axisMat);

	if (normal.ScalarProduct(toCentre) > 0) 
		normal *= -1.0f;

	//check other vertices and if the pen depth is the same then > one point
	Vector3 vertices[1];
	
	vertices[0] = box2->collisionVolume.halfSize * 2.0;
	//vertices[1] = box2->collisionVolume.halfSize * -2.0;
	//if less than zero, then switch vertex sign
	if (Mathe::GetAxis(0, box2->collisionVolume.axisMat).ScalarProduct(normal) < 0)
	{
		vertices[0].x = -vertices[0].x;
		//vertices[1].x = -vertices[1].x;
	}
	if (Mathe::GetAxis(1, box2->collisionVolume.axisMat).ScalarProduct(normal) < 0)
	{
		vertices[0].y = -vertices[0].y;
		//vertices[1].y = -vertices[1].y;
	}
	if (Mathe::GetAxis(2, box2->collisionVolume.axisMat).ScalarProduct(normal) < 0)
	{
		vertices[0].z = -vertices[0].z;
		//vertices[1].z = -vertices[1].z;
	}
	
	Contact contact(box1, box2);
	contact.normal = normal;
	contact.penetrationDepth = penetration;
	Mathe::Transform(vertices[0], box2->collisionVolume.axisMat);
	contact.point = vertices[0];
	contacts.push_back(contact);

	//Mathe::Transform(vertices[1], box2->collisionVolume.axisMat);
	//contact.point = vertices[1];
	//contacts.push_back(contact);
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
		return (contact1 + contact2) * 0.5;
	}
}


/*int Poly::GetSupportPoints(const Vector3& Dir, Vector3* support)
{
	int count = 0;    
	const float threshold = 1.0E-8;    
	float mind;    
	for (int i = 0; i < m_NumVerts; i++)
	{
		float d = m_Vert.Vector(Dir);        
		if (i == 0 || d < mind) 
		{ 
			mind = d; 
		}
	}    
	for (int i = 0; i < m_NumVerts; i++) 
	{ 
		float d = m_Vert.DotProduct(Dir);        
		if (d < (mind + threshold)) 
		{ 
			support[count] = m_Vert;            
			count++;            
			if (count >= 2) 
				return count; 
		} 
	}    
	return count;
}*/