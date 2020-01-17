#pragma once
#include "CollisionVolume.h"

//Will be used for collision response.
struct RigidBody
{
	RigidBody() = default;
	~RigidBody() = default;

	Vector3 velocity = Vector3();
	Vector3 angularVelocity = Vector3();
	Vector3 acceleration = Global::gravity;
	Vector3 angularAcceleration = Vector3();

	Vector3 forceAccumulation = Vector3();
	Vector3 torqueAccumulation = Vector3();

	//Inverse is used to avoid division by 0
	float inverseMass = 1;
	float linearDrag = 0.75;
	float angularDrag = 0.0; //between 0-1
	float friction = 0;

	bool isKinematic = false;
	bool useGravity = true;

	Matrix inverseInertiaTensor = Matrix(4,4);
	Matrix inverseInertiaTensorWorld = Matrix(4,4);

	void Start(); //not called
	bool PhysicsUpdate();
	void EndPhysicsUpdate();
	//void CalculateVelocity(Vector3& newPosition);
	void AddImpulse(Vector3 dir, double force = 1);

	void AddVelocityChange(const Vector3& velChange);
	void AddRotationChange(const Vector3& rotChange);

	void SetTerminalSpeed();

	//bool IsAtRest();
	void SetAwake(const bool awake);
	void EnableSleep(const bool _canSleep);
	bool isAwake = true;
	bool canSleep = true;

	Vector3 GetPreviousVelocity();
	Vector3 GetPreviousAcceleration();

private:
	Vector3 prevVelocity = Vector3();
	Vector3 prevAcceleration = Vector3();

	double motion = 0;

	double terminalSpeed = 10000;
	bool atRest = false;

	double sleepThreshold = 0.05;
};