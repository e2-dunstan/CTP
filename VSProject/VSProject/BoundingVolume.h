#pragma once
#include "Shapes.h"

struct BoundingVolume
{
public:
	//BoundingVolume(Vector3 _centre, float _radius = 0, Vector3 _size = Vector3());
	BoundingVolume() = default;
	~BoundingVolume() = default;

	enum class Type
	{
		BOX, SPHERE
	};
	Type type = Type::BOX;
	
	void Create(Type _type, const Vector3& _centre, float _radius = 0, const Vector3& _size = Vector3());
	void Update(const Vector3& _centre, float _radius = 0, const Vector3& _size = Vector3());
	void SetVertices(std::vector<Vertex> verts);
	void Draw();

	//Public variables
	Vector3 centre = Vector3();
	float radius = 0;
	Vector3 size = Vector3();

	GLenum drawMode = GL_QUADS;
	std::vector<Vertex> vertices;

private:

	std::vector<Vertex> baseBox;

	void UpdateBox(const Vector3& _centre, const Vector3& _size);
	void UpdateSphere(const Vector3& _centre, float _radius);

	void SetVerticesBox(std::vector<Vertex> verts);
	void SetVerticesSphere(std::vector<Vertex> verts);
};