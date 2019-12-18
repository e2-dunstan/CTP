#pragma once
#include "RigidBody.h"
#include "BoundingVolume.h"
//#include "CollisionVolume.h"

struct Primitive
{
	Primitive() = default;
	Primitive(std::vector<Vertex> v) : vertices(v) { initialised = true; }
	~Primitive() = default;

	enum class Type
	{
		BOX, SPHERE, PLANE, COMPLEX
	};
	Type type = Type::BOX;

	void Update(const double& deltaTime);

	//Physics data.
	RigidBody rigidbody = RigidBody();
	
	//Transform data.
	Vector3 translation = Vector3();
	Vector3 rotation = Vector3();
	Vector3 scale = Vector3();
	Matrix transform = Matrix(4, 4);
	void UpdateTransform();
	bool updateTransform = false;

	std::vector<Vertex> vertices;

	bool enableCollision = true;

	BoundingVolume boundingVolume;
	CollisionVolume collisionVolume;

	float radius = 0; //if sphere

	void Tween(const double& deltaTime, float speed, const Vector3& direction, float distance);
	void SetTweenOrigin();

	Vector3 tweenOrigin;
	Vector3 tweenMax;
	bool tweenMaxSet = false;
	bool moveTowards = true;

	bool initialised = false;

private:


};