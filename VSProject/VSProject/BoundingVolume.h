#pragma once
#include "Utilities.h"

class Primitive;

struct BoundingVolume
{
public:
	BoundingVolume(const std::vector<Vertex>& _vertices, Matrix& _transform, Primitive& prim);
	BoundingVolume() = default;
	~BoundingVolume() = default;

	void Generate(const std::vector<Vertex>& _vertices, Matrix& _transform);

	void Draw();

	Vector3 centre = Vector3();
	Vector3 halfSize = Vector3();
	Vector3 min = Vector3();
	Vector3 max = Vector3();

	Primitive& primitive;
};