#pragma once
#include "BoundingVolume.h"
#include "CollisionVolume.h"
#include "CollisionData.h"

class CollisionCoarse
{
public:
	CollisionCoarse() = default;
	~CollisionCoarse() = default;

	bool Overlapping(const BoundingVolume& vol1, const BoundingVolume& vol2);

private:

	float tolerance = 0.5f;
};