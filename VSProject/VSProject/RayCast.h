#pragma once
#include "mathe.h"

struct Primitive;

struct Ray
{
	// ray = origin + (intersection * direction)

	Ray() = default;
	Ray(Vector3 _origin, Vector3 _direction) : origin(_origin), direction(_direction) {}
	~Ray() = default;

	Vector3 origin = Vector3();
	Vector3 direction = Vector3();
	//If this ray is a segment, then intersection is essentially the magnitude
	//always > 0 so default of -1
	float intersection1 = -1;
	float intersection2 = -1;

	Vector3 IntersectionPoint()
	{
		direction = direction.Normalise();

		if (intersection1 == -1) return origin + (direction * 100.0f);

		Vector3 pointA = origin + (direction * intersection1);
		if (intersection2 == -1) return pointA;

		Vector3 pointB = origin + (direction * intersection2);
		return (pointA.Magnitude() < pointB.Magnitude()) ? pointA : pointB;
	}

	bool draw = true;
};

class RayCast
{
public:
	RayCast() = default;
	~RayCast() = default;

	bool Test(const Primitive& prim, Ray& ray);
	const float maxRayLength = 500.0f;

private:
	bool TestPlane(const Vector3& centre, const Vector3& normal, const Vector3& size, Ray& ray);
	bool TestSphere(const Vector3& centre, const float radius, Ray& ray);
	bool TestBox(const Vector3& scale, const Matrix& transform, Ray& ray);


};
