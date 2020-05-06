#include "Engine.h"
#include <ctime>
#include <random>

void Engine::Init()
{
	std::srand(time(0));

	primitiveManager->CreatePlane(Vector3(64, 64, 64), Vector3(0, 0, 0), Vector3());
	individualObjectInitialised.push_back(false);	
	primitiveCount++;

	//primitiveManager->CreateBox(Vector3(1, 1, 1), Vector3(0, 3, 8),	Vector3(0, 0, 0));
	//individualObjectInitialised.push_back(false);
	//primitiveCount++;
	//primitiveManager->CreateSphere(1, Vector3(0, 2, 8));
	//individualObjectInitialised.push_back(false);
	//primitiveCount++;


	//Scene_Dominoes();
	Scene_Stacks();
	//Scene_Castle();
	//Scene_SpheresInBox();
	//Scene_Slopes();
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

	//UpdateTrisForRayCamera();
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

		if (prim.type == PrimitiveType::SPHERE)
		{
			rayCamera->AddPrimitive(prim.tris, prim.transform, 
				dynamic_cast<Sphere*>(&prim)->radius, prim.translation);
		}
		else
		{
			rayCamera->AddPrimitive(prim.tris, prim.transform);
		}

		//switch (prim.type)
		//{
		//case PrimitiveType::SPHERE:
		//	rayCamera->AddPrimitive(prim.tris, prim.transform,
		//		dynamic_cast<Sphere*>(&prim)->radius);
		//	break;
		//default:
		//	break;
		//}
	}
	prevPrimitiveCount = primitiveCount;
}

void Engine::ThrowSphere()
{
	primitiveManager->CreateSphere(1.0, Vector3(0, 4.5, -10), Material::RUBBER);
	//primitiveManager->CreateBox(Vector3(0.5, 0.5, 0.5), Vector3(0, 4, -2), Vector3(0, 0, 0), false, Material::WOOD);
	primitiveManager->GetPrimitives()[primitiveCount]->startingVelocity = Vector3(0, 0, 20);
	individualObjectInitialised.push_back(false);
	objectsInitialised = false;
	primitiveCount++;
}


void Engine::Scene_Stacks()
{
	SpawnStack(Vector3(0, 0, 8), 3, Vector3(1, 1, 1), 0.2f);
	SpawnStack(Vector3(0, 0, 4), 2, Vector3(1, 1, 1), 0.2f);
	SpawnStack(Vector3(0, 0, 0), 5, Vector3(1, 1, 1), 0.2f);
	SpawnStack(Vector3(0, 0, -4), 4, Vector3(1, 1, 1), 0.2f);
}

