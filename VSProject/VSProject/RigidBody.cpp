#include "RigidBody.h"

void RigidBody::CalculateVelocity(Vector3& newPosition)
{
	//highly unlikely in final simulation
	if (previousPosition == Vector3(0, -500, 0)) previousPosition = newPosition;

	velocity = newPosition - previousPosition;
	previousPosition = newPosition;
}
