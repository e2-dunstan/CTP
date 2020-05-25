#pragma once
#include "CollisionVolume.h"
#include "Material.h"

//Will be used for collision response.
struct RigidBody
{
	friend class CollisionResolution;

public:
	RigidBody() = default;
	~RigidBody() = default;

	Vector3 velocity = Vector3();
	Vector3 angularVelocity = Vector3();

	Vector3 acceleration = Global::gravity;

	float inverseMass = 1.0f / 1.0f;

	Material material = Material::WOOD;
	float bounciness = 0.0f;

	Matrix3 inverseInertiaTensor = Matrix3();
	Matrix3 inverseInertiaTensorWorld = Matrix3();

	void Start(Vector3 startingVelocity = Vector3());
	bool PhysicsUpdate();
	void EndPhysicsUpdate(bool colliding);

	void AddVelocityChange(const Vector3& velChange);
	void AddRotationChange(const Vector3& rotChange);

	void SetAwake(const bool awake);
	void EnableSleep(const bool _canSleep);
	bool isKinematic = false;
	bool useGravity = true;
	bool isAwake = true;

	Vector3 GetTrueAccelerationLastFrame();

private:

	void SetTerminalSpeed();

	Vector3 prevVelocity = Vector3();
	Vector3 prevAngularVelocity = Vector3();

	const float linearDrag = 0.5f;
	const float angularDrag = 0.5f;

	float motion = 0;
	float timeMotionBelowSleepThreshold = 0;
	float timeToSleep = 0.0f;
	bool canSleep = true;

	float terminalSpeed = 10000;
	float terminalSpeedAngular = 1000;

protected:

	float GetMotion();
	const float sleepThreshold = 0.08f;
};