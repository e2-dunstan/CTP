#include "CollisionFine2.h"

void CollisionFine2::DetectContacts(Primitive* prim1, Primitive* prim2)
{
	if (prim1->type == Primitive::Type::PLANE || prim2->type == Primitive::Type::PLANE)
	{
		if (prim1->type == Primitive::Type::PLANE && prim2->type == Primitive::Type::PLANE)
		{
			return;
		}
		if ((prim1->type == Primitive::Type::PLANE && prim2->type == Primitive::Type::BOX)
			|| (prim2->type == Primitive::Type::PLANE && prim1->type == Primitive::Type::BOX))
		{

		}
		else if ((prim1->type == Primitive::Type::PLANE && prim2->type == Primitive::Type::SPHERE)
			|| (prim2->type == Primitive::Type::PLANE && prim1->type == Primitive::Type::SPHERE))
		{

		}
		else if ((prim1->type == Primitive::Type::PLANE && prim2->type == Primitive::Type::CAPSULE)
			|| (prim2->type == Primitive::Type::PLANE && prim1->type == Primitive::Type::CAPSULE))
		{

		}
		else if ((prim1->type == Primitive::Type::PLANE && prim2->type == Primitive::Type::CYLINDER)
			|| (prim2->type == Primitive::Type::PLANE && prim1->type == Primitive::Type::CYLINDER))
		{

		}
	}

	else if (prim1->type == Primitive::Type::BOX || prim2->type == Primitive::Type::BOX)
	{
		if (prim1->type == Primitive::Type::BOX && prim2->type == Primitive::Type::BOX)
		{
			//Contact* contact = sat->Test(prim1, prim2);
			//if (contact != nullptr) contacts.push_back(*contact);
		}
		else if ((prim1->type == Primitive::Type::BOX && prim2->type == Primitive::Type::SPHERE)
			|| (prim2->type == Primitive::Type::BOX && prim1->type == Primitive::Type::SPHERE))
		{

		}
		else if ((prim1->type == Primitive::Type::BOX && prim2->type == Primitive::Type::CAPSULE)
			|| (prim2->type == Primitive::Type::BOX && prim1->type == Primitive::Type::CAPSULE))
		{

		}
		else if ((prim1->type == Primitive::Type::BOX && prim2->type == Primitive::Type::CYLINDER)
			|| (prim2->type == Primitive::Type::BOX && prim1->type == Primitive::Type::CYLINDER))
		{

		}
	}

	else if (prim1->type == Primitive::Type::SPHERE || prim2->type == Primitive::Type::SPHERE)
	{
		if (prim1->type == Primitive::Type::SPHERE && prim2->type == Primitive::Type::SPHERE)
		{
			SphereSphere(prim1, prim2);
		}
		else if ((prim1->type == Primitive::Type::SPHERE && prim2->type == Primitive::Type::CAPSULE)
			|| (prim2->type == Primitive::Type::SPHERE && prim1->type == Primitive::Type::CAPSULE))
		{

		}
		else if ((prim1->type == Primitive::Type::SPHERE && prim2->type == Primitive::Type::CYLINDER)
			|| (prim2->type == Primitive::Type::SPHERE && prim1->type == Primitive::Type::CYLINDER))
		{

		}
	}

	//capsule and cylinder stuff
}

void CollisionFine2::SphereSphere(Primitive* prim1, Primitive* prim2)
{
	Vector3 midline = prim1->collisionVolume.centre - prim2->collisionVolume.centre;
	float size = (float)midline.Magnitude();

	if (size <= 0.0f || size >= prim1->radius + prim2->radius) return;

	Vector3 normal = midline * 1.0f / size;

	Contact contact(prim1, prim2);
	contact.normal = normal;
	contact.point = prim1->collisionVolume.centre + midline * 0.5f;
	contact.penetrationDepth = prim1->radius + prim2->radius - size;
	contacts.push_back(contact);
}
