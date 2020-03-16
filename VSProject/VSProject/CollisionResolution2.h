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
	const float minVelocityChange = 0.01f;
	const float minPositionChange = 0.01f;

	unsigned int numContacts = 0;

	void AdjustDeltaVelocity(Contact* thisContact, Contact* otherContact, const unsigned int bt, const Vector3& rcp);
};