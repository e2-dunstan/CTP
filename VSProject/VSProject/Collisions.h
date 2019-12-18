#pragma once
#include "CollisionCoarse.h"
#include "CollisionFine.h"
#include "CollisionResolution.h"

struct Primitive;

//Base class for all collision related behaviours.
class Collisions
{
public:
	Collisions() = default;
	~Collisions() = default;

	void DetectCoarse(Primitive* prim1, Primitive* prim2, bool output = false);
	void DetectFine(bool output = false);
	void Resolution();
	//void Response(RigidBody* rb);

private:

	std::unique_ptr<CollisionData> data = std::make_unique<CollisionData>();
	std::unique_ptr<CollisionCoarse> coarse = std::make_unique<CollisionCoarse>();
	std::unique_ptr<CollisionFine> fine = std::make_unique<CollisionFine>(data->contacts);
	std::unique_ptr<CollisionResolution> resolution = std::make_unique<CollisionResolution>();

	std::vector<PotentialContact> potentialContacts;
};