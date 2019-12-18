#pragma once
#include "BVHNode.h"

//Bounding volume hierarchy
class BVH
{
public:
	BVH() = default;
	~BVH() = default;


	void Construct(BoundingVolume& );

private:

	std::vector<BoundingVolume> boundingVolumes;
};

struct QueryContact
{
	std::unique_ptr<Primitive> primitives[2] = std::make_unique<Primitive>()[];
};
