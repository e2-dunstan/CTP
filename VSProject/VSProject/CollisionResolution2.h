#pragma once
#include "CollisionData.h"

class CollisionResolution2
{
public:
	CollisionResolution2() = default;
	~CollisionResolution2() = default;

	void PenetrationResolution(std::vector<Contact>& contacts);
	void VelocityResolution(std::vector<Contact>& contacts);

private:
	unsigned GetContactWithLargestPenetration(std::vector<Contact>& contacts);
	unsigned GetContactWithLargestVelocityMagnitude(std::vector<Contact>& contacts);

	const float minVelocityChange = 0.01f;
	const float minPositionChange = 0.01f;

	unsigned numContacts = 0;
};