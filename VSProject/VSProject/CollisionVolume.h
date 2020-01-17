#pragma once
#include "Utilities.h"

struct CollisionVolume
{
public:
	CollisionVolume() = default;
	~CollisionVolume() = default;

	enum class Type
	{
		BOX, SPHERE, PLANE, CAPSULE, CYLINDER, COMPLEX
	};
	Type type = Type::BOX;

	void Create(Type _type, const Vector3& _centre, float _radius, 
		const Vector3& _size, const Quaternion& _orientation,
		const Vector3& _normal = Vector3(), float _length = 0);
	void Update(const Vector3& _centre, float _radius, 
		const Vector3& _size, const Quaternion& _orientation);

	//Shared
	Vector3 centre;
	//Sphere, cylinder and capsule
	float radius = 0;
	//Cylinder and capsule
	float length = 0;
	//Box
	Vector3 halfSize;
	std::vector<Vector3> vertices;
	Quaternion orientation;
	std::vector<Vector3> normals;
	//Plane
	Vector3 normal = Vector3(0, 1, 0);

	Matrix axisMat = Matrix(4,4);

private:

	void Box();
	//void Sphere();
	//void Plane();
	//void Complex();
};