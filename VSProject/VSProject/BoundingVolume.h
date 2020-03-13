#pragma once
#include "Utilities.h"

struct BoundingVolume
{
public:
	BoundingVolume(const std::vector<Vertex>& _vertices, Matrix4& _transform);
	BoundingVolume() = default;
	~BoundingVolume() = default;

	void Generate(const std::vector<Vertex>& _vertices, Matrix4& _transform);

	void Draw();

	Vector3 min = Vector3();
	Vector3 max = Vector3();
};