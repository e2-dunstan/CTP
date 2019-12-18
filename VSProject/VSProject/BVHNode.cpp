#include "BVHNode.h"

const bool BVHNode::IsAtBottomOfHierarchy()
{
	return (primitive != NULL);
}
