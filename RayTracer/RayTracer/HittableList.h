#pragma once
#include "Hittable.h"
#include <memory>
#include <vector>

class HittableList : public Hittable
{
public:
	HittableList() = default;
	HittableList(std::shared_ptr<Hittable> object) { Add(object); }
	~HittableList() = default;

	void Clear() { objects.clear(); }
	void Add(std::shared_ptr<Hittable> object) { objects.push_back(object); }

	bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override;


	std::vector<std::shared_ptr<Hittable>> objects;
};