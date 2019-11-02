#pragma once
#include "Utilities.h"
#include "RigidBody.h"
#include "BoundingVolume.h"

class Primitive
{
public:
	Primitive() = default;
	Primitive(std::vector<Vertex> v) : vertices(v) {}
	~Primitive() = default;

	enum Type
	{
		BOX, SPHERE, PLANE, COMPLEX
	};
	Type type = BOX;

	std::unique_ptr<RigidBody> rigidbody = std::make_unique<RigidBody>();
	
	Vector3 translation = Vector3();
	Vector3 rotation = Vector3();
	Vector3 scale = Vector3();
	Matrix transform = Matrix(4, 4);

	std::vector<Vertex> vertices;

	bool enableCollision = true;

	std::unique_ptr<BoundingVolume> boundingVolume = std::make_unique<BoundingVolume>();
	std::unique_ptr<CollisionVolume> collisionVolume = std::make_unique<CollisionVolume>();


};