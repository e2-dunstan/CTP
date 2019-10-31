#include "OctTree.h"
#include <algorithm>


OctTree::OctTree(std::vector<RigidBody*> rbs, Vector3 min, Vector3 max)
{
	for (int i = 0; i < rbs.size(); i++)
	{
		allObjects.push_back(rbs[i]);
		pendingInsertion.push_back(rbs[i]);
	}
	root->region->minp = min;
	root->region->maxp = max;

	currentNode = std::make_unique<OctTreeNode>(&root);
}

void OctTree::UpdateTree()
{
	if (!preexistingTree)
	{
		while (!pendingInsertion.empty())
		{
			currentNode->nodeObjects.push_back(pendingInsertion.front());
			pendingInsertion.pop_front();
		}
		BuildTree();
	}
	else
	{
		while (!pendingInsertion.empty())
		{
			Insert(pendingInsertion.front());
			pendingInsertion.pop_front();
		}
	}
	treeComplete = true;
}

void OctTree::BuildTree()
{
	if (currentNode->nodeObjects.size() <= 1)
		return;

	Vector3 dims = currentNode->region->maxp - currentNode->region->minp;

	//if (dims == Vector3()) ??? not sure what this does
	//{
	//	FindEnclosingCube();
	//	dims = currentNode->region.size;
	//}
	if (dims.x <= minRegionSize.x 
		&& dims.y <= minRegionSize.y
		&& dims.z <= minRegionSize.z)
		return;
	
	Vector3 currentRegionHalfSize = dims / 2.0f;
	Vector3 _min = currentNode->region->minp;
	Vector3 _max = currentNode->region->maxp;
	Vector3 _centre = currentNode->region->minp + currentRegionHalfSize;

	OctTreeRegion octant[8];
	octant[0] = OctTreeRegion(_min, _centre);
	octant[1] = OctTreeRegion(Vector3(_centre.x, _min.y, _min.z), Vector3(_max.x, _centre.y, _centre.z));
	octant[2] = OctTreeRegion(Vector3(_centre.x, _min.y, _centre.z), Vector3(_max.x, _centre.y, _max.z));
	octant[3] = OctTreeRegion(Vector3(_min.x, _min.y, _centre.z), Vector3(_centre.x, _centre.y, _max.z));
	octant[4] = OctTreeRegion(Vector3(_min.x, _centre.y, _min.z), Vector3(_centre.x, _max.y, _centre.z));
	octant[5] = OctTreeRegion(Vector3(_centre.x, _centre.y, _min.z), Vector3(_max.x, _max.y, _centre.z));
	octant[6] = OctTreeRegion(_centre, _max);
	octant[7] = OctTreeRegion(Vector3(_min.x, _centre.y, _centre.z), Vector3(_centre.x, _max.y, _max.z));

	std::vector<std::vector<RigidBody*>> octList;
	for (int i = 0; i < 8; i++)
	{
		octList.push_back(std::vector<RigidBody*>());
	}

	std::vector<RigidBody*> delist;	//objects that can be skipped over

	for (int o = 0; o < allObjects.size(); o++)
	{
		if (allObjects[o]->boundingVolume->cube.active)
		{
			for (int i = 0; i < 8; i++)
			{
				if (Contains(allObjects[o]->boundingVolume->cube.centre, octant[i]))
				{
					octList[i].push_back(allObjects[o]);
					delist.push_back(allObjects[o]);
					break;
				}
			}
		}
		else if (allObjects[o]->boundingVolume->sphere.active)
		{

		}
	}


	for (int d = 0; d < delist.size(); d++)
	{
		allObjects.erase(std::remove(allObjects.begin(), allObjects.end(), delist[d]), allObjects.end());
	}

	for (int i = 0; i < 8; i++)
	{
		if (octList.size() != 0)
		{
			currentNode->childNodes[i]->parent = std::make_unique<OctTreeNode>(&currentNode);
			currentNode->childNodes[i]->region = std::make_unique<OctTreeRegion>(&currentNode->region);
			currentNode->childNodes[i]->nodeObjects = octList[i];

			currentNode = std::make_unique<OctTreeNode>(&currentNode->childNodes[i]);
			BuildTree();
		}
	}

	preexistingTree = true;
	treeComplete = true;

	CreateVertices();
}



//void OctTree::Insert(RigidBody* rb)
//{
//}

bool OctTree::Contains(Vector3 obj, OctTreeRegion oct)
{
	if (obj.x < oct.maxp.x && obj.y < oct.maxp.y && obj.z < oct.maxp.z
		&& obj.x >= oct.minp.x && obj.y >= oct.minp.y && obj.z >= oct.minp.x)
	{
		return true;
	}
	else return false;
}

void OctTree::CreateVertices()
{
	debugVertices.clear();
	std::vector<OctTreeNode*> nodes = GetChildNodes(root.get());
	std::cout << "Oct tree nodes: " << nodes.size() << std::endl;
}

std::vector<OctTreeNode*> OctTree::GetChildNodes(OctTreeNode* parent)
{
	std::vector<OctTreeNode*> children;

	for (int c = 0; c < 8; c++)
	{
		OctTreeNode* child = parent->childNodes[c].get;
		if (child->region->maxp != child->region->minp)
		{
			children.push_back(child);
			std::vector<OctTreeNode*> grandchildren = GetChildNodes(child);
			for (int g = 0; g < grandchildren.size(); g++)
			{
				children.push_back(grandchildren[g]);
			}
		}
	}

	return children;
}

void OctTree::Draw()
{
	if (!treeComplete) return;

	//draw oct tree to help debug
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_QUADS);

	glColor3f(0, 1, 0);

	for (int v = 0; v < debugVertices.size(); v++)
	{
		//glVertex3f(debugVertices[v].position.x, debugVertices[v].position.y, debugVertices[v].position.z);
	}

	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
