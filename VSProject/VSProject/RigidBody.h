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

	//Vector3 forceAccumulation = Vector3();
	//Vector3 torqueAccumulation = Vector3();

	float inverseMass = 1.0f / 1.0f;
	const float linearDrag = 0.75f;
	const float angularDrag = 0.2f;
	float friction = 0.2f;
	float bounciness = 0.0f; // 0 = not at all bouncy, 1 = no loss of energy so very bouncy

	bool isKinematic = false;
	bool useGravity = true;

	Matrix3 inverseInertiaTensor = Matrix3();
	Matrix3 inverseInertiaTensorWorld = Matrix3();

	void Start(Vector3 startingVelocity = Vector3());
	bool PhysicsUpdate();
	void EndPhysicsUpdate(bool colliding);

	void AddVelocityChange(const Vector3& velChange);
	void AddRotationChange(const Vector3& rotChange);

	void SetTerminalSpeed();

	void SetAwake(const bool awake);
	void EnableSleep(const bool _canSleep);
	bool isAwake = true;
	bool canSleep = true;

	Vector3 GetPreviousVelocity();
	double GetMotion();

private:
	Vector3 prevVelocity = Vector3();
	Vector3 prevAngularVelocity = Vector3();

	double motion = 0;
	double timeMotionBelowSleepThreshold = 0;
	double timeToSleep = 0.1;

	double terminalSpeed = 10000;

protected:
	const double sleepThreshold = 0.6;
};