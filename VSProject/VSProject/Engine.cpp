#include "Engine.h"


Engine::Engine()
{
}

void Engine::Update()
{
	if (objectsInitialised) return;

	// Define vertices in anti-clockwise order with normal pointing out
	// Top  ace (y = 0.5)
	cube.push_back(Vertex(0.5, 0.5, -0.5, obj->green));
	cube.push_back(Vertex(-0.5, 0.5, -0.5, obj->green));
	cube.push_back(Vertex(-0.5, 0.5, 0.5, obj->green));
	cube.push_back(Vertex(0.5, 0.5, 0.5, obj->green));

	// Bottom  ace (y = -0.5)
	cube.push_back(Vertex(0.5, -0.5, 0.5, obj->green));
	cube.push_back(Vertex(-0.5, -0.5, 0.5, obj->green));
	cube.push_back(Vertex(-0.5, -0.5, -0.5, obj->green));
	cube.push_back(Vertex(0.5, -0.5, -0.5, obj->green));

	//  ront  ace  (z = 0.5)
	cube.push_back(Vertex(0.5, 0.5, 0.5, obj->red));
	cube.push_back(Vertex(-0.5, 0.5, 0.5, obj->red));
	cube.push_back(Vertex(-0.5, -0.5, 0.5, obj->red));
	cube.push_back(Vertex(0.5, -0.5, 0.5, obj->red));

	// Back  ace (z = -0.5)
	cube.push_back(Vertex(0.5, -0.5, -0.5, obj->red));
	cube.push_back(Vertex(-0.5, -0.5, -0.5, obj->red));
	cube.push_back(Vertex(-0.5, 0.5, -0.5, obj->red));
	cube.push_back(Vertex(0.5, 0.5, -0.5, obj->red));

	// Le t  ace (x = -0.5)
	cube.push_back(Vertex(-0.5, 0.5, 0.5, obj->blue));
	cube.push_back(Vertex(-0.5, 0.5, -0.5, obj->blue));
	cube.push_back(Vertex(-0.5, -0.5, -0.5, obj->blue));
	cube.push_back(Vertex(-0.5, -0.5, 0.5, obj->blue));

	// Right  ace (x = 0.5)
	cube.push_back(Vertex(0.5, 0.5, -0.5, obj->blue));
	cube.push_back(Vertex(0.5, 0.5, 0.5, obj->blue));
	cube.push_back(Vertex(0.5, -0.5, 0.5, obj->blue));
	cube.push_back(Vertex(0.5, -0.5, -0.5, obj->blue));


	obj->CreateObject(cube);
	//cube.push_back(Vertex(-0.5f, -0.5f, 0.5f, obj->red));
	//cube.push_back(Vertex(-0.5f, 0.5f, 0.5f, obj->red));

	objectsInitialised = true;
}

void Engine::Render()
{
	obj->Draw();
}

void Engine::Animate()
{
	obj->Animate();
}
