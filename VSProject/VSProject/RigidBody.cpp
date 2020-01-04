#include "RigidBody.h"

void RigidBody::Start()
{
	AddImpulse(Vector3(1, 0, 0), 0.5);
}

void RigidBody::UpdatePhysics()
{
	if (isKinematic) return;

	if (useGravity)
	{
		velocity += (Global::gravity * Global::deltaTime * 0.01);
	}

	velocity = velocity.Clamp(0, terminalSpeed);
}

//void RigidBody::CalculateVelocity(Vector3& newPosition)
//{
//	//highly unlikely in final simulation
//	if (previousPosition == Vector3(0, -500, 0)) previousPosition = newPosition;
//
//	velocity = newPosition - previousPosition;
//	previousPosition = newPosition;
//}

void RigidBody::AddImpulse(Vector3 dir, double force)
{
	//change in velocity = final - initial
	//double changeInVel = (
	//	- ((1 / inverseMass) * velocity.Magnitude() * -1);

	velocity += dir * (1 / inverseMass) * velocity.Magnitude() * 2 * force;
}

void RigidBody::SetTerminalSpeed()
{
	//Approximation assuming area = 1, which it is not.
	terminalSpeed = sqrt((Global::gravityMag * 2) / (inverseMass * Global::airDensity * drag));
}
