#include "PrimitiveManager.h"
#include "Collisions.h"
#include "Shapes.h"

using namespace Shapes;

//pass by const ref
void PrimitiveManager::Create(Primitive::Type type, 
	const Vector3& scale, 
	const Vector3& translation, 
	const Vector3& rotation, 
	float radius,
	float straight)
{
	//default shape
	Primitive* newObj = nullptr;

	switch (type)
	{
	case Primitive::Type::BOX:
		newObj = new Primitive(ShapeVertices::GetCubeVertices());
		newObj->collisionVolume.Create(CollisionVolume::Type::BOX, translation, 0, scale, rotation, Vector3());
		break;
	case Primitive::Type::PLANE:
		newObj = new Primitive(ShapeVertices::GetPlaneVertices());
		newObj->collisionVolume.Create(CollisionVolume::Type::PLANE, translation, 0, scale, rotation, Vector3(0,1,0));
		newObj->rigidbody.useGravity = false;
		newObj->rigidbody.isKinematic = true;
		break;
	case Primitive::Type::SPHERE:
		newObj = new Primitive(ShapeVertices::GetSphereVertices(radius, Colours::green, 12, 8), radius);
		newObj->collisionVolume.Create(CollisionVolume::Type::SPHERE, translation, radius, scale, rotation, Vector3());
		break;
	case Primitive::Type::CAPSULE:
		newObj = new Primitive(ShapeVertices::GetCapsuleVertices(radius, straight, Colours::green, 12, 9), radius);
		newObj->collisionVolume.Create(CollisionVolume::Type::CAPSULE, translation, radius, scale, rotation, Vector3(), straight);
		break;
	case Primitive::Type::CYLINDER:
		newObj = new Primitive(ShapeVertices::GetCylinderVertices(radius, straight, Colours::magenta, 12), radius);
		newObj->collisionVolume.Create(CollisionVolume::Type::CYLINDER, translation, radius, scale, rotation, Vector3(), straight);
		break;
	case Primitive::Type::COMPLEX:
		break;
	default:
		newObj = new Primitive(ShapeVertices::GetCubeVertices());
		break;
	}
	newObj->type = type;

	newObj->scale = scale;
	newObj->translation = translation;
	newObj->rotation = rotation;

	newObj->Start();
	newObj->UpdateTransform();
	newObj->SetTweenOrigin();

	primitives.push_back(*newObj);
	//delete newObj;
}

void PrimitiveManager::CreateSphere(float radius, const Vector3& translation)
{
	Create(Primitive::Type::SPHERE, Vector3(1, 1, 1), translation, Vector3(), radius);
}

void PrimitiveManager::CreateCapsule(float radius, float straight, const Vector3& translation, const Vector3& rotation)
{
	Create(Primitive::Type::CAPSULE, Vector3(1, 1, 1), translation, rotation, radius, straight);
}

void PrimitiveManager::CreateCylinder(float radius, float length, const Vector3& translation, const Vector3& rotation)
{
	Create(Primitive::Type::CYLINDER, Vector3(1, 1, 1), translation, rotation, radius, length);
}

void PrimitiveManager::Draw()
{
	if (primitives.size() <= 0) return;

	//For each object.
	for (int i = 0; i < primitives.size(); i++)
	{	
		primitives[i].Draw();
	}
}

void PrimitiveManager::Update()
{
	//Moves objects to illustrate collision detection.
	//primitives[2].Tween(1, Vector3(1, 0, 0), 3.5);

	for (int i = 0; i < primitives.size(); i++)
	{
		/*if (numUpdates == 10)
		{
			primitives[i].rigidbody.Start();
		}*/
		primitives[i].Update();
	}
	/*
	if (numUpdates <= 10)
	{
		numUpdates++;
	}
	*/
	//Custom define which objects to detect collisions between.
	//Will convert to a spatial data structure in the future.

	//Plane
	collisions->DetectCoarse(&primitives[0], &primitives[1]);
	collisions->DetectCoarse(&primitives[0], &primitives[2]);
	collisions->DetectCoarse(&primitives[0], &primitives[3]);
	//collisions->DetectCoarse(&primitives[0], &primitives[4]);
	//collisions->DetectCoarse(&primitives[0], &primitives[5]);
	//collisions->DetectCoarse(&primitives[0], &primitives[6]);
	//Box1													
	collisions->DetectCoarse(&primitives[1], &primitives[2]);
	collisions->DetectCoarse(&primitives[1], &primitives[3]);
	//collisions->DetectCoarse(&primitives[1], &primitives[4]);
	//collisions->DetectCoarse(&primitives[1], &primitives[5]);
	//collisions->DetectCoarse(&primitives[1], &primitives[6]);
	//Box2													
	collisions->DetectCoarse(&primitives[2], &primitives[3]);
	//collisions->DetectCoarse(&primitives[2], &primitives[4]);
	//collisions->DetectCoarse(&primitives[2], &primitives[5]);
	//collisions->DetectCoarse(&primitives[2], &primitives[6]);
	////Box3												
	//collisions->DetectCoarse(&primitives[3], &primitives[4]);
	//collisions->DetectCoarse(&primitives[3], &primitives[5]);
	//collisions->DetectCoarse(&primitives[3], &primitives[6]);
	////Sphere
	//collisions->DetectCoarse(&primitives[4], &primitives[5]);
	//collisions->DetectCoarse(&primitives[4], &primitives[6]);
	////Capsule
	//collisions->DetectCoarse(&primitives[5], &primitives[6]);
		
	collisions->DetectFine();
	for (int i = 0; i < primitives.size(); i++)
	{
		primitives[i].colliding = false;
	}
	collisions->Resolution();
}

GLenum PrimitiveManager::GetDrawType(Primitive::Type objectType)
{
	switch (objectType)
	{
		case Primitive::Type::SPHERE:
		{
			return GL_TRIANGLES;
		}
		case Primitive::Type::BOX:
		default:
		{
			return GL_QUADS;
		}
	}
}
