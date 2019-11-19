#include "Engine.h"


Engine::Engine()
{
}

void Engine::Init()
{
	//If you wish to draw more objects, this is where to define them.
	//The collisions will need to be updated in PrimitiveManager.Draw();
	obj->Create(Primitive::Type::PLANE, Vector3(64, 64, 64), Vector3(), Vector3());
	obj->Create(Primitive::Type::BOX, Vector3(1, 2, 1), Vector3(0, 6, 10), Vector3(40, 40, 0));
	obj->Create(Primitive::Type::BOX, Vector3(1, 1, 1), Vector3(0, 2, 10), Vector3(40, 0, 10));
	obj->Create(Primitive::Type::BOX, Vector3(3, 1, 3), Vector3(1, 5, 0), Vector3(0, 0, 0));
	obj->CreateSphere(3, Vector3(5, 5, 0));
	//obj->CreateSphere(2, Vector3(5, 0, -5));
}

void Engine::Update(int deltaTime)
{
	obj->Update(deltaTime);
}

void Engine::Render()
{
	obj->Draw();
}
