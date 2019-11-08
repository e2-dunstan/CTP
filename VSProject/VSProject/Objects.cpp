#include "Objects.h"


void Objects::Create(Primitive::Type type, Vector3 scale, Vector3 translation, Vector3 rotation)
{
	//default shape
	Primitive* newObj = nullptr;

	switch (type)
	{
	case Primitive::Type::BOX:
		newObj = new Primitive(shapes->GetCubeVertices());
		newObj->boundingVolume->Create(BoundingVolume::Type::BOX, translation, 0, scale);
		newObj->collisionVolume->Create(CollisionVolume::Type::BOX, translation, 0, scale / 2, rotation, Vector3());
		break;
	case Primitive::Type::PLANE:
		newObj = new Primitive(shapes->GetPlaneVertices());
		//newObj->boundingVolume->Create(BoundingVolume::Type::BOX, translation, 0, scale);
		newObj->collisionVolume->Create(CollisionVolume::Type::PLANE, translation, 0, scale / 2, rotation, Vector3(0,1,0));
		break;
	case Primitive::Type::SPHERE:
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

void Objects::UpdateTransforms(Primitive* prim)
{
	prim->transform.Identity();

	mathe->Translate(prim->transform, prim->translation.x, prim->translation.y, prim->translation.z);
	mathe->Rotate(prim->transform, prim->rotation.x, prim->rotation.y, prim->rotation.z);
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

void Objects::Animate()
{	
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
		if (primitives[i]->rigidbody->updateTransforms) UpdateTransforms(primitives[i]);

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
	}
}

void Objects::Update(int deltaTime)
{
	for (int i = 1; i < primitives.size(); i++)
	{
		collisions->DetectFine(primitives[0], primitives[i]);
	}
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
	case Primitive::Type::BOX:
		return GL_QUADS;
	default:
		return GL_QUADS;
	}
}
