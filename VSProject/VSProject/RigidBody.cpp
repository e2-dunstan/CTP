#include "RigidBody.h"

void RigidBody::UpdatePhysics(const double& deltaTime)
{
	//How does this work with calculate velocity?
	//if (drag != 0)
	//{
	//	velocity *= drag * deltaTime;
	//}
}

void RigidBody::CalculateVelocity(Vector3& newPosition)
{
	//highly unlikely in final simulation
	if (previousPosition == Vector3(0, -500, 0)) previousPosition = newPosition;

	velocity = newPosition - previousPosition;
	previousPosition = newPosition;
}

void RigidBody::AddImpulse(Vector3 force)
{
	velocity += force;
}
