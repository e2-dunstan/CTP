#pragma once
#include "CollisionVolume.h"

//Will be used for collision response.
struct RigidBody
{
	RigidBody() = default;
	~RigidBody() = default;

	Vector3 velocity = Vector3();

	//Inverse is used to avoid division by 0
	float inverseMass = -1;
	float drag = 0;
	float friction = 0;

	void UpdatePhysics(const double& deltaTime);

	void CalculateVelocity(Vector3& newPosition);
	void AddImpulse(Vector3 force);

private:
	Vector3 previousPosition = Vector3(0, -500, 0);
};