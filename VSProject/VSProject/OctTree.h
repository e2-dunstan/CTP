#pragma once
#include "Utilities.h"
#include <array>

struct Primitive;
class Collisions;

struct Object
{
	//Object() = default;
	Object(Primitive* prim) : primitive(prim) { }
	~Object()
	{
		if (primitive)
		{
			delete primitive;
		}
		if (nextObj)
		{
			delete nextObj;
		}
	};

	Primitive* primitive = nullptr;
	Object* nextObj = nullptr;// = std::make_unique<Object>();
};

struct Node
{
	Node() = default;
	~Node()
	{
		if (children)
		{
			delete[] children;
		}
		if (objList)
		{
			delete objList;
		}
	}

	Vector3 centre = Vector3();
	float halfWidth = 0;

	//std::array<std::unique_ptr<Node>, 8> children = { std::make_unique<Node>() };
	Node* children[8] = { nullptr };
	Object* objList = nullptr;// std::make_unique<Object>();
};

class OctTree
{
public:
	OctTree() = default;
	OctTree(const Vector3& centre, float halfWidth, int depth);
	~OctTree()
	{
		if (root)
		{
			delete root;
		}
	}

	Node* Construct(const Vector3& centre, float halfWidth, int depth);
	void Insert(Primitive& primitive, Node& node);
	void TestCollisions(Node& node, Collisions& collisionRef);

	Node* root = new Node();// std::make_unique<Node>();
};