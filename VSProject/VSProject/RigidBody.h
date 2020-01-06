#pragma once
#include "CollisionVolume.h"

//Will be used for collision response.
struct RigidBody
{
	RigidBody() = default;
	~RigidBody() = default;

	Vector3 velocity = Vector3();
	Vector3 angularVelocity = Vector3();

	//Inverse is used to avoid division by 0
	float inverseMass = 1;
	float drag = 0;
	float friction = 0;

	bool isKinematic = true;
	bool useGravity = false;

	Matrix inverseInertiaTensor;
	Matrix inverseInertiaTensorWorld;

	void Start(); //not called
	void UpdatePhysics();
	//void CalculateVelocity(Vector3& newPosition);
	void AddImpulse(Vector3 dir, double force = 1);

	void SetTerminalSpeed();

private:
	Vector3 prevVelocity = Vector3(0, 0, 0);
	double terminalSpeed = 10000;
};