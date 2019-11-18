#pragma once
#include "Utilities.h"
#include "RigidBody.h"
#include "BoundingVolume.h"

class Primitive
{
public:
	Primitive() = default;
	Primitive(std::vector<Vertex> v) : vertices(v) { initialised = true; }
	~Primitive() = default;

	enum class Type
	{
		BOX, SPHERE, PLANE, COMPLEX
	};
	Type type = Type::BOX;

	//Physics data.
	RigidBody rigidbody = RigidBody();// = std::make_unique<RigidBody>();
	
	//Transform data.
	Vector3 translation = Vector3();
	Vector3 rotation = Vector3();
	Vector3 scale = Vector3();
	Matrix transform = Matrix(4, 4);
	bool updateTransforms = false;

	std::vector<Vertex> vertices;

	bool enableCollision = true;

	BoundingVolume boundingVolume;// = std::make_unique<BoundingVolume>();
	CollisionVolume collisionVolume;// = std::make_unique<CollisionVolume>();

	float radius = 0; //if sphere

	void Tween(double deltaTime, float speed, Vector3 direction, float distance);
	void SetTweenOrigin();

private:
	Vector3 tweenOrigin;
	Vector3 tweenMax;
	bool tweenMaxSet = false;
	bool moveTowards = true;

	bool initialised = false;
};