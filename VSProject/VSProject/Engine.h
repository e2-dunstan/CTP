#pragma once
#include "Utilities.h"
#include "Objects.h"

class Engine
{
public:
	Engine();
	~Engine() = default;

	void Init();
	void Update();
	void Render();
	void Animate();

private:

	std::unique_ptr<Objects> obj = std::make_unique<Objects>();

	std::vector<Vertex> triangle;
	std::vector<Vertex> cube;

	bool objectsInitialised = false;

};