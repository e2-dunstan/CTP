#pragma once
#include "Shapes.h"


class BoundingVolume
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
	
	void Create(Type _type, Vector3 _centre, float _radius = 0, Vector3 _size = Vector3());
	void Update(Vector3 _centre, float _radius = 0, Vector3 _size = Vector3());
	void SetVertices(std::vector<Vertex> verts);
	void Draw();

	//Public variables
	Vector3 centre;
	float radius = 0;
	Vector3 size;

	GLenum drawMode = GL_QUADS;
	std::vector<Vertex> vertices;

	//References
	std::unique_ptr<ShapeVertices> shapes = std::make_unique<ShapeVertices>();
	std::unique_ptr<Mathe> mathe = std::make_unique<Mathe>();

private:

	void UpdateBox(Vector3 _centre, Vector3 _size);
	void UpdateSphere(Vector3 _centre, float _radius);

	void SetVerticesBox(std::vector<Vertex> verts);
	void SetVerticesSphere(std::vector<Vertex> verts);
};