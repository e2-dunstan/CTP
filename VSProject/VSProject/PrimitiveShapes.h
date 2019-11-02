#pragma once
#include "Primitive.h"

class Sphere : public Primitive
{
public:
	Sphere() = default;
	~Sphere() = default;

	float radius;
};

class Box : public Primitive
{
public:
	Box() = default;
	~Box() = default;

	Vector3 size;
};

class Plane : public Primitive
{
public:
	Plane() = default;
	~Plane() = default;

	Vector3 normal;
};