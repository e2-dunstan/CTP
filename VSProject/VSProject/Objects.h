#pragma once
#include "Collisions.h"
//#include "OctTree.h"

class Objects
{
public:

	Objects() = default;
	~Objects() = default;

	void Create(Primitive::Type type, Vector3 scale, Vector3 translation, Vector3 rotation, float radius = 0);
	void CreateSphere(float radius, Vector3 translation);
	void Draw();
	void Update(int deltaTime);

	std::unique_ptr<ShapeVertices> shapes = std::make_unique<ShapeVertices>();

private:

	void UpdateTransforms(Primitive* prim);

	std::unique_ptr<Mathe> mathe = std::make_unique<Mathe>();
	std::unique_ptr<Collisions> collisions = std::make_unique<Collisions>();

	std::vector<Primitive*> primitives;

	//OctTree* octTree;

	GLenum GetDrawType(Primitive::Type objectType);

	bool drawBoundingVolumes = true;
	bool drawCollisionVolumes = false;

	//bool octTreeCreated = false;
};
