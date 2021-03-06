#include "RigidBody.h"

void RigidBody::Start(Vector3 startingVelocity)
{
	SetTerminalSpeed();
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

	prevVelocity = velocity;
	velocity += Global::gravity * Global::deltaTime;

	//Clamp velocities
	velocity = velocity.Clamp(terminalSpeed * -1.0f, terminalSpeed);
	angularVelocity = angularVelocity.Clamp(terminalSpeedAngular * -1.0f, terminalSpeedAngular);

	//Drag
	if (linearDrag != 0)
		velocity *= 1.0f - linearDrag * Global::deltaTime;
	if (angularDrag != 0)
		angularVelocity *= 1.0f - angularDrag * Global::deltaTime;

	return true;
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
			motion = sleepThreshold * 2.0f;
		}
	}
	else timeMotionBelowSleepThreshold = 0;
}

void RigidBody::AddVelocityChange(const Vector3& velChange)
{
	if (Mathe::IsVectorNAN(velChange)) return;
	velocity += velChange;
}

void RigidBody::AddRotationChange(const Vector3& rotChange)
{
	if (Mathe::IsVectorNAN(rotChange)) return;

	else angularVelocity += rotChange;
}

Vector3 RigidBody::GetTrueAccelerationLastFrame()
{
	return (velocity - prevVelocity) / Global::deltaTime;
}

void RigidBody::SetTerminalSpeed()
{
	//Approximation assuming area = 1
	terminalSpeed = sqrtf((Global::gravityMag) / (inverseMass * Global::airDensity * linearDrag));
	terminalSpeedAngular = Mathe::ToRadians(terminalSpeed);
}

void RigidBody::SetAwake(const bool awake)
{
	if (awake)
	{
		isAwake = true;
		motion = sleepThreshold * 2.0f;
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

float RigidBody::GetMotion()
{
	float currentMotion = velocity.ScalarProduct(velocity) + angularVelocity.ScalarProduct(angularVelocity);
	//float contingency = powf(0.5f, Global::deltaTime);
	//float m = contingency * motion + (1 - contingency) * currentMotion;
	//if (currentMotion > 1.5f)
	//std::cout << currentMotion << ", " << m << std::endl;
	return currentMotion;
}
