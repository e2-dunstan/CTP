#include "CollisionFine.h"

void CollisionFine::DetectContacts(Primitive* prim1, Primitive* prim2)
{
	data->contacts.clear();

	if (prim1->type == Primitive::Type::SPHERE && prim2->type == Primitive::Type::SPHERE)
	{
		SphereAndSphere(prim1, prim2,
			prim1->collisionVolume->centre, prim1->collisionVolume->radius,
			prim2->collisionVolume->centre, prim2->collisionVolume->radius);
	}
	if (prim1->type == Primitive::Type::SPHERE && prim2->type == Primitive::Type::PLANE)
	{
		SphereAndPlane(prim1, prim2,
			prim1->collisionVolume->centre, prim1->collisionVolume->radius,
			prim2->collisionVolume->centre, prim2->collisionVolume->normal);
	}
	if (prim1->type == Primitive::Type::PLANE && prim2->type == Primitive::Type::SPHERE)
	{
		SphereAndPlane(prim2, prim1,
			prim2->collisionVolume->centre, prim2->collisionVolume->radius,
			prim1->collisionVolume->centre, prim1->collisionVolume->normal);
	}
	if (prim1->type == Primitive::Type::BOX && prim2->type == Primitive::Type::PLANE)
	{
		BoxAndPlane(prim1, prim2, prim2->collisionVolume->centre, prim2->collisionVolume->normal);
	}
	if (prim1->type == Primitive::Type::PLANE && prim2->type == Primitive::Type::BOX)
	{
		BoxAndPlane(prim2, prim1, prim1->collisionVolume->centre, prim1->collisionVolume->normal);
	}
	if (prim1->type == Primitive::Type::SPHERE && prim2->type == Primitive::Type::BOX)
	{
		SphereAndBox(prim1, prim2, prim1->collisionVolume->centre, prim1->collisionVolume->radius);
	}
	if (prim1->type == Primitive::Type::BOX && prim2->type == Primitive::Type::SPHERE)
	{
		SphereAndBox(prim2, prim1, prim2->collisionVolume->centre, prim2->collisionVolume->radius);
	}
	if (prim1->type == Primitive::Type::BOX && prim2->type == Primitive::Type::BOX)
	{
		BoxAndBox(prim1, prim2);
	}
}

void CollisionFine::SphereAndSphere(Primitive* prim1, Primitive* prim2, Vector3 position1, float radius1, Vector3 position2, float radius2)
{
	if (data->contactsLeft <= 0) return;

	Vector3 midline = position1 - position2;
	float size = midline.Magnitude();

	if (size <= 0.0f || size >= radius1 + radius2)
	{
		return;
	}

	Vector3 normal = midline * 1.0f / size;

	Contact* contact = new Contact(prim1, prim2);
	contact->normal = normal;
	contact->point = position1 + midline * 0.5f;
	contact->penetrationDepth = radius1 + radius2 - size;

	data->contacts.push_back(contact);
}

void CollisionFine::SphereAndPlane(Primitive* sphere, Primitive* plane, Vector3 spherePosition, float radius, Vector3 planePosition, Vector3 normal)
{
	if (data->contactsLeft <= 0) return;

	float distance = (normal * spherePosition).Magnitude() - radius - (normal * planePosition).Magnitude();
	std::cout << "Distance between sphere and plane: " << distance << std::endl;

	if (distance >= 0) return;

	Contact* contact = new Contact(sphere, plane);
	contact->normal = normal;
	contact->penetrationDepth = -distance;
	contact->point = spherePosition - normal * (distance + radius);

	data->contacts.push_back(contact);
}

//Separating axis theorem
void CollisionFine::SphereAndBox(Primitive* sphere, Primitive* box, Vector3 spherePosition, float radius)
{
	Vector3 halfSize = box->collisionVolume->halfSize;

	if (abs(spherePosition.x) - radius > halfSize.x
		|| abs(spherePosition.y) - radius > halfSize.y
		|| abs(spherePosition.z) - radius > halfSize.z)
		return; //not in contact

	Vector3 closestPoint;
	float distance;

	distance = spherePosition.x;
	if (distance > halfSize.x) distance = halfSize.x;
	if (distance < -halfSize.x) distance = -halfSize.x;
	closestPoint.x = distance;

	distance = spherePosition.y;
	if (distance > halfSize.y) distance = halfSize.y;
	if (distance < -halfSize.y) distance = -halfSize.y;
	closestPoint.y = distance;

	distance = spherePosition.z;
	if (distance > halfSize.z) distance = halfSize.z;
	if (distance < -halfSize.z) distance = -halfSize.z;
	closestPoint.z = distance;

	distance = (closestPoint - spherePosition).Magnitude();
	if (distance > radius * radius) return;	//not in contact

	Contact* contact = new Contact(sphere, box);
	contact->normal = (spherePosition - closestPoint).Normalise();
	contact->point = closestPoint;
	contact->penetrationDepth = radius - sqrt(distance);

	data->contacts.push_back(contact);
}

