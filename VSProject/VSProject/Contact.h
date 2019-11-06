#pragma once
#include "Primitive.h"

class Contact
{
public:
	Contact() = default;
	Contact(Primitive* prim1, Primitive* prim2);
	~Contact();

	Vector3 point;
	Vector3 normal;
	float penetrationDepth = 0;

	Primitive* bodies[2];
};