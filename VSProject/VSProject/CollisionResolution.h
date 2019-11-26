#pragma once
#include "CollisionData.h"

class CollisionResolution
{
public:
	CollisionResolution() = default;
	~CollisionResolution() = default;

	void Resolve(Contact& contact);

};