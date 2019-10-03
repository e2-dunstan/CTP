#include "Engine.h"


Engine::Engine()
{
}

void Engine::Update()
{
	if (triInitialised) return;

	triangle.push_back(Vertex(0.5, -0.5, 0.0));
	triangle.push_back(Vertex(0.5, 0.5, 0.0));
	triangle.push_back(Vertex(-0.5, 0.5, 0.0));

	triInitialised = true;
}

void Engine::Render()
{
	shape->Draw(triangle);
}
