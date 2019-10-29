#pragma once
#include "mathe.h"


struct OctTreeNode
{
	Vector3 position;
	//OctTreeNode children[8];

	unsigned int GetChildIndex(const Vector3& objectOrigin)
	{
		unsigned int index = 0;
		if (objectOrigin.x > position.x) index += 1;
		if (objectOrigin.y > position.y) index += 2;
		if (objectOrigin.z > position.z) index += 4;
		return index;
	}
};

