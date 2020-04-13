#pragma once
#include "Vector.h"

class Ray
{
public:
	Ray() = default;
	Ray(const Vector3& origin, const Vector3& direction)
		: orig(origin), dir(direction) {}
	~Ray() = default;

	Vector3 Origin() const { return orig; }
	Vector3 Direction() const { return dir; }

	Vector3 At(double t) const { return orig + dir * t; }

private:
	Vector3 orig = Vector3();
	Vector3 dir = Vector3();
};