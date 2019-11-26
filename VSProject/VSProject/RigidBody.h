#pragma once
#include "CollisionVolume.h"

//Will be used for collision response.
struct RigidBody
{
	RigidBody() = default;
	~RigidBody() = default;

	Vector3 velocity;

	//Inverse is used to avoid division by 0
	float inverseMass = 0;
	float drag = 0;
	float friction = 0;

	void CalculateVelocity(Vector3& newPosition);

private:
	Vector3 previousPosition = Vector3(0, -500, 0);
};