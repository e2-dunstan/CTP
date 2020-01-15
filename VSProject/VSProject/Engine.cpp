#include "Engine.h"


Engine::Engine()
{
}

void Engine::Init()
{
	//If you wish to draw more objects, this is where to define them.
	primitiveManager->Create(Primitive::Type::PLANE, Vector3(64, 64, 64), Vector3(0,0,0), Vector3());
	primitiveManager->Create(Primitive::Type::BOX, Vector3(1, 1, 1), Vector3(0, 10, 10), Vector3(10, 0, 10));
	//primitiveManager->Create(Primitive::Type::BOX, Vector3(1, 1, 1), Vector3(0, 15, 10), Vector3(0, 40, 0));
	//primitiveManager->Create(Primitive::Type::BOX, Vector3(3, 1, 3), Vector3(1, 5, 0), Vector3(0, 0, 0));
	//primitiveManager->CreateSphere(2, Vector3(1, 10, 10));
	//primitiveManager->CreateCapsule(2, 4, Vector3(9, 5, 3), Vector3(90, 0, 0));
	//primitiveManager->CreateCylinder(3, 10, Vector3(5, 7, 5), Vector3(0, 0, 0));
	//primitiveManager->CreateSphere(2, Vector3(5, 0, -5));
}

void Engine::Update()
{
	primitiveManager->Update();
}

void Engine::Render()
{
	primitiveManager->Draw();
}
