#pragma once
#include "RigidBody.h"
#include <deque>

//https://www.gamedev.net/articles/programming/general-and-gameplay-programming/introduction-to-octrees-r3529/

struct OctTreeRegion
{
	Vector3 minp;
	Vector3 maxp;


	OctTreeRegion(Vector3 _min, Vector3 _max)
		: minp(_min), maxp(_max) {}
	OctTreeRegion() = default;
	~OctTreeRegion() = default;


};

struct OctTreeNode
{
	std::unique_ptr<OctTreeRegion> region = std::make_unique<OctTreeRegion>();
	std::vector<RigidBody*> nodeObjects;
	std::unique_ptr<OctTreeNode> parent = std::make_unique<OctTreeNode>();;
	std::unique_ptr<OctTreeNode> childNodes[8];

	OctTreeNode() = default;
};

class OctTree
{
public:
	OctTree() = default;
	OctTree(std::vector<RigidBody*> rbs, Vector3 size = Vector3(128, 128, 128), Vector3 centre = Vector3());

	void UpdateTree();
	void BuildTree();
	void Insert(RigidBody* rb);

	bool Contains(Vector3 obj, OctTreeRegion oct);

	void CreateVertices();
	std::vector<OctTreeNode*> GetChildNodes(OctTreeNode* parent);
	void Draw();

private:
	static std::deque<RigidBody*> pendingInsertion;
	static std::vector<RigidBody*> allObjects;

	std::unique_ptr<OctTreeNode> root = std::make_unique<OctTreeNode>();
	std::unique_ptr<OctTreeNode> currentNode = std::make_unique<OctTreeNode>();

	const Vector3 minRegionSize = Vector3(1, 1, 1);
	
	int maxTreeLifespan = 8;
	int currentTreeLifespan = -1;

	bool treeComplete = false;
	bool preexistingTree = false;

	std::unique_ptr<Shapes> shapes = std::make_unique<Shapes>();
	std::vector<std::vector<Vertex>> debugVertices;
};


//struct OctTreeNode
//{
//	Vector3 position;
//	//OctTreeNode children[8];
//
//	unsigned int GetChildIndex(const Vector3& objectOrigin)
//	{
//		unsigned int index = 0;
//		if (objectOrigin.x > position.x) index += 1;
//		if (objectOrigin.y > position.y) index += 2;
//		if (objectOrigin.z > position.z) index += 4;
//		return index;
//	}
//};

