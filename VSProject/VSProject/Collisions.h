#pragma once
#include "CollisionCoarse.h"
#include "CollisionFine.h"
#include "SeparatingAxisTheorem.h"
#include "CollisionResolution.h"


struct RecordData
{
	RecordData();
	~RecordData();

	const bool resRecordTimes = false;
	const bool detRecordTimes = false;
	const bool recordIter = false;
	uint16_t resRecordIndex = 0;
	uint16_t detRecordIndex = 0;
	uint16_t iterIndex = 0;

	uint16_t* pIters = nullptr;
	uint16_t* vIters = nullptr;

	long long* detFineTimes = nullptr;

	long long* penTimes = nullptr;
	long long* velTimes = nullptr;
	long long* totalTimes = nullptr;

	void StoreDetTimes(long long f);
	void StoreResTimes(long long p, long long v, long long t);
	void StoreIterCount(uint16_t p, uint16_t v);
};


//Base class for all collision related behaviours.
class Collisions
{
public:
	Collisions();
	~Collisions() = default;

	void DetectCoarse(class Primitive* prim1, class Primitive* prim2);
	void DetectFine();
	void Resolution();

	void DrawContacts();

private:

	std::unique_ptr<CollisionData> data = std::make_unique<CollisionData>();

	std::unique_ptr<CollisionCoarse> coarse = std::make_unique<CollisionCoarse>();
	std::unique_ptr<CollisionFine> fine;
	std::unique_ptr<SAT> sat;
	std::unique_ptr<CollisionResolution> resolution = std::make_unique<CollisionResolution>();

	std::vector<PotentialContact> potentialContacts;

	struct ContactDisplay
	{
		Vector3 origin = Vector3();
		Vector3 normal = Vector3();
	};

	ContactDisplay contactDisplays[30];

	std::unique_ptr<RecordData> recordData = std::make_unique<RecordData>();
};