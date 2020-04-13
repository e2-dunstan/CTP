#pragma once
#include "Utilities.h"

struct BoundingVolume
{
public:
	BoundingVolume(const std::vector<Vertex>& _vertices, Matrix4& _transform);
	BoundingVolume(const Vector3& minimum, const Vector3& maximum) : min(minimum), max(maximum) {}
	BoundingVolume(Matrix4& _transform);
	BoundingVolume() = default;
	~BoundingVolume() = default;

	void Generate(const std::vector<Vertex>& _vertices, Matrix4& _transform);
	void GenerateForBox(Matrix4& _transform);

	void UpdateMinMax(const Vector3& minimum, const Vector3& maximum);

	void Draw();



	Vector3 min = Vector3();
	Vector3 max = Vector3();
};