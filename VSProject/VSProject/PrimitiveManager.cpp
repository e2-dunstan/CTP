#include "PrimitiveManager.h"
#include "Collisions.h"
#include "Shapes.h"
//#include "OctTree.h"

using namespace Shapes;

void PrimitiveManager::CreatePlane(const Vector3& scale, const Vector3& translation, const Vector3& rotation, const Material mat)
{
	Plane plane = Plane(ShapeVertices::GetPlaneTris(primitives.size() == 0 ? Colours::grass : GetMaterialColour(mat)));

	plane.normal = Vector3(0, 1, 0);
	Matrix3 normalRot;
	plane.orientation = Mathe::VectorToQuaternion(rotation);
	Mathe::Rotate(normalRot, plane.orientation);
	Mathe::Transform(plane.normal, normalRot);
	plane.normal = plane.normal.Normalise();

	plane.collisionVolume.Create(translation, plane.normal);
	plane.rigidbody.useGravity = false;
	plane.rigidbody.isKinematic = true;
	plane.rigidbody.SetAwake(false);
	plane.rigidbody.EnableSleep(true);
	plane.isStatic = true;

	plane.rigidbody.bounciness = Materials::GetBounciness(mat);
	plane.rigidbody.material = mat;
	 
	plane.type = PrimitiveType::PLANE;
	plane.scale = scale;
	plane.translation = translation;
	plane.UpdateTransform();

	primitives.push_back(std::make_unique<Plane>(std::move(plane)));
}

void PrimitiveManager::CreateBox(const Vector3& scale, const Vector3& translation, const Vector3& rotation, bool isStatic, const Material mat)
{
	Box box = Box(ShapeVertices::GetCubeTris(GetMaterialColour(mat)));
	box.collisionVolume.Create(translation, scale);
	box.rigidbody.bounciness = Materials::GetBounciness(mat);
	box.rigidbody.material = mat;

	if (isStatic)
	{
		box.isStatic = true;
		box.rigidbody.isKinematic = true;
		box.rigidbody.useGravity = false;
		box.rigidbody.SetAwake(false);
		box.rigidbody.EnableSleep(true);
	}

	box.type = PrimitiveType::BOX;
	box.scale = scale;
	box.translation = translation;
	box.orientation = Mathe::VectorToQuaternion(rotation);
	box.UpdateTransform();

	primitives.push_back(std::make_unique<Box>(std::move(box)));
}

void PrimitiveManager::CreateSphere(float radius, const Vector3& translation, const Material mat)
{
	Sphere sphere = Sphere(ShapeVertices::GetSphereVertices(radius, GetMaterialColour(mat), 10, 10), radius);
	sphere.collisionVolume.Create(translation);
	sphere.rigidbody.bounciness = Materials::GetBounciness(mat);
	sphere.rigidbody.material = mat;

	sphere.type = PrimitiveType::SPHERE;
	sphere.translation = translation;
	sphere.UpdateTransform();

	primitives.push_back(std::make_unique<Sphere>(std::move(sphere)));
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
		rayToTest = Ray(Vector3(5, 0.5, 0), Vector3(0, 0, 1).Normalise());

		glBegin(GL_LINES);
		
		glVertex3f((GLfloat)rayToTest.origin.x, (GLfloat)rayToTest.origin.y, (GLfloat)rayToTest.origin.z);

		Vector3 point2 = rayToTest.IntersectionPoint();
		glVertex3f((GLfloat)point2.x, (GLfloat)point2.y, (GLfloat)point2.z);

		glEnd();
	}

	if (drawContacts || !Global::shouldUpdate) collisions->DrawContacts();
}

void PrimitiveManager::Update()
{
	unsigned primSize = primitives.size();

	for (uint32_t i = 0; i < primSize; i++)
	{
		primitives[i]->Update();
		primitives[i]->colliding = false;
	}

	for (unsigned i = 0; i < primSize - 1; i++)
	{
		for (unsigned j = i + 1; j < primSize; j++)
		{
			collisions->DetectCoarse(primitives[i].get(), primitives[j].get());
		}
	}

	collisions->DetectFine();
	collisions->Resolution();

	CheckAwakeStates();
}

std::vector<std::unique_ptr<Primitive>>& PrimitiveManager::GetPrimitives()
{
	return primitives;
}

void PrimitiveManager::CheckAwakeStates()
{
	Ray ray = Ray(Vector3(), Vector3(0, -1, 0));
	for (auto& prim : primitives)
	{
		if (prim->rigidbody.isAwake || prim->type == PrimitiveType::PLANE) continue;

		ray.origin = prim->translation;
		float scale = (prim->type == PrimitiveType::SPHERE) ?
			dynamic_cast<Sphere*>(prim.get())->radius * 2.0f
			: dynamic_cast<Box*>(prim.get())->scale.Magnitude();

		bool setAwake = true;
		for (auto& compPrim : primitives)
		{
			if (prim == compPrim) continue;

			switch (compPrim->type)
			{
			case PrimitiveType::PLANE:
			{
				Plane* plane = dynamic_cast<Plane*>(compPrim.get());
				if (RayCast::TestPlane(plane->translation, plane->normal, plane->scale, ray)
					&& ray.intersection1 <= scale)
				{
					setAwake = false;
				}
				break;
			}
			case PrimitiveType::SPHERE:
			{
				if (RayCast::TestSphere(compPrim->translation, dynamic_cast<Sphere*>(compPrim.get())->radius, ray)
					&& ray.intersection1 <= scale)
				{
					setAwake = false;
				}
				break;
			}
			case PrimitiveType::BOX:
			default:
			{
				for (auto tri : compPrim->tris)
				{
					if (RayCast::TestTriangle(tri, ray, &compPrim->transform)
						&& ray.intersection1 <= scale)
					{
						setAwake = false;
					}
				}
				break;
			}
			}

			if (!setAwake)
			{
				break;
			}
		}
		if (setAwake)
			prim->rigidbody.SetAwake(true);
	}
}

Colour PrimitiveManager::GetMaterialColour(const Material mat)
{
	switch (mat)
	{
	case Material::ICE:
		return Colours::lightCyan;
	case Material::GLASS:
		return Colours::lightestCyan;
	case Material::CONCRETE:
		return Colours::grey;
	case Material::WOOD:
		return Colours::brown;
	case Material::METAL:
		return Colours::darkGrey;
	case Material::SMOOTH_METAL:
		return Colours::darkGrey;
	case Material::RUBBER:
	{
		uint16_t random = rand() % 4;
		Colour colour = Colours::offWhite;
		if (random == 0) colour = Colours::deepRed;
		else if (random == 1) colour = Colour(0.9f, 0.9f, 0.05f);
		else if (random == 2) colour = Colour(0.005f, 0.9f, 0.9f);
		else colour = Colours::pink;
		return colour;
	}
	default:
		return Colours::offWhite;
	}
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
