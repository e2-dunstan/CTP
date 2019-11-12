#include "Objects.h"


void Objects::Create(Primitive::Type type, Vector3 scale, Vector3 translation, Vector3 rotation, float radius)
{
	//default shape
	Primitive* newObj = nullptr;

	switch (type)
	{
	case Primitive::Type::BOX:
		newObj = new Primitive(shapes->GetCubeVertices());
		newObj->boundingVolume->Create(BoundingVolume::Type::BOX, translation, 0, scale);
		newObj->collisionVolume->Create(CollisionVolume::Type::BOX, translation, 0, scale, rotation, Vector3());
		break;
	case Primitive::Type::PLANE:
		newObj = new Primitive(shapes->GetPlaneVertices());
		//newObj->boundingVolume->Create(BoundingVolume::Type::BOX, translation, 0, scale);
		newObj->collisionVolume->Create(CollisionVolume::Type::PLANE, translation, 0, scale, rotation, Vector3(0,1,0));
		break;
	case Primitive::Type::SPHERE:
		newObj = new Primitive(shapes->GetSphereVertices(radius, &shapes->colourPresets->red, 12, 12));
		newObj->boundingVolume->Create(BoundingVolume::Type::SPHERE, translation, radius, scale);
		newObj->collisionVolume->Create(CollisionVolume::Type::SPHERE, translation, radius, scale, rotation, Vector3());
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

	primitives.push_back(newObj);
}

void Objects::CreateSphere(float radius, Vector3 translation)
{
	Create(Primitive::Type::SPHERE, Vector3(1, 1, 1), translation, Vector3(), radius);
}

void Objects::UpdateTransforms(Primitive* prim)
{
	if (prim->type == Primitive::Type::BOX) prim->vertices = shapes->GetCubeVertices();
	prim->transform.Identity();

	mathe->Translate(prim->transform, prim->translation.x, prim->translation.y, prim->translation.z);
	mathe->Rotate(prim->transform, prim->rotation.x, prim->rotation.y, prim->rotation.z);
	prim->collisionVolume->axisMat = prim->transform;
	mathe->Scale(prim->transform, prim->scale.x, prim->scale.y, prim->scale.z);

	for (int v = 0; v < prim->vertices.size(); v++)
	{
		mathe->Transform(prim->vertices[v].position, prim->transform);
		//transform normals
	}

	//SPHERE STUFF HERE IF NOT CUBE
	prim->boundingVolume->SetVertices(prim->vertices);
	prim->boundingVolume->Update(prim->translation, 0, prim->scale);
	prim->collisionVolume->Update(prim->translation, 0, prim->scale / 2, prim->rotation);
	
	prim->rigidbody->updateTransforms = false;
}

void Objects::Draw()
{
	if (primitives.size() <= 0) return;
	/*if (!octTreeCreated)
	{
		octTree = new OctTree(rbs);
		octTree->UpdateTree();
		octTreeCreated = true;
	}*/

	for (int i = 0; i < primitives.size(); i++)
	{	
		glBegin(GetDrawType(primitives[i]->type));

		for (int v = 0; v < primitives[i]->vertices.size(); v++)
		{
			glNormal3f(primitives[i]->vertices[v].normal.x, primitives[i]->vertices[v].normal.y, primitives[i]->vertices[v].normal.z);

			//GLfloat colour[] = { objects[i].vertices[v].colour.r, objects[i].vertices[v].colour.g, objects[i].vertices[v].colour.b,  objects[i].vertices[v].colour.a};
			//glMaterialfv(GL_FRONT, GL_DIFFUSE, colour);

			glColor3f(primitives[i]->vertices[v].colour.r, primitives[i]->vertices[v].colour.g, primitives[i]->vertices[v].colour.b);
			glVertex3f(primitives[i]->vertices[v].position.x, primitives[i]->vertices[v].position.y, primitives[i]->vertices[v].position.z);
		}

		glEnd();
		
		if (drawBoundingVolumes) primitives[i]->boundingVolume->Draw();
		if (drawCollisionVolumes) primitives[i]->collisionVolume->Draw();
	}
}

void Objects::Update(int deltaTime)
{
	double trueDeltaTime = (double)deltaTime / 1000.0;

	//Move an object along the z axis
	//primitives[2]->translation.z -= trueDeltaTime;
	//primitives[2]->rigidbody->updateTransforms = true;

	//Update all transforms
	for (int i = 0; i < primitives.size(); i++)
	{
		if (primitives[i]->rigidbody->updateTransforms) UpdateTransforms(primitives[i]);
	}


	collisions->DetectFine(primitives[2], primitives[3]);

	//use oct tree here

	//for (int i = 0; i < rbs.size(); i++)
	//{
	//	if (rbs[i]->enableCollision)
	//		collisions->DetectCoarse(rbs[i]);
	//}
}

GLenum Objects::GetDrawType(Primitive::Type objectType)
{
	switch (objectType)
	{
	case Primitive::Type::SPHERE:
		return GL_TRIANGLES;
	case Primitive::Type::BOX:
	default:
		return GL_QUADS;
	}
}
