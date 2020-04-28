#pragma once
#include "CollisionCoarse.h"
#include "CollisionFine.h"
#include "SeparatingAxisTheorem.h"
#include "CollisionResolution.h"

//Base class for all collision related behaviours.
class Collisions
{
public:
	Collisions();
	~Collisions() = default;

	void DetectCoarse(const class std::shared_ptr<Primitive> prim1, const class std::shared_ptr<Primitive> prim2);
	void DetectFine();
	void Resolution();

	void DrawContacts();

private:

	std::shared_ptr<CollisionData> data = std::make_shared<CollisionData>();

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
};