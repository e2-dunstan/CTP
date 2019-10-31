#pragma once
#include "Collisions.h"
#include "OctTree.h"

class Objects
{
public:

	Objects() = default;
	~Objects() = default;

	void Create(RigidBody::Type type, Vector3 scale, Vector3 translation, Vector3 rotation);
	void Animate();
	void Draw();
	void Update();

	std::unique_ptr<Shapes> shapes = std::make_unique<Shapes>();

private:

	void UpdateTransforms(RigidBody& rb);
	void CreateBoundingVolume(RigidBody& obj);

	std::unique_ptr<Mathe> mathe = std::make_unique<Mathe>();

	std::unique_ptr<Collisions> collisions = std::make_unique<Collisions>();

	std::vector<RigidBody*> rbs;

	//OctTree* octTree;

	GLenum GetDrawType(RigidBody::Type objectType);

	bool drawBoundingVolumes = true;

	//bool octTreeCreated = false;
};
