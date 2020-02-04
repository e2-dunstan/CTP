#include "BVH.h"

void BVH::ConstructBottomUp()
{
	int numBVs = allBVs.size();
}

void BVH::ConstructTopDown()
{
	int numBVs = allBVs.size();

	TopDownRecursive(tree.get(), allBVs.data(), numBVs);
}

void BVH::TopDownRecursive(BVHNode* parent, BoundingVolume objects[], int objCount)
{
	if (objCount <= 0) return;

	const int minObjectsPerLeaf = 1;

	BVHNode* newNode = new BVHNode();

	if (objCount <= minObjectsPerLeaf)
	{
		newNode->type = BVHNode::Type::LEAF;
		newNode->numObjects = objCount;
		newNode->volume = std::make_unique<BoundingVolume>(objects[0]);
	}
	else
	{
		newNode->type = BVHNode::Type::NODE;
		int k = PartitionObjects(&objects[0], objCount);
		TopDownRecursive(&(newNode->children[0]), &objects[0], k);
		TopDownRecursive(&(newNode->children[1]), &objects[k], objCount - k);
	}
}

//See section 6.2.1.1 in RTCD
int BVH::PartitionObjects(BoundingVolume objects[], int objCount)
{
	return 0;
}

void BVH::AddBV(BoundingVolume& bv)
{
	allBVs.push_back(bv);
}
