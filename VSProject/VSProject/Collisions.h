#pragma once
#include "CollisionFine.h"

struct Primitive;

//Base class for all collision related behaviours.
class Collisions
{
public:
	Collisions() = default;
	~Collisions() = default;

	//void DetectCoarse(RigidBody* rb);
	void DetectFine(Primitive* prim1, Primitive* prim2, bool output = false);

	//void Response(RigidBody* rb);

private:

	std::unique_ptr<CollisionFine> collisionFine = std::make_unique<CollisionFine>();

	bool outputCollisionData = true;
};