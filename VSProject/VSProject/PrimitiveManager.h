#pragma once
#include "Primitive.h"
#include "Collisions.h"
#include "RayCast.h"
//#include "OctTree.h"

//class OctTree;

// Update Order:
// 1. Apply forces
// 2. Update positions and velocities
// 3. Detect collisions
// 4. Solve sonstraints
// 5. Draw

class PrimitiveManager
{
public:

	//resize vec when more primitives in simulation
	PrimitiveManager() = default;
	//PrimitiveManager(OctTree& oTree) : octTree(oTree) {}
	~PrimitiveManager() = default;

	//void Create(PrimitiveType type, const Vector3& scale, const Vector3& translation, const Vector3& rotation, float radius = 0, float straight = 0);
	void CreatePlane(const Vector3& scale, const Vector3& translation);
	void CreateBox(const Vector3& scale, const Vector3& translation, const Vector3& rotation);
	void CreateSphere(float radius, const Vector3& translation);
	//void CreateCapsule(float radius, float straight, const Vector3& translation, const Vector3& rotation = Vector3());
	//void CreateCylinder(float radius, float length, const Vector3& translation, const Vector3& rotation = Vector3());

	void Draw();
	void Update();

	std::vector<std::unique_ptr<Primitive>>& GetPrimitives();
	//std::unique_ptr<OctTree> octTree = std::make_unique<OctTree>();

private:
	std::unique_ptr<Collisions> collisions = std::make_unique<Collisions>();
	std::unique_ptr<RayCast> rayCast = std::make_unique<RayCast>();

	bool drawContacts = false;

	std::vector<std::unique_ptr<Primitive>> primitives;

	Ray rayToTest;
	bool drawRay = false;

	//GLenum GetDrawType(PrimitiveType objectType);

	double timeSinceCollisionDebug = 0;

	int numUpdates = 0;
};
