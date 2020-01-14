#pragma once
#include "CollisionData.h"

class CollisionResolution2
{
public:
	CollisionResolution2() = default;
	~CollisionResolution2() = default;

	void ResolveContacts(const std::vector<Contact>& contacts);

private:

	void InitialiseContacts(const std::vector<Contact>& contacts);

	void CalculatePenetrationResolution(const std::vector<Contact>& contacts);
	void CalculateVelocityResolution(const std::vector<Contact>& contacts);

	void ApplyPositionChange(const Contact& contact, Vector3 linear1, Vector3 linear2, Vector3 angular1, Vector3 angular2);
	void ApplyVelocityChange(const Contact& contact, Vector3 vel1, Vector3 vel2, Vector3 rot1, Vector3 rot2);

	void CalculateFrictionlessImpulse(Matrix* inverseInertiaTensor);
	void CalculateFrictionImpulse(Matrix* inverseInertiaTensor);

	unsigned velocityIterations = 0;
	unsigned penetrationIterations = 0;

	const float minVelocityDelta = 0.01f;
	const float minPositionDelta = 0.01f;
};