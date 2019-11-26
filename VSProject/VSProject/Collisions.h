#pragma once
#include "CollisionFine.h"
#include "CollisionResolution.h"

struct Primitive;

//Base class for all collision related behaviours.
class Collisions
{
public:
	Collisions() = default;
	~Collisions() = default;

	//void DetectCoarse(RigidBody* rb);

	void DetectFine(Primitive* prim1, Primitive* prim2, bool output = false);
	void Resolution();
	//void Response(RigidBody* rb);

private:

	std::unique_ptr<CollisionData> data = std::make_unique<CollisionData>();
	std::unique_ptr<CollisionFine> collisionFine = std::make_unique<CollisionFine>(data->contacts);
	std::unique_ptr<CollisionResolution> collisionResolution = std::make_unique<CollisionResolution>();


	bool outputCollisionData = false;
};