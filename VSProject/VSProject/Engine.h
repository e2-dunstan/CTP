#pragma once
#include "Utilities.h"
#include "Objects.h"

class Engine
{
public:
	Engine();
	~Engine() = default;

	void Init();
	void Update(int deltaTime);
	void Render();

private:

	std::unique_ptr<PrimitiveManager> obj = std::make_unique<PrimitiveManager>();

	bool objectsInitialised = false;

};