#pragma once
#include "Primitive.h"
#include "Collisions.h"
//#include "OctTree.h"

class PrimitiveManager
{
public:

	//resize vec when more primitives in simulation
	PrimitiveManager() = default;
	~PrimitiveManager() = default;

	void Create(Primitive::Type type, const Vector3& scale, const Vector3& translation, const Vector3& rotation, float radius = 0);
	void CreateSphere(float radius, const Vector3& translation);
	void Draw();
	void Update(int deltaTime);

private:

	void UpdateTransforms(Primitive& prim);

	std::unique_ptr<Collisions> collisions = std::make_unique<Collisions>();

	std::vector<Primitive> primitives;

	//OctTree* octTree;
	//bool octTreeCreated = false;

	GLenum GetDrawType(Primitive::Type objectType);

	bool drawBoundingVolumes = true;
	bool drawCollisionVolumes = true;

	double timeSinceCollisionDebug = 0;

};
