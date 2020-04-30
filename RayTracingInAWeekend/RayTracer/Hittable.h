#pragma once
#include <memory>
#include "Ray.h"

class Material;

struct HitRecord
{
	Vector3 point = Vector3();
	Vector3 normal = Vector3();
	std::shared_ptr<Material> matPtr;
	double t = 0;
	bool frontFace = false;

	inline void SetFaceNormal(const Ray& r, const Vector3& outwardNormal)
	{
		frontFace = r.Direction().ScalarProduct(outwardNormal) < 0;
		normal = frontFace ? outwardNormal : (outwardNormal * -1.0);
	}
};

class Hittable
{
public:
	virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const = 0;
};