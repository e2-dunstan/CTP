#pragma once
#include "Utilities.h"
#include <array>

class BoundingVolume;
class Collisions;

struct Object
{
	Object() = default;
	Object(BoundingVolume& bv) : boundingVolume(bv) {}
	~Object() = default;

	BoundingVolume& boundingVolume;// = std::make_unique<BoundingVolume>();
	std::unique_ptr<Object> nextObj = std::make_unique<Object>();
};

struct Node
{
	Node() = default;
	~Node() = default;

	Vector3 centre = Vector3();
	float halfWidth = 0;

	std::array<std::unique_ptr<Node>, 8> children = { std::make_unique<Node>() };
	std::unique_ptr<Object> objList = std::make_unique<Object>();
};

class OctTree
{
public:
	OctTree(const Vector3& centre, float halfWidth, int depth);
	~OctTree() = default;

	std::unique_ptr<Node> Construct(const Vector3& centre, float halfWidth, int depth);
	void Insert(std::unique_ptr<Object>& object, std::unique_ptr<Node>& node);
	void TestCollisions(std::unique_ptr<Node>& node, Collisions& collisionRef);

	std::unique_ptr<Node> root = std::make_unique<Node>();
};