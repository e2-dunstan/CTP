#pragma once
#include "Primitive.h"

//this class is placeholder

struct Contact
{
	Contact() = default;
	Contact(Primitive* prim1, Primitive* prim2);
	~Contact();

	Vector3 point;
	Vector3 normal;
	float penetrationDepth = 0;

	Primitive* body1;
	Primitive* body2;

};