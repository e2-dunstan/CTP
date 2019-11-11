#include "Engine.h"


Engine::Engine()
{
}

void Engine::Init()
{
	obj->Create(Primitive::Type::PLANE, Vector3(64, 64, 64), Vector3(), Vector3());
	obj->Create(Primitive::Type::BOX, Vector3(1, 1, 1), Vector3(0, 7, 10), Vector3(40, 40, 0));
	obj->Create(Primitive::Type::BOX, Vector3(1, 1, 1), Vector3(0, 5, 10), Vector3(0, 0, 0));
	obj->Create(Primitive::Type::BOX, Vector3(1, 1, 1), Vector3(10, 0.2, 0), Vector3(0, 0, 30));
}

void Engine::Update(int deltaTime)
{
	obj->Update(deltaTime);
}

void Engine::Render()
{
	obj->Draw();
}

void Engine::Animate()
{
	obj->Animate();
}
