#include "PrimitiveManager.h"
#include "Collisions.h"
#include "Shapes.h"
#include "OctTree.h"

using namespace Shapes;

//pass by const ref
void PrimitiveManager::Create(Primitive::Type type, 
	const Vector3& scale, const Vector3& translation, const Vector3& rotation, 
	float radius, float straight)
{
	//default shape
	Primitive* newObj = new Primitive();

	Quaternion orientation = Mathe::VectorToQuaternion(rotation);

	switch (type)
	{
	case Primitive::Type::BOX:
		newObj = new Primitive(ShapeVertices::GetCubeVertices());
		newObj->collisionVolume.Create(CollisionVolume::Type::BOX, translation, 0, scale, orientation, Vector3());
		break;
	case Primitive::Type::PLANE:
		newObj = new Primitive(ShapeVertices::GetPlaneVertices());
		newObj->collisionVolume.Create(CollisionVolume::Type::PLANE, translation, 0, scale, orientation, Vector3(0,1,0));
		newObj->rigidbody.useGravity = false;
		newObj->rigidbody.isKinematic = true;
		newObj->rigidbody.SetAwake(false);
		newObj->rigidbody.EnableSleep(true);
		break;
	case Primitive::Type::SPHERE:
		newObj = new Primitive(ShapeVertices::GetSphereVertices(radius, Colours::green, 12, 8), radius);
		newObj->collisionVolume.Create(CollisionVolume::Type::SPHERE, translation, radius, scale, orientation, Vector3());
		break;
	case Primitive::Type::CAPSULE:
		newObj = new Primitive(ShapeVertices::GetCapsuleVertices(radius, straight, Colours::green, 12, 9), radius);
		newObj->collisionVolume.Create(CollisionVolume::Type::CAPSULE, translation, radius, scale, orientation, Vector3(), straight);
		break;
	case Primitive::Type::CYLINDER:
		newObj = new Primitive(ShapeVertices::GetCylinderVertices(radius, straight, Colours::magenta, 12), radius);
		newObj->collisionVolume.Create(CollisionVolume::Type::CYLINDER, translation, radius, scale, orientation, Vector3(), straight);
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
	newObj->orientation = orientation;

	//newObj->Start();
	newObj->UpdateTransform();
	newObj->SetTweenOrigin();

	primitives.push_back(*newObj);

	//if (newObj->type != Primitive::Type::PLANE) octTree->Insert(*newObj, *octTree->root);
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
	if (primitives.size() > 0)
	{
		//For each object.
		for (unsigned i = 0; i < primitives.size(); i++)
		{
			primitives[i].Draw();
		}
	}

	//raycast test
	if (drawRay)
	{
		rayToTest = Ray(Vector3(0, 0.5, 0), Vector3(-1, 0, 1).Normalise());

		glBegin(GL_LINES);

		if (rayCast->Test(primitives[1], rayToTest)) glColor3f(0, 1, 0);
		else glColor3f(1, 0, 0);
		
		glVertex3f(rayToTest.origin.x, rayToTest.origin.y, rayToTest.origin.z);
		Vector3 point2 = rayToTest.IntersectionPoint();
		glVertex3f(point2.x, point2.y, point2.z);

		glEnd();
	}
	

}

void PrimitiveManager::Update()
{
	//Moves objects to illustrate collision detection.
	//primitives[2].Tween(1, Vector3(1, 0, 0), 3.5);

	for (unsigned i = 0; i < primitives.size(); i++)
	{
		primitives[i].Update();
	}

	//Custom define which objects to detect collisions between.
	//Plane
	collisions->DetectCoarse(&primitives[0], &primitives[1]);
	collisions->DetectCoarse(&primitives[0], &primitives[2]);
	//collisions->DetectCoarse(&primitives[0], &primitives[3]);
	//collisions->DetectCoarse(&primitives[0], &primitives[4]);
	//collisions->DetectCoarse(&primitives[0], &primitives[5]);
	//collisions->DetectCoarse(&primitives[0], &primitives[6]);
	//Box1													
	collisions->DetectCoarse(&primitives[1], &primitives[2]);
	//collisions->DetectCoarse(&primitives[1], &primitives[3]);
	//collisions->DetectCoarse(&primitives[1], &primitives[4]);
	//collisions->DetectCoarse(&primitives[1], &primitives[5]);
	//collisions->DetectCoarse(&primitives[1], &primitives[6]);
	//Box2													
	//collisions->DetectCoarse(&primitives[2], &primitives[3]);
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

	//Check plane collisions
	//for (unsigned i = 1; i < primitives.size(); i++)
	//{
	//	collisions->DetectCoarse(&primitives[0], &primitives[1]);
	//}
		
	//octTree->TestCollisions(*octTree->root, *collisions.get());

	collisions->DetectFine();
	for (unsigned i = 0; i < primitives.size(); i++)
	{
		primitives[i].colliding = false;
	}
	collisions->Resolution();
}

std::vector<Primitive> PrimitiveManager::GetPrimitives()
{
	return primitives;
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
