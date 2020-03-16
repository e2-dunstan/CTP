#pragma once
#include "Utilities.h"

struct CollisionVolume
{
public:
	CollisionVolume() = default;
	~CollisionVolume() = default;

	Vector3 centre;
	Matrix4 axisMat = Matrix4();

	//void Create(const Vector3& _centre, float _radius, 
	//	const Vector3& _size, const Quaternion& _orientation,
	//	const Vector3& _normal = Vector3(), float _length = 0);
	//void Update(const Vector3& _centre, float _radius, 
	//	const Vector3& _size, const Quaternion& _orientation);
	//Cylinder and capsule
	//float length = 0;
	//float radius = 0;
};

struct BoxCV : public CollisionVolume
{
	void Create(const Vector3& _centre, const Vector3& _size) { Update(_centre, _size); }
	void Update(const Vector3& _centre, const Vector3& _size);

	Vector3 halfSize;
	Vector3 vertices[8] = { Vector3() };
	Vector3 normals[6] = { Vector3() };
};

struct PlaneCV : public CollisionVolume
{
	void Create(const Vector3& _centre, const Vector3& _normal = Vector3(0, 1, 0)) { Update(_centre, _normal); }
	void Update(const Vector3& _centre, const Vector3& _normal = Vector3(0, 1, 0));

	Vector3 normal = Vector3(0, 1, 0);
};

struct SphereCV : public CollisionVolume
{
	void Create(const Vector3& _centre) { Update(_centre); }
	void Update(const Vector3& _centre);
};