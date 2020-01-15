#include "RigidBody.h"

void RigidBody::Start()
{
	//AddImpulse(Vector3(1, 0, 0), 0.5);
}

void RigidBody::UpdatePhysics(bool colliding)
{
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
}

void RigidBody::AddImpulse(Vector3 dir, double force)
{
	velocity += dir * (1 / inverseMass) * velocity.Magnitude() * 2 * force;
}

void RigidBody::AddVelocityChange(const Vector3& velChange)
{
	velocity += velChange * Global::deltaTime;
}

void RigidBody::AddRotationChange(const Vector3& rotChange)
{
	angularVelocity += rotChange * Global::deltaTime;
}

void RigidBody::SetTerminalSpeed()
{
	//Approximation assuming area = 1, which it is not.
	terminalSpeed = sqrt((Global::gravityMag * 2) / (inverseMass * Global::airDensity * drag));
}

bool RigidBody::IsAtRest()
{
	if (velocity.Magnitude() + angularVelocity.Magnitude() < 0.001)
		return true;
	else
		return false;
}

Vector3 RigidBody::GetPreviousVelocity()
{
	return prevVelocity;
}
