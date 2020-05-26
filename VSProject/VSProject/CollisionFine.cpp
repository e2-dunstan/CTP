#include "CollisionFine.h"
#include "Primitive.h"

void CollisionFine::DetectContacts(Primitive* prim1, Primitive* prim2)
{
	if ((prim1->isStatic && prim2->isStatic)
		|| (prim1->freeze && prim2->freeze)) return;

	if (prim1->type == PrimitiveType::PLANE && prim2->type == PrimitiveType::SPHERE)
	{
		Plane* p1 = dynamic_cast<Plane*>(prim1);
		Sphere* p2 = dynamic_cast<Sphere*>(prim2);
		SphereAndPlane(p2, p1,
			p2->collisionVolume.centre, p2->radius,
			p1->collisionVolume.centre, p1->collisionVolume.normal);
	}
	else if (prim1->type == PrimitiveType::PLANE && prim2->type == PrimitiveType::BOX)
	{
		Plane* p1 = dynamic_cast<Plane*>(prim1);
		Box* p2 = dynamic_cast<Box*>(prim2);
		BoxAndPlane(p2, p1, p1->collisionVolume.centre, p1->collisionVolume.normal);
	}
	else if (prim1->type == PrimitiveType::BOX && prim2->type == PrimitiveType::PLANE)
	{
		Box* p1 = dynamic_cast<Box*>(prim1);
		Plane* p2 = dynamic_cast<Plane*>(prim2);
		BoxAndPlane(p1, p2, p2->collisionVolume.centre, p2->collisionVolume.normal);
	}
	else if (prim1->type == PrimitiveType::PLANE && prim2->type == PrimitiveType::CYLINDER)
	{
		//CylinderAndPlane(prim2, prim1);
	}
	else if (prim1->type == PrimitiveType::PLANE && prim2->type == PrimitiveType::CAPSULE)
	{
		//CapsuleAndPlane(prim2, prim1);
	}
	else if (prim1->type == PrimitiveType::SPHERE && prim2->type == PrimitiveType::SPHERE)
	{
		Sphere* p1 = dynamic_cast<Sphere*>(prim1);
		Sphere* p2 = dynamic_cast<Sphere*>(prim2);
		SphereAndSphere(p1, p2,
			p1->collisionVolume.centre, p1->radius,
			p2->collisionVolume.centre, p2->radius);
	}
	else if (prim1->type == PrimitiveType::SPHERE && prim2->type == PrimitiveType::PLANE)
	{
		Plane* p1 = dynamic_cast<Plane*>(prim2);
		Sphere* p2 = dynamic_cast<Sphere*>(prim1);
		SphereAndPlane(p2, p1,
			p2->collisionVolume.centre, p2->radius,
			p1->collisionVolume.centre, p1->collisionVolume.normal);
	}
	else if (prim1->type == PrimitiveType::SPHERE && prim2->type == PrimitiveType::BOX)
	{
		Sphere* p1 = dynamic_cast<Sphere*>(prim1);
		Box* p2 = dynamic_cast<Box*>(prim2);
		SphereAndBox(p1, p2, p1->collisionVolume.centre, p1->radius);
	}
	else if (prim1->type == PrimitiveType::SPHERE && prim2->type == PrimitiveType::CYLINDER)
	{
		//CylinderAndSphere(prim2, prim1);
	}
	else if (prim1->type == PrimitiveType::SPHERE && prim2->type == PrimitiveType::CAPSULE)
	{
		//CapsuleAndSphere(prim2, prim1);
	}
	//else if (prim1->type == PrimitiveType::BOX && prim2->type == PrimitiveType::PLANE)
	//{
	//	BoxAndPlane(prim1, prim2, prim2->collisionVolume.centre, prim2->collisionVolume.normal);
	//}
	else if (prim1->type == PrimitiveType::BOX && prim2->type == PrimitiveType::SPHERE)
	{
		Box* p1 = dynamic_cast<Box*>(prim1);
		Sphere* p2 = dynamic_cast<Sphere*>(prim2);
		SphereAndBox(p2, p1, p2->collisionVolume.centre, p2->radius);
	}
	//else if (prim1->type == PrimitiveType::BOX && prim2->type == PrimitiveType::BOX)
	//{
		//BoxAndBox(prim1, prim2);
	//}
	else if (prim1->type == PrimitiveType::BOX && prim2->type == PrimitiveType::CYLINDER)
	{
		//CylinderAndBox(prim2, prim1);
	}
	else if (prim1->type == PrimitiveType::BOX && prim2->type == PrimitiveType::CAPSULE)
	{

	}
	else if (prim1->type == PrimitiveType::CYLINDER && prim2->type == PrimitiveType::CYLINDER)
	{
		//CylinderAndCylinder(prim1, prim2);
	}
	//else if (prim1->type == PrimitiveType::CYLINDER && prim2->type == PrimitiveType::PLANE) 
	//{
	//	CylinderAndPlane(prim1, prim2);
	//}
	else if (prim1->type == PrimitiveType::CYLINDER && prim2->type == PrimitiveType::SPHERE)
	{
		//CylinderAndSphere(prim1, prim2);
	}
	else if (prim1->type == PrimitiveType::CYLINDER && prim2->type == PrimitiveType::BOX)
	{
		//CylinderAndBox(prim1, prim2);
	}
	else if (prim1->type == PrimitiveType::CYLINDER && prim2->type == PrimitiveType::CAPSULE)
	{
		
	}
	else if (prim1->type == PrimitiveType::CAPSULE && prim2->type == PrimitiveType::CAPSULE)
	{

	}
	else if (prim1->type == PrimitiveType::CAPSULE && prim2->type == PrimitiveType::CYLINDER)
	{
		
	}
	//else if (prim1->type == PrimitiveType::CAPSULE && prim2->type == PrimitiveType::PLANE)
	//{
	//	CapsuleAndPlane(prim1, prim2);
	//}
	else if (prim1->type == PrimitiveType::CAPSULE && prim2->type == PrimitiveType::BOX)
	{

	}
	else if (prim1->type == PrimitiveType::CAPSULE && prim2->type == PrimitiveType::SPHERE)
	{
		//CapsuleAndSphere(prim1, prim2);
	}
	else
	{
		std::cout << "WARNING: Types not found: " << (int)prim1->type << " & " << (int)prim2->type << std::endl;
	}
}

