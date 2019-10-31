#pragma once
#include "Shapes.h"

struct BoundingSphere
{
	bool active = false;
	Vector3 centre;
	float radius = 0;
};
struct BoundingBox
{
	bool active = false;
	Vector3 centre;
	Vector3 size;

	BoundingBox() = default;
	BoundingBox(Vector3 _s, Vector3 _c)
		: size(_s), centre(_c) {}
};

class BoundingVolume
{
public:
	//BoundingVolume(Vector3 _centre, float _radius = 0, Vector3 _size = Vector3());
	BoundingVolume() = default;
	~BoundingVolume() = default;

	enum class Type
	{
		CUBE, SPHERE
	};
	
	void Create(Vector3 _centre, float _radius = 0, Vector3 _size = Vector3());
	void Create(std::vector<Vertex> rbVerts, Type type);
	
	void SetCentreAndSizeFromVertices(std::vector<Vertex> verts);

	void Update(Vector3 _centre, float _radius = 0, Vector3 _size = Vector3(), Vector3 _rotation = Vector3());
	void Draw();

	BoundingBox cube;
	BoundingSphere sphere;

private:

	GLenum drawMode = GL_QUADS;

	std::unique_ptr<Shapes> shapes = std::make_unique<Shapes>();

	std::unique_ptr<Mathe> mathe = std::make_unique<Mathe>();

	std::vector<Vertex> vertices;

};