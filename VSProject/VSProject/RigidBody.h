#pragma once
#include "CollisionVolume.h"

//Will be used for collision response.
struct RigidBody
{
	friend class CollisionResolution2;

public:
	RigidBody() = default;
	~RigidBody() = default;

	Vector3 velocity = Vector3();
	Vector3 angularVelocity = Vector3();
	Vector3 acceleration = Global::gravity;
	Vector3 angularAcceleration = Vector3();

	Vector3 forceAccumulation = Vector3();
	Vector3 torqueAccumulation = Vector3();

	//Inverse is used to avoid division by 0
	float inverseMass = 1.0f / 1.0f;
	//Drag is a percentage so 1.0f is a lot and 0.0f is none
	const float linearDrag = 0.75f;
	const float angularDrag = 0.05f;
	float friction = 1.0f;
	float bounciness = 0.0f; // 0 = not at all bouncy, 1 = no loss of energy so very bouncy

	bool isKinematic = false;
	bool useGravity = true;

	Matrix3 inverseInertiaTensor = Matrix3();
	Matrix3 inverseInertiaTensorWorld = Matrix3();

	void Start(Vector3 startingVelocity = Vector3());
	bool PhysicsUpdate();
	void EndPhysicsUpdate(bool colliding);
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
	//Vector3 GetPreviousAcceleration();
	double GetMotion();

private:
	Vector3 prevVelocity = Vector3();
	Vector3 prevAcceleration = Vector3();

	double motion = 0;
	double timeMotionBelowSleepThreshold = 0;
	double timeToSleep = 1.0;

	double terminalSpeed = 10000;

protected:
	const double sleepThreshold = 0.5;
};