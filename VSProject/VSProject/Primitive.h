#pragma once
#include "RigidBody.h"
#include "BoundingVolume.h"
//#include "CollisionVolume.h"

struct Primitive
{
	Primitive() = default;
	Primitive(std::vector<Vertex> v, float r = 0) : vertices(v), radius(r) { initialised = true; }
	~Primitive() = default;

	enum class Type
	{
		BOX, SPHERE, PLANE, CAPSULE, CYLINDER, COMPLEX
	};
	Type type = Type::BOX;

	void Start();
	void Update();
	void Draw();

	void CalculateInertiaTensor();

	//Physics data.
	RigidBody rigidbody = RigidBody();
	
	//Transform data.
	Vector3 translation = Vector3();
	//Vector3 rotation = Vector3();
	Quaternion orientation = Quaternion();
	Vector3 scale = Vector3();
	Matrix transform = Matrix();
	void UpdateTransform();
	bool updateTransform = false;
	bool transformChanged = true;

	void GetOrientation(Quaternion* _orientation) const;
	Quaternion GetOrientation() const;
	void GetOrientation(Matrix* _matrix) const;
	void GetOrientation(double _matrix[16]) const;

	void SetOrientation(const Quaternion& _orientation);
	void SetOrientation(const double r, const double i, const double j, const double k);

	std::vector<Vertex> vertices = std::vector<Vertex>();

	bool enableCollision = true;
	bool colliding = false;

	BoundingVolume boundingVolume;
	CollisionVolume collisionVolume;

	float radius = 0; //if sphere, capsule or cylinder
	Vector3 upDir = Vector3(0, 1, 0); //if capsule or cylinder
	Matrix upDirMat = Matrix(); //if cylinder

	void Tween(float speed, const Vector3& direction, float distance);
	void SetTweenOrigin();

	Vector3 tweenOrigin = Vector3();
	Vector3 tweenMax = Vector3();
	bool tweenMaxSet = false;
	bool moveTowards = true;

	bool initialised = false;

private:
	Vector3 previousPosition = Vector3();
	GLenum GetDrawType(Primitive::Type objectType);

	bool debugCollision = false;
};