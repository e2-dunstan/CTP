#include "Engine.h"
#include <time.h>

Engine::Engine()
{
}

void Engine::Init()
{
	//If you wish to draw more objects, this is where to define them.
	primitiveManager->Create(Primitive::Type::PLANE, Vector3(64, 64, 64), Vector3(0,0,0), Vector3());
	primitiveManager->Create(Primitive::Type::BOX, Vector3(1, 1, 1), Vector3(0, 15, 10), Vector3(60, 0, 0));
	//primitiveManager->Create(Primitive::Type::BOX, Vector3(1, 1, 1), Vector3(1, 15, 10), Vector3(0, 0, 0));
	//primitiveManager->Create(Primitive::Type::BOX, Vector3(3, 1, 3), Vector3(1, 5, 0), Vector3(0, 0, 0));
	primitiveManager->CreateSphere(2, Vector3(1, 10, 10));
	//primitiveManager->CreateCapsule(2, 4, Vector3(9, 5, 3), Vector3(90, 0, 0));
	//primitiveManager->CreateCylinder(3, 10, Vector3(5, 7, 5), Vector3(0, 0, 0));
	//primitiveManager->CreateSphere(2, Vector3(5, 0, -5));
	individualObjectInitialised.push_back(false);
	individualObjectInitialised.push_back(false);
	individualObjectInitialised.push_back(false);

	primitiveCount += 3;
}

void Engine::Update()
{
	if (!objectsInitialised)
	{
		for (int i = 0; i < primitiveManager->GetPrimitives().size(); i++)
		{
			if (!individualObjectInitialised[i])
			{
				primitiveManager->GetPrimitives()[i].Start();
				individualObjectInitialised[i] = true;
			}
		}
		objectsInitialised = true;
	}

	if (primitiveCount != primitiveManager->GetPrimitives().size())
	{
		//rebuild spatial data structures
	}

	primitiveManager->Update();
}

void Engine::Render()
{
	primitiveManager->Draw();
}

void Engine::SpawnSphere()
{
	srand(time(NULL));

	float radius = (rand() % 10 + 1) / 2;
	Vector3 pos((double)(rand() % 20 + radius), (double)(rand() % 20 + radius), (double)(rand() % 20 + radius));

	std::string sphere = "Spawning SPHERE with position (";
	sphere += std::to_string(pos.x) + "," + std::to_string(pos.y) + "," + std::to_string(pos.z) + "), ";
	sphere += "and radius " + std::to_string(radius);

	std::cout << sphere << std::endl;
	primitiveManager->CreateSphere(radius, pos);

	individualObjectInitialised.push_back(false);
	objectsInitialised = false;
	primitiveCount++;
}

void Engine::SpawnBox()
{
	srand(time(NULL));

	Vector3 scale((double)(rand() % 5 + 1), (double)(rand() % 5 + 1), (double)(rand() % 5 + 1));
	Vector3 pos((double)(rand() % 20 + scale.x), (double)(rand() % 20 + scale.y), (double)(rand() % 20 + scale.z));
	Vector3 rot((double)(rand() % 10), (double)(rand() % 10), (double)(rand() % 10));

	std::string box = "Spawning BOX with position (";
	box += std::to_string(pos.x) + "," + std::to_string(pos.y) + "," + std::to_string(pos.z) + "), ";
	box += "scale (" + std::to_string(scale.x) + "," + std::to_string(scale.y) + "," + std::to_string(scale.z) + "), ";
	box += "and rotation (" + std::to_string(rot.x) + "," + std::to_string(rot.y) + "," + std::to_string(rot.z) + "), ";;

	std::cout << box << std::endl;
	primitiveManager->Create(Primitive::Type::BOX, scale, pos, rot);

	individualObjectInitialised.push_back(false);
	objectsInitialised = false;
	primitiveCount++;
}
