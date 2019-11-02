#pragma once
#include "BoundingVolume.h"

class BoundingBox : public BoundingVolume
{
public:
	BoundingBox() = default;
	~BoundingBox() = default;

	void Create(Vector3 _centre, Vector3 _size);
	void Update(Vector3 _centre, Vector3 _size = Vector3(), Vector3 _rotation = Vector3());
	void SetVertices(std::vector<Vertex> verts);

	Vector3 size;
};