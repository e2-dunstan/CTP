#pragma once
#include "BVHNode.h"
#include <deque>

//Bounding volume hierarchy
class BVH
{
public:
	BVH() = default;
	~BVH() = default;

	void AddBV(BoundingVolume& bv);

	void ConstructBottomUp();
	void ConstructTopDown();


private:

	void TopDownRecursive(BVHNode* parent, BoundingVolume objects[], int objCount);
	int PartitionObjects(BoundingVolume objects[], int objCount);

	std::vector<BoundingVolume> allBVs;
	std::unique_ptr<BVHNode> tree = std::make_unique<BVHNode>();
};