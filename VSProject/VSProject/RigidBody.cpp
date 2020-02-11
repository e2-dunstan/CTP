#include "RigidBody.h"

void RigidBody::Start()
{
	//AddImpulse(Vector3(1, 0, 0), 0.5);
	SetAwake(true);
}

bool RigidBody::PhysicsUpdate()
{
	if (!isAwake || isKinematic) return false;

	prevAcceleration = acceleration;
	prevAcceleration += forceAccumulation * inverseMass;
	
	prevVelocity = velocity;

	angularAcceleration = torqueAccumulation;
	Mathe::Transform(angularAcceleration, inverseInertiaTensorWorld);

	velocity += prevAcceleration * Global::deltaTime;
	angularVelocity += angularAcceleration * Global::deltaTime;

	//Drag
	if (linearDrag != 0)
		velocity *= pow(linearDrag, Global::deltaTime);
	if (angularDrag != 0)
		angularVelocity *= pow(angularDrag, Global::deltaTime);

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
	forceAccumulation = Vector3();
	torqueAccumulation = Vector3();

	if (canSleep && colliding)
	{
		double currentMotion = velocity.ScalarProduct(velocity) + angularVelocity.ScalarProduct(angularVelocity);
		double contingency = pow(0.5, Global::deltaTime);
		motion = contingency * motion + (1 - contingency) * currentMotion;
		if (motion < sleepThreshold)
		{
			timeMotionBelowSleepThreshold += Global::deltaTime;
			if (timeMotionBelowSleepThreshold > timeToSleep)
			{
				timeMotionBelowSleepThreshold = 0;
				SetAwake(false);
			}
		}
		else if (motion > sleepThreshold * 10.0)
		{
			timeMotionBelowSleepThreshold = 0;
			motion = sleepThreshold * 10.0;
		}
	}
}

void RigidBody::AddImpulse(Vector3 dir, double force)
{
	velocity += dir * (1 / inverseMass) * velocity.Magnitude() * 2.0 * force;
}

void RigidBody::AddVelocityChange(const Vector3& velChange)
{
	velocity += velChange;// *Global::deltaTime* Global::deltaTime;
}

void RigidBody::AddRotationChange(const Vector3& rotChange)
{
	angularVelocity += rotChange;// *Global::deltaTime* Global::deltaTime;
}

void RigidBody::SetTerminalSpeed()
{
	//Approximation assuming area = 1, which it is not.
	terminalSpeed = sqrt((Global::gravityMag * 2) / (inverseMass * Global::airDensity * linearDrag));
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

//bool RigidBody::IsAtRest()
//{
//	if (velocity.Magnitude() + angularVelocity.Magnitude() < 0.001)
//		return true;
//	else
//		return false;
//}

Vector3 RigidBody::GetPreviousVelocity()
{
	return prevVelocity;
}

Vector3 RigidBody::GetPreviousAcceleration()
{
	return prevAcceleration;
}