void CollisionFine::SphereAndSphere(Sphere* prim1, Sphere* prim2, const Vector3& position1, float radius1, const Vector3& position2, float radius2)
{
	Vector3 midline = position1 - position2;
	float size = midline.Magnitude();

	if (size <= 0.0f || size >= radius1 + radius2) return;

	Vector3 normal = midline * 1.0f / size;

	if (prim1->isStatic)
	{
		Contact contact(prim2, prim1);
		contact.normal = normal.Normalise().Inverse();
		contact.point = position1 + midline * 0.5f;
		contact.penetrationDepth = radius1 + radius2 - size;
		contacts.push_back(contact);
	}
	else
	{
		Contact contact(prim1, prim2);
		contact.normal = normal.Normalise();
		contact.point = position1 + midline * 0.5f;
		contact.penetrationDepth = radius1 + radius2 - size;
		contacts.push_back(contact);
	}
}

void CollisionFine::SphereAndPlane(Sphere* sphere, Plane* plane, const Vector3& spherePosition, float radius, const Vector3& planePosition, const Vector3& normal)
{
	float distance = normal.ScalarProduct(spherePosition) - radius - normal.ScalarProduct(planePosition);

	if (distance >= 0) return;

	Contact contact(sphere, plane);
	contact.normal = normal;
	contact.penetrationDepth = -distance;
	contact.point = spherePosition - normal * (distance + radius);
	contacts.push_back(contact);
}

