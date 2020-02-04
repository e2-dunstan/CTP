#pragma once
#include "OctTree.h"
#include "PrimitiveManager.h"

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

private:

	unsigned primitiveCount = 0;
	std::unique_ptr<PrimitiveManager> primitiveManager = std::make_unique<PrimitiveManager>();

	bool objectsInitialised = false;
	std::vector<bool> individualObjectInitialised;

protected:
	void SpawnSphere();
	void SpawnBox();
};