#include "Engine.h"


Engine::Engine()
{
}

void Engine::Init()
{
	//If you wish to draw more objects, this is where to define them.
	//The collisions will need to be updated in PrimitiveManager.Draw();
	primitiveManager->Create(Primitive::Type::PLANE, Vector3(64, 64, 64), Vector3(), Vector3());
	primitiveManager->Create(Primitive::Type::BOX, Vector3(1, 2, 1), Vector3(0, 6, 10), Vector3(40, 40, 0));
	primitiveManager->Create(Primitive::Type::BOX, Vector3(1, 1, 1), Vector3(0, 2, 10), Vector3(40, 0, 10));
	primitiveManager->Create(Primitive::Type::BOX, Vector3(3, 1, 3), Vector3(1, 5, 0), Vector3(0, 0, 0));
	primitiveManager->CreateSphere(3, Vector3(5, 5, 0));
	primitiveManager->CreateCapsule(3, 6, Vector3(-10, 5, -5));
	primitiveManager->CreateCylinder(3, 5, Vector3(10, 8, 10), Vector3(40, 0, 0));
	//primitiveManager->CreateSphere(2, Vector3(5, 0, -5));
}

void Engine::Update(double deltaTime)
{
	primitiveManager->Update(deltaTime);
}

void Engine::Render()
{
	primitiveManager->Draw();
}
