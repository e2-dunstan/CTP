#pragma once
#include "Shapes.h"

class Contact
{
public:
	Contact() = default;
	~Contact() = default;

	Vector3 point;
	Vector3 normal;
	float penetrationDepth;

	Primitive* bodies[2];
};