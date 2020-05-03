#pragma once
#include "CollisionData.h"
#include <fstream>

class CollisionResolution
{
public:
	CollisionResolution() = default;
	~CollisionResolution() = default;

	void PenetrationResolution(std::vector<Contact>& contacts);
	void VelocityResolution(std::vector<Contact>& contacts);

	void SortContactsByPenetration(std::vector<Contact>& contacts);
	void SortContactsByVelocityMag(std::vector<Contact>& contacts);

private:

	unsigned int numContacts = 0;

	//Per frame
	const unsigned int maxPenetrationIterations = 100;
	const unsigned int maxVelocityIterations = 50;

	void AdjustDeltaVelocity(Contact& thisContact, Contact& otherContact, const unsigned int bt, const Vector3& rcp, bool sign);

	void CreateCSVFile();
	void WriteToFile(float value, unsigned int obj, unsigned int iter);

	std::ofstream file;
};