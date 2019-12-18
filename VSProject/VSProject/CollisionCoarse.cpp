#include "CollisionCoarse.h"


bool CollisionCoarse::Overlapping(const BoundingVolume& vol1, const BoundingVolume& vol2)
{
	if (vol1.min.x <= vol2.max.x
	 && vol2.min.x <= vol1.max.x
	 && vol1.min.y <= vol2.max.y
	 && vol2.min.y <= vol1.max.y
	 && vol1.min.z <= vol2.max.z
	 && vol2.min.z <= vol1.max.z)
	{
		return true;
	}
	else
	{
		return false;
	}
}
