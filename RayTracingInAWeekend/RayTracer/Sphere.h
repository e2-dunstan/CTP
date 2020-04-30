#pragma once
#include "Hittable.h"

class Sphere : public Hittable
{
public:
	Sphere() = default;
	Sphere(Vector3 c, double r, std::shared_ptr<Material> m) 
		: centre(c), radius(r), matPtr(m) {};
	~Sphere() = default;

	bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override;

	Vector3 centre = Vector3();
	double radius = 0;
	std::shared_ptr<Material> matPtr;
};