#pragma once
#include "CollisionData.h"
#include <fstream>

class CollisionResolution
{
	friend class Collisions;
public:
	CollisionResolution() = default;
	~CollisionResolution() = default;

	void PenetrationResolution(std::vector<Contact>& contacts);
	void VelocityResolution(std::vector<Contact>& contacts);

	void SortContactsByPenetration(std::vector<Contact>& contacts);
	void SortContactsByVelocityMag(std::vector<Contact>& contacts);

private:

	uint16_t numContacts = 0;

	//Per frame
	const uint16_t maxPenetrationIterations = 100;
	const uint16_t maxVelocityIterations = 100;

	void AdjustDeltaVelocity(Contact& thisContact, Contact& otherContact, const unsigned int bt, const Vector3& rcp, bool sign);

	//void CreateCSVFile();
	//void WriteToFile(float value, unsigned int obj, unsigned int iter);

	//std::ofstream file;

protected:
	uint16_t numPenIterations = 0;
	uint16_t numVelIterations = 0;
};