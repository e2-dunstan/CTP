#pragma once
#include "Utilities.h"

#include "Shape.h"

class Engine
{
public:
	Engine();
	~Engine() = default;

	void Update();
	void Render();

private:

	Objects* shape = new Objects();

	std::vector<Vertex> triangle;

	bool triInitialised = false;

protected:

};