void CollisionFine::SphereAndBox(Sphere* sphere, Box* box, Vector3& spherePosition, float radius)
{
	Vector3 halfSize = box->collisionVolume.halfSize;
	Vector3 relCentre = Mathe::MatrixInverse(box->collisionVolume.axisMat, spherePosition);

	if (abs(relCentre.x) - radius > halfSize.x
		|| abs(relCentre.y) - radius > halfSize.y
		|| abs(relCentre.z) - radius > halfSize.z)
		return; //not in contact

	Vector3 closestPoint;
	float distance;

	for (unsigned i = 0; i < 3; i++)
	{
		distance = relCentre[i];
		if (distance > halfSize[i]) distance = halfSize[i];
		if (distance < -halfSize[i]) distance = -halfSize[i];
		closestPoint[i] = distance;
	}

	distance = (closestPoint - relCentre).Magnitude();
	if (distance > radius * radius) return;	//not in contact
	
	Mathe::Transform(closestPoint, box->collisionVolume.axisMat);

	Vector3 normal = (spherePosition - closestPoint).Normalise();
	if (Mathe::IsVectorNAN(normal) || normal == Vector3()) return;

	if (sphere->isStatic)
	{
		Contact contact(box, sphere);
		contact.normal = normal.Inverse();
		contact.point = closestPoint;
		contact.penetrationDepth = radius - distance;
		contacts.push_back(contact);
	}
	else
	{
		Contact contact(sphere, box);
		contact.normal = normal;
		contact.point = closestPoint;
		contact.penetrationDepth = radius - distance;
		contacts.push_back(contact);
	}
}

float CollisionFine::PositionOnAxis(const Box* box, const Vector3& axis)
{
	Matrix4 axisMat = box->collisionVolume.axisMat;

	return
		box->collisionVolume.halfSize.x * abs(axis.ScalarProduct(Mathe::GetAxis(0, axisMat)))
		+ box->collisionVolume.halfSize.y * abs(axis.ScalarProduct(Mathe::GetAxis(1, axisMat)))
		+ box->collisionVolume.halfSize.z * abs(axis.ScalarProduct(Mathe::GetAxis(2, axisMat)));
}

void CollisionFine::BoxAndPlane(Box* box, Plane* plane, const Vector3& planePosition, const Vector3& normal)
{
	float boxDistance = (normal * box->collisionVolume.centre).Magnitude() - PositionOnAxis(box, normal);
	if (normal.y != 1)
		boxDistance = normal.ScalarProduct(box->collisionVolume.centre) - PositionOnAxis(box, normal);

	float planeOffset = planePosition.ScalarProduct(normal);

	if (boxDistance > planeOffset) return;

	ContactPoint contactPoints[4];
	unsigned numContacts = 0;
	float totalPenetration = 0.0f;

	for (uint16_t v = 0; v < 8; v++)
	{
		//distance from vertex to plane
		float distance = box->collisionVolume.vertices[v].ScalarProduct(normal);

		if (distance <= planeOffset + 0.01f)
		{
			contactPoints[numContacts].point = normal;
			contactPoints[numContacts].point *= distance - (normal.y == 1 ? (planePosition * normal).Magnitude() : planeOffset);
			contactPoints[numContacts].point += box->collisionVolume.vertices[v];
			contactPoints[numContacts].penetration = abs((normal.y == 1 ? (planePosition * normal).Magnitude() : planeOffset) - distance) + 0.01f;
			totalPenetration += contactPoints[numContacts].penetration;
			
			numContacts++;
			if (numContacts == 4) break;
		}
	}

	if (numContacts <= 0) return;
	else if (numContacts == 1)
	{
		Contact c(box, plane);
		c.point = contactPoints[0].point;
		c.penetrationDepth = contactPoints[0].penetration;
		c.normal = normal;
		contacts.push_back(c);
		return;
	}

	bool mergeContacts = false;

	if (mergeContacts)
	{
		Contact mergedContact(box, plane);
		mergedContact.normal = normal;
		for (uint16_t i = 0; i < numContacts; i++)
		{
			contactPoints[i].weighting = contactPoints[i].penetration / totalPenetration;
			mergedContact.point += contactPoints[i].point * contactPoints[i].weighting;
			mergedContact.penetrationDepth += contactPoints[i].penetration;
		}
		mergedContact.penetrationDepth /= (float)numContacts;
		contacts.push_back(mergedContact);
	}
	else
	{
		Contact contact(box, plane);
		contact.normal = normal;
		for (uint16_t i = 0; i < numContacts; i++)
		{
			contact.point = contactPoints[i].point;
			contact.penetrationDepth = contactPoints[i].penetration;
			contacts.push_back(contact);
		}
	}
}

