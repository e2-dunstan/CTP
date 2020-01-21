#pragma once
#include "Primitive.h"


class BVHNode
{
public:
	BVHNode() = default;
	~BVHNode() = default;

	enum class Type
	{
		UNASSIGNED, LEAF, NODE
	};
	BVHNode::Type type = BVHNode::Type::UNASSIGNED;

	int numObjects = 0;
	//std::unique_ptr<BVHNode> children[2] = std::make_unique<BVHNode>()[2];

	std::unique_ptr<BoundingVolume> volume = std::make_unique<BoundingVolume>(); //encompases this node

	const bool IsAtBottomOfHierarchy();

	std::unique_ptr<BVHNode[]> children = std::make_unique<BVHNode[]>(2);
};