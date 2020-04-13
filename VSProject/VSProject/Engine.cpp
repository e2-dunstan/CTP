#include "Engine.h"
#include <time.h>
#include <random>

void Engine::Init()
{
	//primitiveManager->octTree->root = primitiveManager->octTree->Construct(Vector3(0, 32, 0), 64, 4);

	//If you wish to draw more objects, this is where to define them.
	primitiveManager->CreatePlane(Vector3(64, 64, 64), Vector3(0, 0, 0));
	individualObjectInitialised.push_back(false);

	/*primitiveManager->CreateBox(Vector3(1.2, 1.2, 1.2),	Vector3(-5, 5, 8),	Vector3(0, 0, 0));
	primitiveManager->CreateBox(Vector3(1.5, 1.5, 1.5), Vector3(-10, 15, 8),	Vector3(90, 0, 0));
	primitiveManager->CreateBox(Vector3(1, 1, 1),		Vector3(-15, 10, 8),	Vector3(90, 0, 180));*/

	//primitiveManager->CreateSphere(2, Vector3(5, 20, 5));
	//primitiveManager->CreateCapsule(2, 4, Vector3(12, 5, 3), Vector3(90, 0, 0));
	//primitiveManager->CreateCylinder(2, 5, Vector3(5, 15, 5), Vector3(0, 0, 0));
	//primitiveManager->CreateSphere(2, Vector3(10, 50, -10));
	//individualObjectInitialised.push_back(false);
	//individualObjectInitialised.push_back(false);
	//individualObjectInitialised.push_back(false);
	//individualObjectInitialised.push_back(false);
	//individualObjectInitialised.push_back(false);

	//SpawnDominoes(3, Vector3(0.5, 1.0, 0.1), 0.9f);

	srand(time(NULL));

	SpawnStack(Vector3(0, 0, 8), 2, Vector3(1, 1, 1), 0.2f);

	//SpawnBox();
	//SpawnBox();
	//SpawnBox();
	//SpawnBox();
	//SpawnBox();
	//SpawnBox();
	//SpawnBox();
	//SpawnBox();
	//SpawnBox();
	//SpawnBox();
	//SpawnSphere();
	//SpawnSphere();
	//SpawnSphere();
	//SpawnSphere();

	primitiveCount += 1;
	//primitiveCount++;
	//skip plane
	//for (unsigned i = 1; i < primitiveManager->GetPrimitives().size(); i++)
	//{
	//	//Object* newObject(primitiveManager->GetPrimitives()[i]);
	//	octTree->Insert(primitiveManager->GetPrimitives()[i], *octTree->root);
	//}
}

void Engine::Update()
{
	if (!objectsInitialised)
	{
		for (unsigned i = 0; i < primitiveManager->GetPrimitives().size(); i++)
		{
			if (!individualObjectInitialised[i])
			{
				primitiveManager->GetPrimitives()[i]->Start();
				individualObjectInitialised[i] = true;
			}
		}
		objectsInitialised = true;
	}

	/*if (primitiveCount != primitiveManager->GetPrimitives().size())
	{
		//rebuild spatial data structures
	}*/
	primitiveManager->Update();

	UpdateTrisForRayCamera();
}

void Engine::Render()
{
	primitiveManager->Draw();
	rayCamera->DrawLatestRay();
}

void Engine::UpdateTrisForRayCamera()
{
	if (prevPrimitiveCount == primitiveCount) return;
	prevPrimitiveCount = primitiveCount;

	for (uint16_t i = 0; i < primitiveCount; i++)
	{
		Primitive& prim = *primitiveManager->GetPrimitives()[i].get();

		switch (prim.type)
		{
		case PrimitiveType::BOX:
		{
			rayCamera->AddPrimitive(dynamic_cast<Box*>(&prim)->tris, &prim.transform);
			break;
		}
		case PrimitiveType::PLANE:
		{
			rayCamera->AddPrimitive(dynamic_cast<Plane*>(&prim)->tris, &prim.transform);
			break;
		}
		case PrimitiveType::SPHERE:
		case PrimitiveType::CAPSULE:
		case PrimitiveType::CYLINDER:
		case PrimitiveType::COMPLEX:
		default:
		{
			break;
		}
		}
	}
}

void Engine::SpawnSphere()
{
	float radius = ((rand() % 10) + 1.0f) / 5.0f;
	Vector3 pos((double)(rand() % 10) - 5, ((double)(rand() % 20) + radius), (double)(rand() % 10));

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
	Vector3 scale((double)(rand() % 20) + 2.0, (double)(rand() % 20) + 2.0, (double)(rand() % 20) + 2.0);
	scale /= 10.0;
	Vector3 pos((double)(rand() % 10) - 5, (double)(rand() % 20) + 5.0, (double)(rand() % 10));
	pos += scale;
	Vector3 rot((double)(rand() % 180) - 90.0, (double)(rand() % 180) - 90.0, (double)(rand() % 180) - 90.0);

	std::string box = "Spawning BOX with position (";
	box += std::to_string(pos.x) + "," + std::to_string(pos.y) + "," + std::to_string(pos.z) + "), ";
	box += "scale (" + std::to_string(scale.x) + "," + std::to_string(scale.y) + "," + std::to_string(scale.z) + "), ";
	box += "and rotation (" + std::to_string(rot.x) + "," + std::to_string(rot.y) + "," + std::to_string(rot.z) + "), ";;

	std::cout << box << std::endl;
	primitiveManager->CreateBox(scale, pos, rot);

	individualObjectInitialised.push_back(false);
	objectsInitialised = false;
	primitiveCount++;
}

void Engine::SpawnDominoes(const unsigned int count, const Vector3& size, const float spacing)
{
	Vector3 pos = Vector3(0, size.y * 2, 0);
	for (unsigned int i = 0; i < count; i++)
	{
		primitiveManager->CreateBox(size, pos, Vector3(0, 0, 0));
		pos += Vector3(0, 0, spacing);

		individualObjectInitialised.push_back(false);
		objectsInitialised = false;
		primitiveCount++;

	}
}

void Engine::SpawnStack(const Vector3& origin, const unsigned int count, const Vector3& size, const float sizeVariance)
{
	Vector3 pos = Vector3(origin.x, size.y * 2.0 + 1.0, origin.z);
	for (unsigned int i = 0; i < count; i++)
	{
		float r = ((rand() % (int)(sizeVariance * 20.0f)) / 10.0f) - sizeVariance + 1.0f;
		std::cout << r << std::endl;
		primitiveManager->CreateBox(size * (float)r, pos, Vector3(0, i * 0.0f, 0));
		pos += Vector3(0, size.y * 2.0 + sizeVariance * 2.0f, 0);

		individualObjectInitialised.push_back(false);
		objectsInitialised = false;
		primitiveCount++;
	}
}

void Engine::ThrowSphere()
{
	//primitiveManager->CreateSphere(0.5, Vector3(0, 2, -5));
	primitiveManager->CreateBox(Vector3(0.5, 0.5, 0.5), Vector3(-5, 2, 0), Vector3(0, 0, 0));
	primitiveManager->GetPrimitives()[primitiveCount].get()->startingVelocity = Vector3(0, 0, 15);
	individualObjectInitialised.push_back(false);
	objectsInitialised = false;
	primitiveCount++;
}