float CollisionFine::PositionOnAxis(Primitive* box, Vector3 axis)
{
	return
		box->collisionVolume->halfSize.x * abs(axis.ScalarProduct(mathe->GetColumnVectorFromMatrix(0, box->transform)))
		+ box->collisionVolume->halfSize.y * abs(axis.ScalarProduct(mathe->GetColumnVectorFromMatrix(1, box->transform)))
		+ box->collisionVolume->halfSize.z * abs(axis.ScalarProduct(mathe->GetColumnVectorFromMatrix(2, box->transform)));
}

bool CollisionFine::BoxesOverlapOnAxis(Primitive* box1, Primitive* box2, Vector3 toCentre, Vector3 axis)
{
	float box1AxisPos = PositionOnAxis(box1, axis);
	float box2AxisPos = PositionOnAxis(box2, axis);

	float distance = abs(toCentre.ScalarProduct(axis));// -data->tolerance;
	
	//std::cout << "box1: " << box1AxisPos << ", box2: " << box2AxisPos << std::endl;
	
	return (distance < box1AxisPos + box2AxisPos);
}

//Separating axis theorem
void CollisionFine::BoxAndBox(Primitive* box1, Primitive* box2)
{
	Vector3 toCentre = box2->collisionVolume->centre - box1->collisionVolume->centre;
	//std::vector<Vector3> axes1 = GetBoxAxes(box1);
	//std::vector<Vector3> axes2 = GetBoxAxes(box2);

	//check all axes on both shapes
	//for (int n1 = 0; n1 < 3; n1++)
	//{
	//	if (!BoxesOverlapOnAxis(box1, box2, toCentre, axes1[n1])
	//		|| !BoxesOverlapOnAxis(box1, box2, toCentre, axes2[n1]))
	//	{
	//		std::cout << "no overlap" << std::endl;
	//		return;
	//	}
	//	for (int n2 = 0; n2 < 3; n2++)
	//	{
	//		Vector3 vecProd = axes1[n1] % axes2[n2];
	//		if (!BoxesOverlapOnAxis(box1, box2, toCentre, vecProd))
	//		{
	//			std::cout << "no overlap on cross product" << std::endl;
	//			return;
	//		}
	//	}
	//}
	for (int n1 = 0; n1 < 3; n1++)
	{
		if (!BoxesOverlapOnAxis(box1, box2, toCentre, mathe->GetColumnVectorFromMatrix(n1, box1->transform))
			|| !BoxesOverlapOnAxis(box1, box2, toCentre, mathe->GetColumnVectorFromMatrix(n1, box2->transform)))
		{
			std::cout << "no overlap" << std::endl;
			return;
		}
		for (int n2 = 0; n2 < 3; n2++)
		{
			Vector3 vecProd = mathe->GetColumnVectorFromMatrix(n1, box1->transform) % mathe->GetColumnVectorFromMatrix(n2, box2->transform);
			if (!BoxesOverlapOnAxis(box1, box2, toCentre, vecProd))
			{
				std::cout << "no overlap on cross product" << std::endl;
				return;
			}
		}
	}
	std::cout << "boxes colliding" << std::endl;
}

std::vector<Vector3> CollisionFine::GetBoxAxes(Primitive* box1)
{
	std::vector<Vector3> axes;
	for (int i = 0; i < box1->vertices.size(); i++)
	{
		Vector3 normal = box1->vertices[i].normal;
		if (axes.size() <= 0 || std::find(axes.begin(), axes.end(), normal) == axes.end())
		{
			//if (normal.x < 0 || normal.y < 0 || normal.z < 0) continue;
			//else
			//{
				axes.push_back(normal);
			//}
		}
	}
	for (int n = 0; n < axes.size(); n++)
	{
		mathe->Transform(axes[n], box1->transform);
	}

	return axes;
}

void CollisionFine::BoxAndPlane(Primitive* box, Primitive* plane, Vector3 planePosition, Vector3 normal)
{
	//Intersection test - DONT THINK IT WORKS???
	Vector3 boxCentre = box->collisionVolume->centre;
	int projectedRadius = (boxCentre.x * normal.x + boxCentre.y * normal.y + boxCentre.z * normal.z) * 10;
	// Work out how far the box is from the origin
	int boxDistance = ((normal * boxCentre).Magnitude() * 10) - projectedRadius;
	float planeOffset = planePosition.x * normal.x + planePosition.y * normal.y + planePosition.z * normal.z;

	if (((float)boxDistance / 10) > planeOffset) return;


	for (int v = 0; v < 8; v++)
	{
		//distance from vertex to plane
		float distance = (box->collisionVolume->vertices[v] * normal).Magnitude();

		if (distance <= planeOffset + data->tolerance)//(planePosition % normal).Magnitude()) //* data->tolerance
		{
			Contact* contact = new Contact(box, plane);
			contact->point = normal;
			contact->point *= distance - (planePosition * normal).Magnitude();
			contact->point += box->collisionVolume->vertices[v];
			contact->normal = normal;
			contact->penetrationDepth = (planePosition * normal).Magnitude() - distance + data->tolerance;

			data->contacts.push_back(contact);
		}
	}
}
