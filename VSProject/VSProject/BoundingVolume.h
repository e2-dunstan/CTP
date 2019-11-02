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
	
	//void Create(Vector3 _centre, float _radius = 0, Vector3 _size = Vector3());
	//void Create(std::vector<Vertex> rbVerts, Type type);
	virtual void Create(Vector3 _centre, float _radius) = 0;
	virtual void Create(Vector3 _centre, Vector3 _size) = 0;
	virtual void Update(Vector3 _centre, float _radius, Vector3 _rotation = Vector3()) = 0;
	virtual void Update(Vector3 _centre, Vector3 _size = Vector3(), Vector3 _rotation = Vector3()) = 0;
	virtual void SetVertices(std::vector<Vertex> verts) {};

	//void SetCentreAndSizeFromVertices(std::vector<Vertex> verts);
	void Draw();

	//Public variables
	Vector3 centre;
	GLenum drawMode = GL_QUADS;
	std::vector<Vertex> vertices;
	std::unique_ptr<ShapeVertices> shapes = std::make_unique<ShapeVertices>();
	std::unique_ptr<Mathe> mathe = std::make_unique<Mathe>();

};