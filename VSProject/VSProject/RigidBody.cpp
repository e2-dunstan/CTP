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
	else
	{
		velocity = Vector3();
	}

	velocity = velocity.Clamp(0, terminalSpeed);
}

void RigidBody::AddImpulse(Vector3 dir, double force)
{
	//change in velocity = final - initial
	//double changeInVel = (
	//	- ((1 / inverseMass) * velocity.Magnitude() * -1);

	velocity += dir * (1 / inverseMass) * velocity.Magnitude() * 2 * force;
}

void RigidBody::AddVelocityChange(const Vector3& velChange)
{
	//Vector3 change = velChange;
	//change.DebugOutput();
	velocity += velChange * Global::deltaTime;
}

void RigidBody::AddRotationChange(const Vector3& rotChange)
{
	angularVelocity += rotChange;
}

void RigidBody::SetTerminalSpeed()
{
	//Approximation assuming area = 1, which it is not.
	terminalSpeed = sqrt((Global::gravityMag * 2) / (inverseMass * Global::airDensity * drag));
}
