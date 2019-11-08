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
	void Animate();

private:

	std::unique_ptr<Objects> obj = std::make_unique<Objects>();

	bool objectsInitialised = false;

};