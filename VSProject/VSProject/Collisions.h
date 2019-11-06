#pragma once
#include "CollisionFine.h"


class Collisions
{
public:
	Collisions() = default;
	~Collisions() = default;

	//void DetectCoarse(RigidBody* rb);
	void DetectFine(Primitive* prim1, Primitive* prim2);

	//void Response(RigidBody* rb);

private:

	std::unique_ptr<CollisionFine> collisionFine = std::make_unique<CollisionFine>();

};