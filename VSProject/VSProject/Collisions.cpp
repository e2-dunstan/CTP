#include "Collisions.h"

void Collisions::DetectContacts(Primitive* rb1, Primitive* rb2, CollisionData* data)
{
	if (rb1->type == Primitive::Type::SPHERE && rb2->type == Primitive::Type::SPHERE)
		SphereAndSphere(rb1, rb2, Vector3(), 0, Vector3(), 0);	//replace with proper variables when spheres implemented
}

void Collisions::SphereAndSphere(Primitive* prim1, Primitive* prim2, Vector3 position1, float radius1, Vector3 position2, float radius2)
{
	Vector3 midline = position1 - position2;
	float size = midline.Magnitude();

	if (size <= 0.0f || size >= radius1 + radius2)
	{
		return;
	}

	Vector3 normal = midline * 1.0f / size;

	Contact* contact = data->contacts.get();
	contact->normal = normal;
	contact->point = position1 + midline * 0.5f;
	contact->penetrationDepth = radius1 + radius2 - size;
	contact->bodies[0] = prim1;
	contact->bodies[1] = prim2;
}
