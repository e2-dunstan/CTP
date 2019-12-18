#pragma once
#include "Primitive.h"


class BVHNode
{
public:
	BVHNode() = default;
	~BVHNode() = default;

	std::unique_ptr<BVHNode> children[2] = std::make_unique<BVHNode>()[2];

	BoundingVolume volume; //encompases this node

	std::unique_ptr<Primitive> primitive = std::make_unique<Primitive>(); //prim at this node

	const bool IsAtBottomOfHierarchy();

private:
};