#pragma once
#include "Utilities.h"
#include "PrimitiveManager.h"

class Engine
{
public:
	Engine();
	~Engine() = default;

	void Init();
	void Update(double deltaTime);
	void Render();

private:

	std::unique_ptr<PrimitiveManager> primitiveManager = std::make_unique<PrimitiveManager>();

	bool objectsInitialised = false;

};