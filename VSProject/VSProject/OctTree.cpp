#include "OctTree.h"
#include "Primitive.h"
#include "Collisions.h"

OctTree::OctTree(const Vector3& centre, float halfWidth, int depth)
{
	root = Construct(centre, halfWidth, depth);

	if (root == nullptr)
	{
		std::cout << "ERROR: depth of oct tree less than zero" << std::endl;
	}
}

Node* OctTree::Construct(const Vector3& centre, float halfWidth, int depth)
{
	if (depth < 0) return nullptr;

	Node* node = new Node();// = std::make_unique<Node>();
	Vector3 offset;

	float step = halfWidth * 0.5f;
	for (unsigned i = 0; i < 8; i++)
	{
		offset.x = (i & 1) ? step : -step;
		offset.y = (i & 2) ? step : -step;
		offset.z = (i & 4) ? step : -step;
		node->children[i] = Construct(centre + offset, step, depth - 1);
	}
	return node;
}

void OctTree::Insert(Primitive& primitive, Node& node)
{
	unsigned index = 0;
	int straddle = 0;

	for (unsigned i = 0; i < 3; i++)
	{
		double delta = primitive.boundingVolume.centre[i] - node.centre[i];
		if (abs(delta) <= primitive.boundingVolume.halfSize[i])
		{
			straddle = 1;
			break;
		}
		if (delta > 0.0)
		{
			index |= (1 << i);
		}
	}

	if (!straddle)
	{
		if (node.children[index] == NULL)
		{
			node.children[index] = new Node();// std::make_unique<Node>();
		}
		Insert(primitive, *node.children[index]);
	}
	else
	{
		Object* object = new Object(&primitive);
		object->nextObj = node.objList;
		node.objList = object;
	}
}

void OctTree::TestCollisions(Node& node, Collisions& collisionRef)
{
	const int maxDepth = 40;
	//static std::array<std::unique_ptr<Node>, maxDepth> ancestors;
	static Node* ancestors[maxDepth];
	static unsigned depth = 0;

	ancestors[depth++] = &node;// std::make_unique<Node>(node);
	for (unsigned n = 0; n < depth; n++)
	{
		Object* obj1;
		Object* obj2;

		for (obj1 = ancestors[n]->objList; obj1; obj1 = obj1->nextObj)
		{
			for (obj2 = node.objList; obj2; obj2 = obj2->nextObj)
			{
				if (obj1 == obj2) break;
				collisionRef.DetectCoarse(obj1->primitive, obj2->primitive);
			}
		}
	}
	for (unsigned i = 0; i < 8; i++)
	{
		if (node.children[i])
		{
			TestCollisions(*node.children[i], collisionRef);
		}
	}
	depth--;
}
