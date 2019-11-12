#pragma once
#include "Shapes.h"

class CollisionVolume
{
public:
	CollisionVolume() = default;
	~CollisionVolume() = default;

	enum class Type
	{
		BOX, SPHERE, PLANE, COMPLEX
	};
	Type type = Type::BOX;

	void Create(Type _type, Vector3 _centre, float _radius, Vector3 _size, Vector3 _rotation, Vector3 _normal);
	void Update(Vector3 _centre, float _radius, Vector3 _size, Vector3 _rotation);

	void Draw();

	//Shared
	Vector3 centre;

	//Sphere
	float radius = 0;

	//Box
	Vector3 halfSize;
	std::vector<Vector3> vertices;
	Vector3 rotation;
	std::vector<Vector3> normals;

	//Plane
	Vector3 normal = Vector3(0, 1, 0);

	Matrix axisMat = Matrix(4,4);

private:

	void Box();
	//void Sphere();
	//void Plane();
	//void Complex();

	std::unique_ptr<Mathe> mathe = std::make_unique<Mathe>();
};