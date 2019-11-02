#pragma once
#include "Shapes.h"

class CollisionVolume
{
public:
	CollisionVolume() = default;
	~CollisionVolume() = default;

	//void Create(RigidBody rb, RigidBody::Type type);


private:

	void Cube();
	void Sphere();
};