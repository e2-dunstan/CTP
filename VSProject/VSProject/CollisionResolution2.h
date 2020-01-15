#pragma once
#include "CollisionData.h"

class CollisionResolution2
{
public:
	CollisionResolution2() = default;
	~CollisionResolution2() = default;

	void ResolveContacts(std::vector<Contact>& contacts);

private:
	void InitialiseContacts(std::vector<Contact>& contacts);
	void CalculatePenetrationResolution(std::vector<Contact>& contacts);
	void CalculateVelocityResolution(std::vector<Contact>& contacts);

	unsigned velocityIterations = 0;
	unsigned penetrationIterations = 0;

	const float minVelocityDelta = 0.01f;
	const float minPositionDelta = 0.01f;
};