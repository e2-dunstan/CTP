#pragma once
#include "CollisionCoarse.h"
#include "CollisionFine.h"
#include "SeparatingAxisTheorem.h"
#include "CollisionResolution2.h"

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
	std::unique_ptr<CollisionFine> fine = std::make_unique<CollisionFine>(data->contacts);
	std::unique_ptr<SAT> sat = std::make_unique<SAT>(data->contacts);
	std::unique_ptr<CollisionResolution2> resolution2 = std::make_unique<CollisionResolution2>();

	std::vector<PotentialContact> potentialContacts;

	struct ContactDisplay
	{
		Vector3 origin = Vector3();
		Vector3 normal = Vector3();
	};

	ContactDisplay contactDisplays[30];
};