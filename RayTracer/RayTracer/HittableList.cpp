#include "HittableList.h"

bool HittableList::Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const
{
	HitRecord tempRec = rec;
	bool hitAnything = false;
	auto closest = tMax;

	for (const auto& object : objects)
	{
		if (object->Hit(r, tMin, closest, tempRec))
		{
			hitAnything = true;
			closest = tempRec.t;
			rec = tempRec;
		}
	}
	return hitAnything;
}