void Engine::Scene_Castle()
{
	for (uint16_t i = 0; i < 3; i++)
	{
		float yOffset = 3.0f + (i * 3.0f);
		primitiveManager->CreateBox(Vector3(1.5f, 1.5f, 1.5f), Vector3(-6.5f, yOffset, 10), Vector3());
		primitiveManager->CreateBox(Vector3(1.5f, 1.5f, 1.5f), Vector3(6.5f, yOffset, 10), Vector3());
		primitiveManager->CreateBox(Vector3(1.5f, 1.5f, 1.5f), Vector3(-6.5f, yOffset, 23), Vector3());
		primitiveManager->CreateBox(Vector3(1.5f, 1.5f, 1.5f), Vector3(6.5f, yOffset, 23), Vector3());
		primitiveCount += 4;
		for(uint16_t j = 0; j < 4; j++) individualObjectInitialised.push_back(false);
	}
	
	for (uint16_t i = 0; i < 2; i++)
	{
		primitiveManager->CreateBox(Vector3(1, 1, 1), Vector3(2, 1.5f, 10 + (i == 1 ? 13: 0)), Vector3());
		primitiveManager->CreateBox(Vector3(1, 1, 1), Vector3(-2, 1.5f, 10 + (i == 1 ? 13 : 0)), Vector3());
		primitiveManager->CreateBox(Vector3(1, 1, 1), Vector3(4, 1.5f, 10 + (i == 1 ? 13 : 0)), Vector3());
		primitiveManager->CreateBox(Vector3(1, 1, 1), Vector3(-4, 1.5f, 10 + (i == 1 ? 13 : 0)), Vector3());
		primitiveManager->CreateBox(Vector3(1, 1, 1), Vector3(3, 3.5f, 10 + (i == 1 ? 13 : 0)), Vector3());
		primitiveManager->CreateBox(Vector3(1, 1, 1), Vector3(-3, 3.5f, 10 + (i == 1 ? 13 : 0)), Vector3());
		primitiveManager->CreateBox(Vector3(4.5f, 1, 1), Vector3(0, 5.5f, 10 + (i == 1 ? 13 : 0)), Vector3());
		primitiveCount += 7;
		for (uint16_t j = 0; j < 7; j++) individualObjectInitialised.push_back(false);
	}
	for (uint16_t i = 0; i < 2; i++)
	{
		primitiveManager->CreateBox(Vector3(1, 1, 1), Vector3(6.5f * (i == 1 ? 1.0f : -1.0f), 1.5f, 2 + 16.5f), Vector3());
		primitiveManager->CreateBox(Vector3(1, 1, 1), Vector3(6.5f * (i == 1 ? 1.0f : -1.0f), 1.5f, -2 + 16.5f), Vector3());
		primitiveManager->CreateBox(Vector3(1, 1, 1), Vector3(6.5f * (i == 1 ? 1.0f : -1.0f), 1.5f, 4 + 16.5f), Vector3());
		primitiveManager->CreateBox(Vector3(1, 1, 1), Vector3(6.5f * (i == 1 ? 1.0f : -1.0f), 1.5f, -4 + 16.5f), Vector3());
		primitiveManager->CreateBox(Vector3(1, 1, 1), Vector3(6.5f * (i == 1 ? 1.0f : -1.0f), 3.5f, 3 + 16.5f), Vector3());
		primitiveManager->CreateBox(Vector3(1, 1, 1), Vector3(6.5f * (i == 1 ? 1.0f : -1.0f), 3.5f, -3 + 16.5f), Vector3());
		primitiveManager->CreateBox(Vector3(1, 1, 4.5f), Vector3(6.5f * (i == 1 ? 1.0f : -1.0f), 5.5f, 16.5f), Vector3());
		primitiveCount += 7;
		for (uint16_t j = 0; j < 7; j++) individualObjectInitialised.push_back(false);
	}
	objectsInitialised = false;
}

void Engine::Scene_SpheresInBox()
{
	//base
	primitiveManager->CreateBox(Vector3(5.8f, 0.1f, 6.2f), Vector3(0, -0.09f, 0), Vector3(), true, Material::WOOD);
	//surrounding walls
	primitiveManager->CreateBox(Vector3(0.2f, 4, 6.2f), Vector3(-6, 4, 0), Vector3(), true, Material::WOOD);
	primitiveManager->CreateBox(Vector3(0.2f, 4, 6.2f), Vector3(6, 4, 0), Vector3(), true, Material::WOOD);
	primitiveManager->CreateBox(Vector3(5.8f, 4, 0.2f), Vector3(0, 4, -6), Vector3(), true, Material::WOOD);
	primitiveManager->CreateBox(Vector3(5.8f, 4, 0.2f), Vector3(0, 4, 6), Vector3(), true, Material::WOOD);
	//flaps - see notebook for maths
	primitiveManager->CreateBox(Vector3(0.2f, 3, 6.2f), Vector3(-7.4f - (sinf(20) * 1.5f), 7.6f - (cosf(20) * 1.5f), 0), Vector3(0, 0, -70), true, Material::WOOD);
	primitiveManager->CreateBox(Vector3(0.2f, 3, 6.2f), Vector3(7.4f + (sinf(20) * 1.5f), 7.6f - (cosf(20) * 1.5f), 0), Vector3(0, 0, 70), true, Material::WOOD);
	primitiveManager->CreateBox(Vector3(6.0f, 3, 0.2f), Vector3(0, 7.6f - (cosf(20) * 1.5f), -7.4f - (sinf(20) * 1.5f)), Vector3(70, 0, 0), true, Material::WOOD);
	primitiveManager->CreateBox(Vector3(6.0f, 3, 0.2f), Vector3(0, 7.6f - (cosf(20) * 1.5f), 7.4f + (sinf(20) * 1.5f)), Vector3(-70, 0, 0), true, Material::WOOD);
	
	primitiveCount += 9;
	for(uint16_t i = 0; i < 9; i++)
		individualObjectInitialised.push_back(false);

	//Spheres
	for (uint16_t j = 0; j < 100; j++)
	{
		float radius = ((rand() % 5) + 1.0f) / 4.0f;
		float y = ((rand() % 100) / 10.0f) + 10.0f;
		Vector3 pos = Vector3(float(rand() % 9 - 5), y, float(rand() % 9 - 5));
		primitiveManager->CreateSphere(radius, pos, Material::GLASS);
		individualObjectInitialised.push_back(false);
		primitiveCount++;
	}

	objectsInitialised = false;
}

