#pragma once
#include "RigidBody.h"

class Collisions
{
public:
	Collisions() = default;
	~Collisions() = default;

	void DetectCoarse(RigidBody* rb);
	void DetectFine(RigidBody* rb);

	void Response(RigidBody* rb);

};