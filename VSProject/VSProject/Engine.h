#pragma once
//#include "OctTree.h"
#include "RayCamera.h"
#include "PrimitiveManager.h"

class ConsoleControls;

class Engine
{
public:
	Engine() = default;
	~Engine() = default;

	friend ConsoleControls;

	void Init();
	void Update();
	void Render();

	void Scene_Stacks();
	void Scene_Castle();
	void Scene_SpheresInBox();
	void Scene_Dominoes();
	void Scene_Slopes();
	void Scene_Cover();
	void Scene_Spheres();
	void Scene_Restitution();

	void SpawnRandomSphere();
	void SpawnRandomBox();

	void SpawnDominoes(const unsigned int count, const Vector3& size, const Vector3& origin, Vector3& dir, const float spacing, const bool startTilted = false);
	void SpawnStack(const Vector3& origin, const unsigned int count, const Vector3& size, const float sizeVariance);

	void UpdateTrisForRayCamera();

	std::unique_ptr<PrimitiveManager> primitiveManager = std::make_unique<PrimitiveManager>();
	std::unique_ptr<RayCamera> rayCamera = std::make_unique<RayCamera>();

private:


	bool objectsInitialised = false;
	std::vector<bool> individualObjectInitialised;
	unsigned primitiveCount = 0;
	unsigned prevPrimitiveCount = 0;

protected:

	void ThrowSphere();
};