void Engine::Scene_Dominoes()
{
	Vector3 dir = Vector3(0, 0, 1);
	SpawnDominoes(10, Vector3(1.0f, 2.0f, 0.1f), Vector3(), dir, 2, true);
	
	///primitiveManager->CreatePlane(Vector3(3, 3, 3), Vector3(0, 0.5f, 0), Material::CONCRETE);
	//primitiveManager->CreateBox(Vector3(3, 1, 4), Vector3(0, -0.5f, 1), Vector3(0, 0, 0), true, Material::CONCRETE);
	//individualObjectInitialised.push_back(false);
	//primitiveCount++;

	/*primitiveManager->CreateBox(Vector3(1.0f, 2.0f, 0.3f), Vector3(0.8f, 4, 16.4f), Vector3(0, 30, 0));
	individualObjectInitialised.push_back(false);
	primitiveCount++;

	primitiveManager->CreateBox(Vector3(1.0f, 2.0f, 0.3f), Vector3(2, 4, 18), Vector3(0, 60, 0));
	individualObjectInitialised.push_back(false);
	primitiveCount++;

	dir = Vector3(1, 0, 0);
	SpawnDominoes(3, Vector3(0.3f, 2.0f, 1.0f), Vector3(3.4f, 0, 19), dir, 2.0f);

	primitiveManager->CreateBox(Vector3(1.0f, 2.0f, 0.3f), Vector3(8, 4, 18), Vector3(0, 30, 0));
	individualObjectInitialised.push_back(false);
	primitiveCount++;

	primitiveManager->CreateBox(Vector3(1.0f, 2.0f, 0.3f), Vector3(2, 4, 18), Vector3(0, 60, 0));
	individualObjectInitialised.push_back(false);
	primitiveCount++;*/
}

void Engine::Scene_Slopes()
{
	//primitiveManager->CreatePlane(Vector3(5, 0, 5), Vector3(-10, 3.5f, 9.2f), Vector3(-45, 0, 0));
	//individualObjectInitialised.push_back(false);
	//primitiveCount++;
	primitiveManager->CreatePlane(Vector3(5, 5, 5), Vector3(-10, 0.5f, 10.8f), Vector3(-5, 0, 0));
	individualObjectInitialised.push_back(false);
	primitiveCount++;
	primitiveManager->CreatePlane(Vector3(5, 5, 5), Vector3(0, 2.5f, 10), Vector3(-30, 0, 0));
	individualObjectInitialised.push_back(false);
	primitiveCount++;
	primitiveManager->CreatePlane(Vector3(5, 5, 5), Vector3(10, 1.3f, 10.5f), Vector3(-15, 0, 0));
	individualObjectInitialised.push_back(false);
	primitiveCount++;

	Material boxMaterials[3] = { Material::ICE, Material::WOOD, Material::METAL };
	uint16_t matIndex = 0;
	for (int16_t i = -12; i < 12; i += 3)
	{
		primitiveManager->CreateBox(Vector3(0.5, 0.5, 0.5), Vector3(i, 10, 12), Vector3(), false, boxMaterials[matIndex % 3]);
		individualObjectInitialised.push_back(false);
		primitiveCount++;
		matIndex++;
	}
	objectsInitialised = false;
}

