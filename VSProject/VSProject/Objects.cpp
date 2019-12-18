#include "Objects.h"
#include "collisions.h"
#include "mathe.h"

using namespace Shapes;

//pass by const ref
void PrimitiveManager::Create(Primitive::Type type, 
	const Vector3& scale, 
	const Vector3& translation, 
	const Vector3& rotation, 
	float radius)
{
	//default shape
	Primitive* newObj = nullptr;

	switch (type)
	{
	case Primitive::Type::BOX:
		newObj = new Primitive(ShapeVertices::GetCubeVertices());
		newObj->boundingVolume.Create(BoundingVolume::Type::BOX, translation, 0, scale);
		newObj->collisionVolume.Create(CollisionVolume::Type::BOX, translation, 0, scale, rotation, Vector3());
		break;
	case Primitive::Type::PLANE:
		newObj = new Primitive(ShapeVertices::GetPlaneVertices());
		//newObj->boundingVolume->Create(BoundingVolume::Type::BOX, translation, 0, scale);
		newObj->collisionVolume.Create(CollisionVolume::Type::PLANE, translation, 0, scale, rotation, Vector3(0,1,0));
		break;
	case Primitive::Type::SPHERE:
		newObj = new Primitive(ShapeVertices::GetSphereVertices(radius, Colours::red, 12, 8));
		newObj->boundingVolume.Create(BoundingVolume::Type::SPHERE, translation, radius, scale);
		newObj->collisionVolume.Create(CollisionVolume::Type::SPHERE, translation, radius, scale, rotation, Vector3());
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

	newObj->UpdateTransform();

	newObj->SetTweenOrigin();
	//newObj->rigidbody.AddImpulse(Vector3(0, 1, 0));

	primitives.push_back(*newObj);
	//delete newObj;
}

void PrimitiveManager::CreateSphere(float radius, const Vector3& translation)
{
	Create(Primitive::Type::SPHERE, Vector3(1, 1, 1), translation, Vector3(), radius);
}

void PrimitiveManager::Draw()
{
	if (primitives.size() <= 0) return;

	//For each object.
	for (int i = 0; i < primitives.size(); i++)
	{	
		glBegin(GetDrawType(primitives[i].type));
		
		//Only locally transform the vertices. This makes it so that each draw call
		//the vertices do not have to be cleared and redefined if their transforms have
		//changed. Performance boost.
		std::vector<Vertex> verts = primitives[i].vertices;
		for (int v = 0; v < verts.size(); v++)
		{
			//Transform the vertices.
			Mathe::Transform(verts[v].position, primitives[i].transform);

			//Set normals, colour and positions.

			glNormal3f(verts[v].normal.x, verts[v].normal.y, verts[v].normal.z);

			//For future shader use.
			//GLfloat colour[] = { objects[i].vertices[v].colour.r, objects[i].vertices[v].colour.g, objects[i].vertices[v].colour.b,  objects[i].vertices[v].colour.a};
			//glMaterialfv(GL_FRONT, GL_DIFFUSE, colour);

			glColor3f(verts[v].colour.r, verts[v].colour.g, verts[v].colour.b);
			glVertex3f(verts[v].position.x, verts[v].position.y, verts[v].position.z);
		}

		glEnd();
		
		//Debug display for bounding and collision volumes.
		//Bounding volumes have been put on hold until spatial data structures have been implemented.
		//if (drawBoundingVolumes) primitives[i]->boundingVolume->Draw();
		//if (drawCollisionVolumes) primitives[i]->collisionVolume.Draw();
	}
}

void PrimitiveManager::Update(double deltaTime)
{
	//Moves objects to illustrate collision detection.
	primitives[1].Tween(deltaTime, 1, Vector3(-0.5, 0, -1), 6);
	primitives[2].Tween(deltaTime, 1, Vector3(-0.2, 1, 0), 10);
	primitives[3].Tween(deltaTime, 1, Vector3(-1, 0, 0), 8);
	primitives[4].Tween(deltaTime, 0.5, Vector3(0, -1, 0), 5);


	for (int i = 0; i < primitives.size(); i++)
	{
		primitives[i].Update(deltaTime);
	}

	//Custom define which objects to detect collisions between.
	//Will convert to a spatial data structure in the future.
	//Console output is every quarter of a second for readability.

	//Plane
	collisions->DetectFine(&primitives[0], &primitives[1], timeSinceCollisionDebug > 0.25);
	collisions->DetectFine(&primitives[0], &primitives[2], timeSinceCollisionDebug > 0.25);
	collisions->DetectFine(&primitives[0], &primitives[3], timeSinceCollisionDebug > 0.25);
	collisions->DetectFine(&primitives[0], &primitives[4], timeSinceCollisionDebug > 0.25);
	//Box1																			 
	collisions->DetectFine(&primitives[1], &primitives[2], timeSinceCollisionDebug > 0.25);
	collisions->DetectFine(&primitives[1], &primitives[3], timeSinceCollisionDebug > 0.25);
	collisions->DetectFine(&primitives[1], &primitives[4], timeSinceCollisionDebug > 0.25);
	//Box2																			 
	collisions->DetectFine(&primitives[2], &primitives[3], timeSinceCollisionDebug > 0.25);
	collisions->DetectFine(&primitives[2], &primitives[4], timeSinceCollisionDebug > 0.25);
	//Box3																			 
	collisions->DetectFine(&primitives[3], &primitives[4], timeSinceCollisionDebug > 0.25);
		
	
	if (timeSinceCollisionDebug > 0.25)
	{
		timeSinceCollisionDebug = 0;
		//std::cout << std::endl;
	}
	timeSinceCollisionDebug += deltaTime;

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
