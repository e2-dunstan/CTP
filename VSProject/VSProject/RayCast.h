#pragma once
#include "mathe.h"

struct Colour;

struct Ray
{
	// ray = origin + (intersection * direction)

	Ray() = default;
	Ray(const Vector3& _origin, const Vector3& _direction) : origin(_origin), direction(_direction) {}
	~Ray() = default;

	Vector3 origin = Vector3();
	Vector3 direction = Vector3();
	//If this ray is a segment, then intersection is essentially the magnitude
	//always > 0 so default of -1
	float intersection1 = -1;
	float intersection2 = -1;

	Vector3 IntersectionPoint();
	void Draw(const Colour& colour);
};

struct Tri;

namespace RayCast
{
	//bool Test(Primitive* prim, Ray& ray);
	bool TestTriangle(const Tri& tri, Matrix4& transform, Ray& ray);

	const float maxRayLength = 500.0f;

	bool TestPlane(const Vector3& centre, const Vector3& normal, const Vector3& size, Ray& ray);
	bool TestSphere(const Vector3& centre, const float radius, Ray& ray);
	bool TestBox(const Vector3& scale, const Matrix4& transform, Ray& ray);
	bool TestCylinder();

};
