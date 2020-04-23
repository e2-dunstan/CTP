#include "RigidBody.h"

void RigidBody::Start(Vector3 startingVelocity)
{
	AddVelocityChange(startingVelocity);
	SetAwake(true);
}

bool RigidBody::PhysicsUpdate()
{
	if (!isAwake || isKinematic)
	{
		velocity = Vector3();
		angularVelocity = Vector3();
		return false;
	}
	
	acceleration = Global::gravity;
	acceleration += forceAccumulation * inverseMass;
	angularAcceleration = torqueAccumulation;
	Mathe::Transform(angularAcceleration, inverseInertiaTensorWorld);

	prevVelocity = velocity;
	velocity += acceleration * Global::deltaTime;
	angularVelocity += angularAcceleration * Global::deltaTime;

	//Clamp velocities
	velocity = velocity.Clamp(terminalSpeed * -1.0, terminalSpeed);
	//angularVelocity = angularVelocity.Clamp(terminalSpeed * -1.0, terminalSpeed);

	//Drag
	if (linearDrag != 0)
		velocity *= 1.0f - linearDrag * Global::deltaTime;
	if (angularDrag != 0)
		angularVelocity *= 1.0f - angularDrag * Global::deltaTime;

	forceAccumulation = Vector3();
	torqueAccumulation = Vector3();

	return true;

	/*
	if (isKinematic) return;

	prevVelocity = velocity;

	if (useGravity)
	{
		velocity += Global::gravity * Global::deltaTime;
	}
	velocity = velocity.Clamp(0, terminalSpeed);

	if (isnan(velocity.x) || isnan(velocity.y) || isnan(velocity.z))
	{
		velocity = Vector3();
	}

	if (angularVelocity.Magnitude() > 0)
	{
		angularVelocity *= 1 - angularDrag;
		if (angularVelocity.Magnitude() < 0.001)
		{
			angularVelocity = Vector3();
		}
	}
	*/
}

void RigidBody::EndPhysicsUpdate(bool colliding = false)
{
	if (canSleep && colliding)
	{
		motion = GetMotion();
		if (motion < sleepThreshold)
		{
			timeMotionBelowSleepThreshold += Global::deltaTime;
			if (timeMotionBelowSleepThreshold > timeToSleep)
			{
				timeMotionBelowSleepThreshold = 0;
				SetAwake(false);
				//std::cout << "Sleeping... " << motion << std::endl;
			}
		}
		else if (motion > sleepThreshold * 2.0)
		{
			timeMotionBelowSleepThreshold = 0;
			motion = sleepThreshold * 2.0;
		}
	}
}

void RigidBody::AddVelocityChange(const Vector3& velChange)
{
	if (Mathe::IsVectorNAN(velChange)) return;

	velocity += velChange;
}

void RigidBody::AddRotationChange(const Vector3& rotChange, bool convertToRadians)
{
	if (Mathe::IsVectorNAN(rotChange)) return;

	if (convertToRadians)
		angularVelocity += Vector3(
			Mathe::ToRadians(rotChange.x),
			Mathe::ToRadians(rotChange.y),
			Mathe::ToRadians(rotChange.z)
		);
	else angularVelocity += rotChange;
}

void RigidBody::SetTerminalSpeed()
{
	//Approximation assuming area = 1, which it is not.
	terminalSpeed = sqrt((Global::gravityMag) / (inverseMass * Global::airDensity * linearDrag));
}

void RigidBody::SetAwake(const bool awake)
{
	if (awake)
	{
		isAwake = true;
		motion = sleepThreshold * 2.0;
		//add motion here
	}
	else
	{
		isAwake = false;
		velocity = Vector3();
		angularVelocity = Vector3();
	}
}

void RigidBody::EnableSleep(const bool _canSleep)
{
	canSleep = _canSleep;
	if (!canSleep && !isAwake) SetAwake(true);
}

Vector3 RigidBody::GetPreviousVelocity()
{
	return prevVelocity;
}

double RigidBody::GetMotion()
{
	double currentMotion = velocity.ScalarProduct(velocity) + angularVelocity.ScalarProduct(angularVelocity);
	double contingency = pow(0.5, Global::deltaTime);
	return contingency * motion + (1 - contingency) * currentMotion;
}
