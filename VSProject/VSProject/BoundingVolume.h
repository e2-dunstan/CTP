#pragma once
#include "Shapes.h"

struct BoundingSphere
{
	bool active = false;
	Vector3 centre;
	float radius = 0;
};

struct BoundingCube
{
	bool active = false;
	Vector3 centre;
	Vector3 size;
};

class BoundingVolume
{
public:
	//BoundingVolume(Vector3 _centre, float _radius = 0, Vector3 _size = Vector3());
	BoundingVolume() = default;
	~BoundingVolume() = default;
	
	void Create(Vector3 _centre, float _radius = 0, Vector3 _size = Vector3());
	void Update(Vector3 _centre, float _radius = 0, Vector3 _size = Vector3());
	void Draw();

private:
	BoundingSphere sphere;
	BoundingCube cube;

	GLenum drawMode = GL_QUADS;

	std::unique_ptr<Shapes> shapes = std::make_unique<Shapes>();

	std::unique_ptr<Mathe> mathe = std::make_unique<Mathe>();

	std::vector<Vertex> vertices;
};