void Engine::Scene_Cover()
{
	SpawnStack(Vector3(2, 0, 3), 3, Vector3(1, 1, 1), 0.3f);
	SpawnStack(Vector3(-2, 0, -4), 2, Vector3(1, 1, 1), 0.3f);
	SpawnStack(Vector3(0, 0, 0), 5, Vector3(1, 1, 1), 0.3f);
	SpawnStack(Vector3(2, 0, -2), 4, Vector3(1, 1, 1), 0.5f);

	//primitiveManager->CreateSphere(1.2f, Vector3(-6, 2, 5));
	//primitiveManager->CreateSphere(1.5f, Vector3(-4, 2, 0));
	//primitiveManager->CreateSphere(1, Vector3(-1, 2, -3));
	//primitiveCount += 1;
	//individualObjectInitialised.push_back(false);
	//individualObjectInitialised.push_back(false);
	//individualObjectInitialised.push_back(false);
	objectsInitialised = false;
}

void Engine::SpawnRandomSphere()
{
	float radius = ((rand() % 10) + 1.0f) / 5.0f;
	Vector3 pos((float)(rand() % 10) - 5, (float)(rand() % 20) + radius, (float)(rand() % 10));

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
	Vector3 scale((float)(rand() % 20) + 2.0f, (float)(rand() % 20) + 2.0f, (float)(rand() % 20) + 2.0f);
	scale /= 10.0;
	Vector3 pos((float)(rand() % 10) - 5, (float)(rand() % 20) + 5.0f, (float)(rand() % 10));
	pos += scale;
	Vector3 rot((float)(rand() % 180) - 90.0f, (float)(rand() % 180) - 90.0f, (float)(rand() % 180) - 90.0f);

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

void Engine::SpawnDominoes(const unsigned int count, const Vector3& size, const Vector3& origin, Vector3& dir, const float spacing, const bool startTilted)
{
	//position with y offset
	Vector3 pos = origin + Vector3(0, size.y * 2.0f, 0);

	Vector3 tilt = Vector3();
	if (startTilted)
	{
		if (dir.IsRoughlyEqualTo(Vector3(1, 0, 0))) tilt = Vector3(0, 0, -25);
		else if (dir.IsRoughlyEqualTo(Vector3(0, 0, 1))) tilt = Vector3(25, 0, 0);
		else if (dir.IsRoughlyEqualTo(Vector3(0, 0, -1))) tilt = Vector3(-25, 0, 0);
		else if (dir.IsRoughlyEqualTo(Vector3(-1, 0, 0))) tilt = Vector3(0, 0, 25);
	}

	for (unsigned int i = 0; i < count; i++)
	{
		primitiveManager->CreateBox(size, pos, (i == 0 && startTilted) ? tilt : Vector3(0, 0, 0));
		pos += dir * spacing;

		individualObjectInitialised.push_back(false);
		objectsInitialised = false;
		primitiveCount++;
	}
}

void Engine::SpawnStack(const Vector3& origin, const unsigned int count, const Vector3& size, const float sizeVariance)
{
	Vector3 pos = Vector3(origin.x, size.y * 2.0f + 1.0f, origin.z);
	for (unsigned int i = 0; i < count; i++)
	{
		float r = ((rand() % (int)(sizeVariance * 20.0f)) / 10.0f) - sizeVariance + 1.0f;
		primitiveManager->CreateBox(size * r, pos, Vector3(0, i * 10.0f, 0));
		pos += Vector3(0, size.y * 2.0f + sizeVariance * 2.0f, 0);

		individualObjectInitialised.push_back(false);
		objectsInitialised = false;
		primitiveCount++;
	}
}