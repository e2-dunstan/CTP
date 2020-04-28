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
	void AddRotationChange(const Vector3& rotChange, bool convertToRadians = false);

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

	double motion = 0;
	double timeMotionBelowSleepThreshold = 0;
	double timeToSleep = 0.1;
	bool canSleep = true;

	double terminalSpeed = 10000;
	double terminalSpeedAngular = 1000;

protected:

	double GetMotion();
	const double sleepThreshold = 1.2;
};