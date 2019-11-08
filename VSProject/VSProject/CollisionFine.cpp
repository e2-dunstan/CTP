#include "CollisionFine.h"

void CollisionFine::DetectContacts(Primitive* prim1, Primitive* prim2)
{
	data->contacts.clear();

	//std::string str1 = "???";
	//std::string str2 = "???";
	//if (prim1->type == Primitive::Type::PLANE)
	//{
	//	str1 = "plane";
	//}
	//if (prim2->type == Primitive::Type::BOX)
	//{
	//	str2 = "box";
	//}

	//std::cout << "detecting contacts " << str1 << ", " << str2 << std::endl;

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