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

	Objects* obj = new Objects();

	std::vector<Vertex> triangle;
	std::vector<Vertex> cube;

	bool objectsInitialised = false;

};