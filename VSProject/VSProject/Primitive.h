#pragma once
#include "RigidBody.h"
#include "BoundingVolume.h"
//#include "CollisionVolume.h"

struct Primitive
{
	Primitive() = default;
	Primitive(std::vector<Vertex> v, float r = 0) : vertices(v), radius(r) { initialised = true; }
	~Primitive() = default;

	enum Type
	{
		BOX, SPHERE, PLANE, CAPSULE, CYLINDER, COMPLEX
	};
	Type type = Type::BOX;

	void Update();
	void Draw();

	void CalculateInertiaTensor();

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
	bool colliding = false;

	BoundingVolume boundingVolume;
	CollisionVolume collisionVolume;

	float radius = 0; //if sphere, capsule or cylinder
	Vector3 upDir = Vector3(0, 1, 0); //if capsule or cylinder
	Matrix upDirMat = Matrix(4, 4); //if cylinder

	void Tween(float speed, const Vector3& direction, float distance);
	void SetTweenOrigin();

	Vector3 tweenOrigin;
	Vector3 tweenMax;
	bool tweenMaxSet = false;
	bool moveTowards = true;

	bool initialised = false;

private:

	GLenum GetDrawType(Primitive::Type objectType);

};