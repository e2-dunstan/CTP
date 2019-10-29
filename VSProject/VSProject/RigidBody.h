#pragma once
#include "BoundingVolume.h"

struct RigidBody
{
	enum class Type
	{
		CUBE, SPHERE, COMPLEX
	};
	Type type = Type::CUBE;

	std::vector<Vertex> vertices;

	Vector3 translation = Vector3();
	Vector3 rotation = Vector3();
	Vector3 scale = Vector3();
	Matrix transform = Matrix(4, 4);

	bool updateTransforms = false;

	std::unique_ptr<BoundingVolume> boundingVolume = std::make_unique<BoundingVolume>();

	RigidBody(std::vector<Vertex> _v)
		: vertices(_v) {}
};