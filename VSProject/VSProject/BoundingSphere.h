#pragma once
#include "BoundingVolume.h"


class BoundingSphere : public BoundingVolume
{
public:
	BoundingSphere() = default;
	~BoundingSphere() = default;

	void Create(Vector3 _centre, float _radius);
	void Update(Vector3 _centre, float _radius);
	void SetVertices(std::vector<Vertex> verts);

	float radius;

};