#include "Engine.h"
#include <time.h>
#include <random>

void Engine::Init()
{
	//Materials::ReadFrictionCoefficientsFromFile();

	//If you wish to draw more objects, this is where to define them.
	primitiveManager->CreatePlane(Vector3(64, 64, 64), Vector3(0, 0, 0));
	individualObjectInitialised.push_back(false);

	//primitiveManager->CreateBox(Vector3(1.2, 1.2, 1.2),	Vector3(-5, 5, 8),	Vector3(0, 0, 0));
	//primitiveManager->CreateBox(Vector3(1, 1, 1), Vector3(0, 3, 8),	Vector3(0, 0, 0));
	//primitiveManager->CreateBox(Vector3(1, 1, 1),		Vector3(0, 6, 8),	Vector3(90, 0, 0));

	primitiveCount += 1;

	//primitiveManager->CreateSphere(2, Vector3(0, 10, 8));
	//individualObjectInitialised.push_back(false);
	//individualObjectInitialised.push_back(false);
	//individualObjectInitialised.push_back(false);
	//individualObjectInitialised.push_back(false);
	//individualObjectInitialised.push_back(false);

	srand(time(NULL));

	//Stacks();
	//SpawnDominoes(4, Vector3(1, 2.0, 0.2), 2.0f);
	SpheresInBox();
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

	for (uint16_t i = prevPrimitiveCount; i < primitiveCount; i++)
	{
		Primitive& prim = *primitiveManager->GetPrimitives()[i].get();

		switch (prim.type)
		{
		case PrimitiveType::BOX:
			rayCamera->AddPrimitive(dynamic_cast<Box*>(&prim)->tris, &prim.transform);
			break;
		case PrimitiveType::PLANE:
			rayCamera->AddPrimitive(dynamic_cast<Plane*>(&prim)->tris, &prim.transform);
			break;
		case PrimitiveType::SPHERE:
			rayCamera->AddPrimitive(dynamic_cast<Sphere*>(&prim)->tris, &prim.transform);
			break;
		case PrimitiveType::CAPSULE:
		case PrimitiveType::CYLINDER:
		case PrimitiveType::COMPLEX:
		default:
			break;
		}
	}
	prevPrimitiveCount = primitiveCount;
}

void Engine::ThrowSphere()
{
	primitiveManager->CreateSphere(1.0, Vector3(0, 4.5, -12));
	//primitiveManager->CreateBox(Vector3(2, 2, 2), Vector3(0, 4, 0), Vector3(0, 0, 0));
	primitiveManager->GetPrimitives()[primitiveCount]->startingVelocity = Vector3(0, 0, 25);
	individualObjectInitialised.push_back(false);
	objectsInitialised = false;
	primitiveCount++;
}


void Engine::Stacks()
{
	SpawnStack(Vector3(0, 0, 8), 3, Vector3(1, 1, 1), 0.2f);
	SpawnStack(Vector3(0, 0, 4), 2, Vector3(1, 1, 1), 0.2f);
	SpawnStack(Vector3(0, 0, 0), 5, Vector3(1, 1, 1), 0.2f);
	SpawnStack(Vector3(0, 0, -4), 4, Vector3(1, 1, 1), 0.2f);
	//SpawnStack(Vector3(0, 0, 0), 8, Vector3(1, 1, 1), 0.2f);
}

void Engine::SpheresInBox()
{
	//base
	primitiveManager->CreateBox(Vector3(5.8, 0.1, 6.2), Vector3(0, -0.09, 0), Vector3(), true, Material::WOOD);
	//surrounding walls
	primitiveManager->CreateBox(Vector3(0.2, 4, 6.2), Vector3(-6, 4, 0), Vector3(), true, Material::WOOD);
	primitiveManager->CreateBox(Vector3(0.2, 4, 6.2), Vector3(6, 4, 0), Vector3(), true, Material::WOOD);
	primitiveManager->CreateBox(Vector3(5.8, 4, 0.2), Vector3(0, 4, -6), Vector3(), true, Material::WOOD);
	primitiveManager->CreateBox(Vector3(5.8, 4, 0.2), Vector3(0, 4, 6), Vector3(), true, Material::WOOD);
	//flaps - see notebook for maths
	primitiveManager->CreateBox(Vector3(0.2, 3, 6.2), Vector3(-7.4 - (sin(20) * 1.5), 7.6 - (cos(20) * 1.5), 0), Vector3(0, 0, -70), true, Material::WOOD);
	primitiveManager->CreateBox(Vector3(0.2, 3, 6.2), Vector3(7.4 + (sin(20) * 1.5), 7.6 - (cos(20) * 1.5), 0), Vector3(0, 0, 70), true, Material::WOOD);
	primitiveManager->CreateBox(Vector3(6.0, 3, 0.2), Vector3(0, 7.6 - (cos(20) * 1.5), -7.4 - (sin(20) * 1.5)), Vector3(70, 0, 0), true, Material::WOOD);
	primitiveManager->CreateBox(Vector3(6.0, 3, 0.2), Vector3(0, 7.6 - (cos(20) * 1.5), 7.4 + (sin(20) * 1.5)), Vector3(-70, 0, 0), true, Material::WOOD);
	
	primitiveCount += 9;
	for(uint16_t i = 0; i < 9; i++)
		individualObjectInitialised.push_back(false);

	//Spheres
	for (uint16_t j = 0; j < 100; j++)
	{
		float radius = ((rand() % 5) + 1.0f) / 2.0f;
		float y = ((rand() % 100) / 10.0f) + 10.0f;
		Vector3 pos = Vector3((rand() % 9 - 5), y, (rand() % 9 - 5));
		primitiveManager->CreateSphere(radius, pos, Material::CONCRETE);
		individualObjectInitialised.push_back(false);
		primitiveCount++;
	}

	objectsInitialised = false;
}

void Engine::SpawnRandomSphere()
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

void Engine::SpawnRandomBox()
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
		primitiveManager->CreateBox(size * (float)r, pos, Vector3(0, i * 45.0, 0));
		pos += Vector3(0, size.y * 2.0 + sizeVariance * 2.0, 0);

		individualObjectInitialised.push_back(false);
		objectsInitialised = false;
		primitiveCount++;
	}
}