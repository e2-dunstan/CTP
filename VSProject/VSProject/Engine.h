#pragma once
//#include "OctTree.h"
#include "PrimitiveManager.h"
#include "RayCamera.h"

class ConsoleControls;

class Engine
{
public:
	Engine() = default;
	~Engine() = default;

	void Init();
	void Update();
	void Render();

	friend ConsoleControls;
	std::unique_ptr<RayCamera> rayCamera = std::make_unique<RayCamera>();

private:

	void UpdateTrisForRayCamera();

	unsigned prevPrimitiveCount = 0;
	unsigned primitiveCount = 0;
	std::unique_ptr<PrimitiveManager> primitiveManager = std::make_unique<PrimitiveManager>();

	bool objectsInitialised = false;
	std::vector<bool> individualObjectInitialised;

protected:
	void SpawnSphere();
	void SpawnBox();

	void SpawnDominoes(const unsigned int count, const Vector3& size, const float spacing);

	void SpawnStack(const Vector3& origin, const unsigned int count, const Vector3& size, const float sizeVariance);

	void ThrowSphere();
};