//void CollisionFine::CylinderAndPlane(Primitive* cyl, Primitive* plane)
//{
//	//Top and bottom centre points
//	Vector3 cylEndT = cyl->collisionVolume.centre + (cyl->upDir * cyl->collisionVolume.length);
//	Vector3 cylEndB = cyl->collisionVolume.centre - (cyl->upDir * cyl->collisionVolume.length);
//
//	float planePosition = (float)plane->collisionVolume.centre.ScalarProduct(plane->collisionVolume.normal);
//
//	Vector3 cylToPlane = (plane->collisionVolume.normal.VectorProduct(cyl->upDir)).VectorProduct(cyl->upDir);
//
//	Vector3 points[] = {
//		cylEndT + (cylToPlane * cyl->radius),
//		cylEndT - (cylToPlane * cyl->radius),
//		cylEndB + (cylToPlane * cyl->radius),
//		cylEndB - (cylToPlane * cyl->radius)
//	};
//	float scalarProducts[] ={
//		(float)points[0].ScalarProduct(plane->collisionVolume.normal),
//		(float)points[1].ScalarProduct(plane->collisionVolume.normal),
//		(float)points[2].ScalarProduct(plane->collisionVolume.normal),
//		(float)points[3].ScalarProduct(plane->collisionVolume.normal)
//	};
//
//	if (scalarProducts[0] < planePosition
//		|| scalarProducts[1] < planePosition
//		|| scalarProducts[2] < planePosition
//		|| scalarProducts[3] < planePosition)
//	{
//		//COLLIDING
//
//		float dirScalarProduct = (float)plane->collisionVolume.normal.ScalarProduct(cyl->upDir);
//
//		//STANDING LIKE A COLUMN
//		if (abs(dirScalarProduct) < 1.01f && abs(dirScalarProduct) > 0.99f)	
//		{
//			//Three contacts required, each 120 degrees away from eachother about the centre
//			//Arbritrary points calculated
//			//THIS WILL INTRODUCE PROBLEMS IF THE PLANE'S NORMAL IS EVER NOT 0,1,0
//			Vector3 centre(cyl->collisionVolume.centre.x, planePosition, cyl->collisionVolume.centre.z);
//			float PI = 3.14159265359f;
//
//			Contact contact(cyl, plane);
//			contact.penetrationDepth = 0.00000001f;
//			contact.normal = plane->collisionVolume.normal;
//
//			contact.point = centre + Vector3((double)cyl->radius * (double)cos(0), 0, (double)cyl->radius * (double)sin(0));
//			contacts.push_back(contact);
//
//			contact.point = centre + Vector3((double)cyl->radius * (double)cos((2.0f * PI) / 3.0f), 0, (double)cyl->radius * (double)sin((2.0f * PI) / 3.0f));
//			contacts.push_back(contact);
//
//			contact.point = centre + Vector3((double)cyl->radius * (double)cos(-(2.0f * PI) / 3.0f), 0, (double)cyl->radius * (double)sin(-(2.0f * PI) / 3.0f));
//			contacts.push_back(contact);
//
//			return;
//		}
//		else
//		{
//			//FIRST CONTACT
//			float smallestDistance1 = 1000;
//			int closestIndex1 = 0;
//
//			for (int i = 0; i < 4; i++)
//			{
//				if ((float)abs(scalarProducts[i] - plane->collisionVolume.normal.Magnitude()) < smallestDistance1)
//				{
//					smallestDistance1 = (float)abs(scalarProducts[i] - plane->collisionVolume.normal.Magnitude());
//					closestIndex1 = i;
//				}
//			}
//			Contact contact(cyl, plane);
//			contact.point = points[closestIndex1];
//			contact.penetrationDepth = smallestDistance1;
//			contact.normal = plane->collisionVolume.normal;
//			contacts.push_back(contact);
//
//
//			//PARALLEL (rolling)
//			if (abs(dirScalarProduct) < 0.01f)
//			{
//				float smallestDistance2 = 1000;
//				int closestIndex2 = 0;
//
//				for (int i = 0; i < 4; i++)
//				{
//					if (abs(scalarProducts[i] - plane->collisionVolume.normal.Magnitude()) < smallestDistance2
//						&& i != closestIndex1)
//					{
//						smallestDistance2 = (float)abs(scalarProducts[i] - plane->collisionVolume.normal.Magnitude());
//						closestIndex2 = i;
//					}
//				}
//				Contact contact(cyl, plane);
//				contact.point = points[closestIndex2];
//				contact.penetrationDepth = smallestDistance2;
//				contact.normal = plane->collisionVolume.normal;
//				contacts.push_back(contact);
//			}
//
//		}
//	}
//	else
//	{
//		return;	//No contact
//	}
//}
//
//void CollisionFine::CylinderAndSphere(Primitive* cyl, Primitive* sphere)
//{
//	//Convert to cylinder basis (transform)
//	Vector3 relativeSpherePos = (sphere->collisionVolume.centre - cyl->collisionVolume.centre);// .Normalise();
//	Matrix4 cylTranspose = cyl->collisionVolume.axisMat.GetTranspose();
//	Mathe::Transform(relativeSpherePos, cylTranspose);
//
//	//NOT COLLIDING - actually calculated in AABB stage so sort of unnecessary here
//	if ((relativeSpherePos.y - sphere->collisionVolume.radius) > (cyl->collisionVolume.length / 2)
//		|| (relativeSpherePos.y + sphere->collisionVolume.radius) < -(cyl->collisionVolume.length / 2))
//	{
//		return;
//	}
//
//	//Y is along the centre line of the cyl therefore null
//	float pow1 = (float)pow(relativeSpherePos.x, 2);
//	float pow2 = (float)pow(relativeSpherePos.z, 2);
//	float distanceToCentreLine = pow1 + pow2;
//
//	//Could be colliding
//	if (distanceToCentreLine < pow(cyl->collisionVolume.radius + sphere->collisionVolume.radius, 2))
//	{
//		//COLLIDING ON CURVED FACE, therefore treat cylinder as a sphere
//		if (relativeSpherePos.y < (cyl->collisionVolume.length / 2)
//			&& relativeSpherePos.y > -(cyl->collisionVolume.length / 2))
//		{
//			Vector3 cylSphereRepresentation(0, relativeSpherePos.y, 0);
//			Mathe::Transform(cylSphereRepresentation, cyl->collisionVolume.axisMat);
//			SphereAndSphere(cyl, sphere, cylSphereRepresentation, cyl->collisionVolume.radius, sphere->collisionVolume.centre, sphere->collisionVolume.radius);
//			return;
//		}
//		//COLLIDING ON FLAT PLANES
//		else if ((relativeSpherePos.y - sphere->collisionVolume.radius) < (cyl->collisionVolume.length / 2)
//			&& (relativeSpherePos.y + sphere->collisionVolume.radius) > -(cyl->collisionVolume.length / 2))
//		{
//			double closestEndpoint = (abs(relativeSpherePos.y - cyl->collisionVolume.length) >
//				abs(relativeSpherePos.y + cyl->collisionVolume.length))
//				? cyl->collisionVolume.length
//				: -cyl->collisionVolume.length;
//
//			//if sphere x and z magnitude is greater than cyl radius,
//			//it is a edge collision, else it's colliding with the face
//
//			Vector3 contactPoint = Vector3(relativeSpherePos.x, closestEndpoint, relativeSpherePos.z);
//
//			if (distanceToCentreLine > pow(cyl->collisionVolume.radius, 2))
//			{
//				contactPoint = contactPoint.Normalise() * cyl->collisionVolume.radius;
//				Mathe::Transform(contactPoint, cyl->collisionVolume.axisMat); //back to world space
//				Vector3 normal = (sphere->collisionVolume.centre - contactPoint).Normalise();
//
//				Contact contact(cyl, sphere);
//				contact.point = contactPoint;
//				contact.normal = normal;
//				contact.penetrationDepth = 0;
//				contacts.push_back(contact);
//				return;
//			}
//			else
//			{
//				Mathe::Transform(contactPoint, cyl->collisionVolume.axisMat); //back to world space
//
//				Contact contact(cyl, sphere);
//				contact.point = contactPoint;
//				contact.normal = cyl->upDir;
//				contact.penetrationDepth = 0;
//				contacts.push_back(contact);
//				return;
//			}
//		}
//	}
//	//NOT COLLIDING
//	return;
//}
//
//void CollisionFine::CapsuleAndPlane(Primitive* cap, Primitive* plane)
//{
//	//VERY similar to cylinder plane, but checks for end points also necessary
//
//	//Top and bottom centre points
//	Vector3 capEndT = cap->collisionVolume.centre + (cap->upDir * cap->collisionVolume.length);
//	Vector3 capEndB = cap->collisionVolume.centre - (cap->upDir * cap->collisionVolume.length);
//
//	float planePosition = (float)plane->collisionVolume.centre.ScalarProduct(plane->collisionVolume.normal);
//
//	Vector3 capToPlane = (plane->collisionVolume.normal.VectorProduct(cap->upDir)).VectorProduct(cap->upDir);
//
//	Vector3 points[] = {
//		capEndT + (capToPlane * cap->radius),
//		capEndT - (capToPlane * cap->radius),
//		capEndB + (capToPlane * cap->radius),
//		capEndB - (capToPlane * cap->radius),
//		capEndT + (cap->upDir * cap->radius), 
//		capEndB - (cap->upDir * cap->radius)
//	};
//	float scalarProducts[] = {
//		(float)points[0].ScalarProduct(plane->collisionVolume.normal),
//		(float)points[1].ScalarProduct(plane->collisionVolume.normal),
//		(float)points[2].ScalarProduct(plane->collisionVolume.normal),
//		(float)points[3].ScalarProduct(plane->collisionVolume.normal),
//		(float)points[4].ScalarProduct(plane->collisionVolume.normal),
//		(float)points[5].ScalarProduct(plane->collisionVolume.normal)
//	};
//
//	if (scalarProducts[0] < planePosition
//		|| scalarProducts[1] < planePosition
//		|| scalarProducts[2] < planePosition
//		|| scalarProducts[3] < planePosition
//		|| scalarProducts[4] < planePosition
//		|| scalarProducts[5] < planePosition)
//	{
//		//COLLIDING
//
//		float dirScalarProduct = (float)plane->collisionVolume.normal.ScalarProduct(cap->upDir);
//
//		//STANDING LIKE A COLUMN
//		if (abs(dirScalarProduct) < 1.01f && abs(dirScalarProduct) > 0.99f)
//		{
//			//One contact point on a half-sphere
//			Vector3 centre(cap->collisionVolume.centre.x, planePosition, cap->collisionVolume.centre.z);
//
//			Contact contact(cap, plane);
//			contact.penetrationDepth = 0.00000001f;
//			contact.normal = plane->collisionVolume.normal;
//			contact.point = centre;
//			contacts.push_back(contact);
//
//			return;
//		}
//		//PARALLEL THEREFORE ROLLING
//		else if (abs(dirScalarProduct) < 0.01f)
//		{
//			float smallestDistance1 = 1000;
//			int closestIndex1 = 0;
//
//			for (int i = 0; i < 4; i++)
//			{
//				if ((float)abs(scalarProducts[i] - plane->collisionVolume.normal.Magnitude()) < smallestDistance1)
//				{
//					smallestDistance1 = (float)abs(scalarProducts[i] - plane->collisionVolume.normal.Magnitude());
//					closestIndex1 = i;
//				}
//			}
//			Contact contact(cap, plane);
//			contact.point = points[closestIndex1];
//			contact.penetrationDepth = smallestDistance1;
//			contact.normal = plane->collisionVolume.normal;
//			contacts.push_back(contact);
//
//			float smallestDistance2 = 1000;
//			int closestIndex2 = 0;
//
//			for (int i = 0; i < 4; i++)
//			{
//				if ((float)abs(scalarProducts[i] - plane->collisionVolume.normal.Magnitude()) < smallestDistance2
//					&& i != closestIndex1)
//				{
//					smallestDistance2 = (float)abs(scalarProducts[i] - plane->collisionVolume.normal.Magnitude());
//					closestIndex2 = i;
//				}
//			}
//			contact.point = points[closestIndex2];
//			contact.penetrationDepth = smallestDistance2;
//			contacts.push_back(contact);
//		}
//		else
//		{
//			Vector3 closestPoint = (abs(scalarProducts[5] - planePosition) > abs(scalarProducts[4] - planePosition))
//								 ? capEndT : capEndB;
//			closestPoint += capToPlane * cap->radius;
//			closestPoint.y = planePosition;
//			Contact contact(cap, plane);
//			contact.point = closestPoint;
//			contact.normal = plane->collisionVolume.normal;
//			contact.penetrationDepth = 0.000001f;
//			contacts.push_back(contact);
//		}
//	}
//	else
//	{
//		return;	//No contact
//	}
//}
//
//void CollisionFine::CapsuleAndSphere(Primitive* cap, Primitive* sphere)
//{
//	//Convert to capsule basis (transform)
//	Vector3 relativeSpherePos = (sphere->collisionVolume.centre - cap->collisionVolume.centre);
//	Matrix4 capTranspose = cap->collisionVolume.axisMat.GetTranspose();
//	Mathe::Transform(relativeSpherePos, capTranspose);
//
//	//NOT COLLIDING - actually calculated in AABB stage so sort of unnecessary here
//	//if (((float)relativeSpherePos.y - sphere->collisionVolume.radius) > (cap->collisionVolume.length / 2.0f) + cap->radius
//	//	|| ((float)relativeSpherePos.y + sphere->collisionVolume.radius) < -(cap->collisionVolume.length / 2.0f) - cap->radius)
//	//{
//	//	return;
//	//}
//
//	//Y is along the centre line of the cap therefore null
//	float pow1 = (float)pow(relativeSpherePos.x, 2);
//	float pow2 = (float)pow(relativeSpherePos.z, 2);
//	float distanceToCentreLine = pow1 + pow2;
//
//	//Could be colliding
//	if (distanceToCentreLine < pow(cap->collisionVolume.radius + cap->collisionVolume.radius + sphere->collisionVolume.radius, 2))
//	{
//		//COLLIDING ON CURVED FACE, therefore treat cylinder as a sphere
//		if (relativeSpherePos.z < (cap->collisionVolume.length / 2)
//			&& relativeSpherePos.z > -(cap->collisionVolume.length / 2))
//		{
//			Vector3 capSphereRepresentation(0, relativeSpherePos.y, 0);
//			Mathe::Transform(capSphereRepresentation, cap->collisionVolume.axisMat);
//			SphereAndSphere(cap, sphere, 
//				capSphereRepresentation, cap->collisionVolume.radius, 
//				sphere->collisionVolume.centre, sphere->collisionVolume.radius);
//			return;
//		}
//		//COLLIDING ON DOME
//		else
//		{
//			double closestEndpoint = (abs(relativeSpherePos.y - cap->collisionVolume.length) >
//				abs(relativeSpherePos.y + cap->collisionVolume.length))
//				? cap->collisionVolume.length / 2
//				: -cap->collisionVolume.length / 2;
//
//			Vector3 capSphereOrigin = Vector3(relativeSpherePos.x, closestEndpoint, relativeSpherePos.z);
//			SphereAndSphere(cap, sphere, 
//				capSphereOrigin, cap->collisionVolume.radius, 
//				sphere->collisionVolume.centre, sphere->collisionVolume.radius);
//		}
//	}
//	else
//	{
//		//NOT COLLIDING
//		return;
//	}
//}
