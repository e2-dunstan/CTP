#pragma once
#include "CollisionVolume.h"

class RigidBody
{
public:
	RigidBody() = default;
	~RigidBody() = default;

	bool updateTransforms = false;

	float mass = 0;
	float drag = 0;
	float friction = 0;

};