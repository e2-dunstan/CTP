#include "Objects.h"


Objects::~Objects()
{
	for (auto prim : primitives)
	{
		delete prim;
	}
	primitives.clear();
}

void Objects::Create(Primitive::Type type, Vector3 scale, Vector3 translation, Vector3 rotation, float radius)
{
	//default shape
	Primitive* newObj = nullptr;

	switch (type)
	{
	case Primitive::Type::BOX:
		newObj = new Primitive(shapes->GetCubeVertices());
		newObj->boundingVolume.Create(BoundingVolume::Type::BOX, translation, 0, scale);
		newObj->collisionVolume.Create(CollisionVolume::Type::BOX, translation, 0, scale, rotation, Vector3());
		break;
	case Primitive::Type::PLANE:
		newObj = new Primitive(shapes->GetPlaneVertices());
		//newObj->boundingVolume->Create(BoundingVolume::Type::BOX, translation, 0, scale);
		newObj->collisionVolume.Create(CollisionVolume::Type::PLANE, translation, 0, scale, rotation, Vector3(0,1,0));
		break;
	case Primitive::Type::SPHERE:
		newObj = new Primitive(shapes->GetSphereVertices(radius, &shapes->colourPresets->red, 12, 8));
		newObj->boundingVolume.Create(BoundingVolume::Type::SPHERE, translation, radius, scale);
		newObj->collisionVolume.Create(CollisionVolume::Type::SPHERE, translation, radius, scale, rotation, Vector3());
		break;
	case Primitive::Type::COMPLEX:
		break;
	default:
		newObj = new Primitive(shapes->GetCubeVertices());
		break;
	}
	newObj->type = type;

	newObj->scale = scale;
	newObj->translation = translation;
	newObj->rotation = rotation;

	UpdateTransforms(newObj);

	newObj->SetTweenOrigin();

	primitives.push_back(newObj);
	//delete newObj;
}

void Objects::CreateSphere(float radius, Vector3 translation)
{
	Create(Primitive::Type::SPHERE, Vector3(1, 1, 1), translation, Vector3(), radius);
}

void Objects::UpdateTransforms(Primitive* prim)
{
	prim->transform.Identity();

	//Update the transform matrix with the new transform vectors.
	mathe->Translate(prim->transform, prim->translation.x, prim->translation.y, prim->translation.z);
	mathe->Rotate(prim->transform, prim->rotation.x, prim->rotation.y, prim->rotation.z);
	prim->collisionVolume.axisMat = prim->transform;
	mathe->Scale(prim->transform, prim->scale.x, prim->scale.y, prim->scale.z);

	//Define bounding volume based on the primitive's vertices.
	//Gets the min and max values.
	//prim->boundingVolume->SetVertices(prim->vertices);

	prim->boundingVolume.Update(prim->translation, 0, prim->scale);
	prim->collisionVolume.Update(prim->translation, prim->radius, prim->scale / 2, prim->rotation);
	
	prim->updateTransforms = false;
}

void Objects::Draw()
{
	if (primitives.size() <= 0) return;

	//For each object.
	for (int i = 0; i < primitives.size(); i++)
	{	
		glBegin(GetDrawType(primitives[i]->type));
		
		//Only locally transform the vertices. This makes it so that each draw call
		//the vertices do not have to be cleared and redefined if their transforms have
		//changed. Performance boost.
		std::vector<Vertex> verts = primitives[i]->vertices;
		for (int v = 0; v < verts.size(); v++)
		{
			//Transform the vertices.
			mathe->Transform(verts[v].position, primitives[i]->transform);

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

void Objects::Update(int deltaTime)
{
	//Convert DT to seconds
	double trueDeltaTime = (double)deltaTime / 1000.0;

	//Moves objects to illustrate collision detection.
	primitives[1]->Tween(trueDeltaTime, 1, Vector3(-0.5, 0, -1), 6);
	primitives[2]->Tween(trueDeltaTime, 1, Vector3(-0.2, 1, 0), 10);
	primitives[3]->Tween(trueDeltaTime, 1, Vector3(-1, 0, 0), 8);
	primitives[4]->Tween(trueDeltaTime, 0.5, Vector3(0, -1, 0), 5);

	//Update transforms if they have changed.
	for (int i = 0; i < primitives.size(); i++)
	{
		if (primitives[i]->updateTransforms) UpdateTransforms(primitives[i]);
	}

	//Custom define which objects to detect collisions between.
	//Will convert to a spatial data structure in the future.
	//Console output is every quarter of a second for readability.

	//Plane
	collisions->DetectFine(primitives[0], primitives[1], timeSinceCollisionDebug > 0.25);
	collisions->DetectFine(primitives[0], primitives[2], timeSinceCollisionDebug > 0.25);
	collisions->DetectFine(primitives[0], primitives[3], timeSinceCollisionDebug > 0.25);
	collisions->DetectFine(primitives[0], primitives[4], timeSinceCollisionDebug > 0.25);
	//Box1																			 
	collisions->DetectFine(primitives[1], primitives[2], timeSinceCollisionDebug > 0.25);
	collisions->DetectFine(primitives[1], primitives[3], timeSinceCollisionDebug > 0.25);
	collisions->DetectFine(primitives[1], primitives[4], timeSinceCollisionDebug > 0.25);
	//Box2																			 
	collisions->DetectFine(primitives[2], primitives[3], timeSinceCollisionDebug > 0.25);
	collisions->DetectFine(primitives[2], primitives[4], timeSinceCollisionDebug > 0.25);
	//Box3																			 
	collisions->DetectFine(primitives[3], primitives[4], timeSinceCollisionDebug > 0.25);
		
	if (timeSinceCollisionDebug > 0.25)
	{
		timeSinceCollisionDebug = 0;
		std::cout << std::endl;
	}
	timeSinceCollisionDebug += trueDeltaTime;
}

GLenum Objects::GetDrawType(Primitive::Type objectType)
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
