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

	Shape* shape = new Shape();

	std::vector<Vertex> triangle;

	bool triInitialised = false;

protected:

};