#pragma once
#include "CollisionData.h"
#include <fstream>

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
	const unsigned int penetrationIterations = 20;
	const unsigned int velocityIterations = 1;

	void AdjustDeltaVelocity(Contact& thisContact, Contact& otherContact, const unsigned int bt, const Vector3& rcp, bool sign);

	void CreateCSVFile();
	void WriteToFile(float value, unsigned int obj, unsigned int iter);

	std::ofstream file;
};