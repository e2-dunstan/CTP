#include "OctTree.h"
#include "BoundingVolume.h"
#include "Collisions.h"

OctTree::OctTree(const Vector3& centre, float halfWidth, int depth)
{
	root = Construct(centre, halfWidth, depth);

	if (root == nullptr)
	{
		std::cout << "ERROR: depth of oct tree less than zero" << std::endl;
	}
}

std::unique_ptr<Node> OctTree::Construct(const Vector3& centre, float halfWidth, int depth)
{
	if (depth < 0) return nullptr;

	std::unique_ptr<Node> node = std::make_unique<Node>();
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

void OctTree::Insert(std::unique_ptr<Object>& object, std::unique_ptr<Node>& node)
{
	unsigned index = 0;
	int straddle = 0;

	for (unsigned i = 0; i < 3; i++)
	{
		float delta = object->boundingVolume.centre[i] - node->centre[i];
		if (abs(delta) <= object->boundingVolume.halfSize[i])
		{
			straddle = 1;
			break;
		}
		if (delta > 0.0f)
		{
			index |= (1 << i);
		}
	}

	if (!straddle)
	{
		if (node->children[index] == NULL)
		{
			node->children[index] = std::make_unique<Node>();
		}
		Insert(object, node->children[index]);
	}
	else
	{
		object->nextObj = std::make_unique<Object>(node->objList);
		node->objList = std::make_unique<Object>(object);
	}
}

void OctTree::TestCollisions(std::unique_ptr<Node>& node, Collisions& collisionRef)
{
	const int maxDepth = 40;
	static std::array<std::unique_ptr<Node>, maxDepth> ancestors;
	static int depth = 0;

	ancestors[depth++] = std::make_unique<Node>(node);
	for (unsigned n = 0; n < depth; n++)
	{
		Object* obj1;
		Object* obj2;

		for (obj1 = ancestors[n]->objList.get(); obj1; obj1 = obj1->nextObj.get())
		{
			for (obj2 = node->objList.get(); obj2; obj2 = obj2->nextObj.get())
			{
				if (obj1 == obj2) break;
				collisionRef.DetectCoarse(obj1->boundingVolume, obj2->boundingVolume);
			}
		}
	}

	for (unsigned i = 0; i < 8; i++)
	{
		if (node->children[i])
		{
			TestCollisions(node->children[i], collisionRef);
		}
	}

	depth--;
}
