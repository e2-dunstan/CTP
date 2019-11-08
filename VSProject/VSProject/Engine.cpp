#include "Engine.h"


Engine::Engine()
{
}

void Engine::Init()
{
	obj->Create(Primitive::Type::PLANE, Vector3(64, 64, 64), Vector3(), Vector3());
	//obj->Create(Primitive::Type::BOX, Vector3(1.5, 1.5, 1.5), Vector3(5, 10, 5), Vector3(0, 45, 45));
	//obj->Create(Primitive::Type::BOX, Vector3(3, 1, 1), Vector3(0, 2, 10), Vector3(0, 30, 0));
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
