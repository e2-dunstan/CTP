#include "PrimitiveManager.h"
#include "Collisions.h"
#include "Shapes.h"
//#include "OctTree.h"

using namespace Shapes;

/*void PrimitiveManager::Create(PrimitiveType type, 
	const Vector3& scale, const Vector3& translation, const Vector3& rotation, 
	float radius, float straight)
{
	//default shape
	Primitive* newObj;// = new Primitive();

	Quaternion orientation = Mathe::VectorToQuaternion(rotation);

	switch (type)
	{
	case PrimitiveType::BOX:
		newObj = new Box(ShapeVertices::GetCubeVertices());
		newObj->collisionVolume.Create(CollisionVolume::PrimitiveType::BOX, translation, 0, scale, orientation, Vector3());
		newObj->rigidbody.bounciness = 0.1f;
		break;

	case PrimitiveType::PLANE:
		newObj = new Primitive(ShapeVertices::GetPlaneVertices());
		newObj->collisionVolume.Create(CollisionVolume::PrimitiveType::PLANE, translation, 0, scale, orientation, Vector3(0,1,0));
		newObj->rigidbody.useGravity = false;
		newObj->rigidbody.isKinematic = true;
		newObj->rigidbody.SetAwake(false);
		newObj->rigidbody.EnableSleep(true);
		break;

	case PrimitiveType::SPHERE:
		newObj = new Primitive(ShapeVertices::GetSphereVertices(radius, Colours::green, 12, 8), radius);
		newObj->collisionVolume.Create(CollisionVolume::PrimitiveType::SPHERE, translation, radius, scale, orientation, Vector3());
		newObj->rigidbody.bounciness = 0.8f;
		break;

	case PrimitiveType::CAPSULE:
		newObj = new Primitive(ShapeVertices::GetCapsuleVertices(radius, straight, Colours::green, 12, 9), radius);
		newObj->collisionVolume.Create(CollisionVolume::PrimitiveType::CAPSULE, translation, radius, scale, orientation, Vector3(), straight);
		break;

	case PrimitiveType::CYLINDER:
		newObj = new Primitive(ShapeVertices::GetCylinderVertices(radius, straight, Colours::magenta, 12), radius);
		newObj->collisionVolume.Create(CollisionVolume::PrimitiveType::CYLINDER, translation, radius, scale, orientation, Vector3(), straight);
		break;

	case PrimitiveType::COMPLEX:
		break;

	default:
		newObj = new Box(ShapeVertices::GetCubeVertices());
		break;
	}
	newObj->type = type;

	newObj->scale = scale;
	newObj->translation = translation;
	newObj->orientation = orientation;

	//newObj->Start();
	newObj->UpdateTransform();
	//newObj->SetTweenOrigin();

	primitives.push_back(*newObj);

	//if (newObj->type != PrimitiveType::PLANE) octTree->Insert(*newObj, *octTree->root);
	//delete newObj;
}*/

void PrimitiveManager::CreatePlane(const Vector3& scale, const Vector3& translation)
{
	Plane plane = Plane(ShapeVertices::GetPlaneVertices());
	plane.collisionVolume.Create(translation, Vector3(0, 1, 0));
	plane.rigidbody.useGravity = false;
	plane.rigidbody.isKinematic = true;
	plane.rigidbody.SetAwake(false);
	plane.rigidbody.EnableSleep(true);

	plane.type = PrimitiveType::PLANE;
	plane.scale = scale;
	plane.translation = translation;
	plane.UpdateTransform();

	primitives.emplace_back(std::make_unique<Plane>(std::move(plane)));
}

void PrimitiveManager::CreateBox(const Vector3& scale, const Vector3& translation, const Vector3& rotation)
{
	Box box = Box(ShapeVertices::GetCubeVertices());
	box.collisionVolume.Create(translation, scale);
	box.rigidbody.bounciness = 0.1f;

	box.type = PrimitiveType::BOX;
	box.scale = scale;
	box.translation = translation;
	box.orientation = Mathe::VectorToQuaternion(rotation);
	box.UpdateTransform();

	primitives.emplace_back(std::make_unique<Box>(std::move(box)));
}

void PrimitiveManager::CreateSphere(float radius, const Vector3& translation)
{
	Sphere sphere = Sphere(ShapeVertices::GetSphereVertices(radius, Colours::green, 12, 8), radius);
	sphere.collisionVolume.Create(translation);
	sphere.rigidbody.bounciness = 0.8f;

	sphere.type = PrimitiveType::SPHERE;
	sphere.translation = translation;
	sphere.UpdateTransform();

	primitives.emplace_back(std::make_unique<Sphere>(std::move(sphere)));
}

//void PrimitiveManager::CreateCapsule(float radius, float straight, const Vector3& translation, const Vector3& rotation)
//{
//	Create(PrimitiveType::CAPSULE, Vector3(1, 1, 1), translation, rotation, radius, straight);
//}
//void PrimitiveManager::CreateCylinder(float radius, float length, const Vector3& translation, const Vector3& rotation)
//{
//	Create(PrimitiveType::CYLINDER, Vector3(1, 1, 1), translation, rotation, radius, length);
//}

void PrimitiveManager::Draw()
{
	if (primitives.size() > 0)
	{
		//For each object.
		for (unsigned i = 0; i < primitives.size(); i++)
		{
			primitives[i]->Draw();
		}
	}

	//raycast test
	if (drawRay)
	{
		rayToTest = Ray(Vector3(0, 0.5, 0), Vector3(-1, 0, 1).Normalise());

		glBegin(GL_LINES);

		//if (rayCast->Test(&primitives[1], rayToTest)) glColor3f(0, 1, 0);
		//else glColor3f(1, 0, 0);
		
		glVertex3f(rayToTest.origin.x, rayToTest.origin.y, rayToTest.origin.z);
		Vector3 point2 = rayToTest.IntersectionPoint();
		glVertex3f(point2.x, point2.y, point2.z);

		glEnd();
	}

	if (drawContacts) collisions->DrawContacts();
}

void PrimitiveManager::Update()
{
	unsigned primSize = primitives.size();
	//Moves objects to illustrate collision detection.
	//primitives[2].Tween(1, Vector3(1, 0, 0), 3.5);

	for (uint32_t i = 0; i < primSize; i++)
	{
		primitives[i]->Update();
		primitives[i]->colliding = false;
	}

	//Custom define which objects to detect collisions between.
		
	//octTree->TestCollisions(*octTree->root, *collisions.get());

	for (unsigned i = 0; i < primSize; i++)
	{
		for (unsigned j = i + 1; j < primSize; j++)
		{
			collisions->DetectCoarse(primitives[i].get(), primitives[j].get());
		}
	}

	collisions->DetectFine();
	collisions->Resolution();
}

std::vector<std::unique_ptr<Primitive>>& PrimitiveManager::GetPrimitives()
{
	return primitives;
}

//GLenum PrimitiveManager::GetDrawType(Primitive::PrimitiveType objectType)
//{
//	switch (objectType)
//	{
//		case PrimitiveType::SPHERE:
//		{
//			return GL_TRIANGLES;
//		}
//		case PrimitiveType::BOX:
//		default:
//		{
//			return GL_QUADS;
//		}
//	}
//}
