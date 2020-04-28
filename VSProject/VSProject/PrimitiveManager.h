#pragma once
#include "Primitive.h"
#include "Collisions.h"
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

	PrimitiveManager() = default;
	~PrimitiveManager() = default;

	void CreatePlane(const Vector3& scale, const Vector3& translation, const Material mat = Material::CONCRETE);
	void CreateBox(const Vector3& scale, const Vector3& translation, const Vector3& rotation, bool isStatic = false, const Material mat = Material::WOOD);
	void CreateSphere(float radius, const Vector3& translation, const Material mat = Material::RUBBER);
	//void CreateCapsule(float radius, float straight, const Vector3& translation, const Vector3& rotation = Vector3());
	//void CreateCylinder(float radius, float length, const Vector3& translation, const Vector3& rotation = Vector3());

	void Draw();
	void Update();

	std::vector<std::shared_ptr<Primitive>>& GetPrimitives();

private:

	Colour GetMaterialColour(const Material mat);

	std::unique_ptr<Collisions> collisions = std::make_unique<Collisions>();

	bool drawContacts = false;

	std::vector<std::shared_ptr<Primitive>> primitives;

	Ray rayToTest;
	bool drawRay = false;

	double timeSinceCollisionDebug = 0;

	int numUpdates = 0;
};
