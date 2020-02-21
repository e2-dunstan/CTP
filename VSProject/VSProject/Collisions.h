#pragma once
#include "CollisionCoarse.h"
#include "CollisionFine.h"
#include "SeparatingAxisTheorem.h"
#include "CollisionResolution2.h"

struct Primitive;

//Base class for all collision related behaviours.
class Collisions
{
public:
	Collisions();
	~Collisions() = default;

	void DetectCoarse(Primitive* prim1, Primitive* prim2);
	void DetectFine();
	void Resolution();

private:

	std::unique_ptr<CollisionData> data = std::make_unique<CollisionData>();
	std::unique_ptr<CollisionCoarse> coarse = std::make_unique<CollisionCoarse>();
	std::unique_ptr<CollisionFine> fine = std::make_unique<CollisionFine>(data->contacts);
	std::unique_ptr<SAT> sat = std::make_unique<SAT>(data->contacts);
	//std::unique_ptr<CollisionResolution> resolution = std::make_unique<CollisionResolution>();
	std::unique_ptr<CollisionResolution2> resolution2 = std::make_unique<CollisionResolution2>();

	std::vector<PotentialContact> potentialContacts;
};