#include "Objects.h"


void Objects::Create(RigidBody::Type type, Vector3 scale, Vector3 translation, Vector3 rotation)
{
	//default shape
	RigidBody* newObj = new RigidBody(shapes->GetCubeVertices());

	switch (type)
	{
	case RigidBody::Type::CUBE:
		//newObj = RigidBody(shapes->GetCubeVertices());
		newObj->boundingVolume->Create(newObj->vertices, BoundingVolume::Type::CUBE);
		break;
	default:
		break;
	}

	newObj->scale = scale;
	newObj->translation = translation;
	newObj->rotation = rotation;

	UpdateTransforms(*newObj);

	rbs.push_back(newObj);
}

void Objects::UpdateTransforms(RigidBody& rb)
{
	rb.transform.Identity();

	mathe->Translate(rb.transform, rb.translation.x, rb.translation.y, rb.translation.z);
	mathe->Rotate(rb.transform, rb.rotation.x, rb.rotation.y, rb.rotation.z);
	mathe->Scale(rb.transform, rb.scale.x, rb.scale.y, rb.scale.z);

	for (int v = 0; v < rb.vertices.size(); v++)
	{
		mathe->Transform(rb.vertices[v].position, rb.transform);
		//transform normals
	}

	//SPHERE STUFF HERE IF NOT CUBE
	rb.boundingVolume->SetCentreAndSizeFromVertices(rb.vertices);
	rb.boundingVolume->Update(rb.translation, 0, rb.scale, rb.rotation);
	
	rb.updateTransforms = false;
}

void Objects::CreateBoundingVolume(RigidBody& rb)
{
	if (rb.type == RigidBody::Type::CUBE)
	{
		rb.boundingVolume->Create(rb.translation, 0, rb.scale);
	}
}

void Objects::Animate()
{	
}

void Objects::Draw()
{

	if (rbs.size() <= 0) return;
	/*if (!octTreeCreated)
	{
		octTree = new OctTree(rbs);
		octTree->UpdateTree();
		octTreeCreated = true;
	}*/

	//std::cout << "drawing" << std::endl;

	for (int i = 0; i < rbs.size(); i++)
	{	
		if (rbs[i]->updateTransforms) UpdateTransforms(*rbs[i]);
		/*PRE CUSTOM MATRIX MATH
		glPushMatrix();

		glTranslated(rigidBodies[i].translation.x, rigidBodies[i].translation.y, rigidBodies[i].translation.z);
		if (rigidBodies[i].rotation.x != 0 || rigidBodies[i].rotation.y != 0 || rigidBodies[i].rotation.z != 0)
			glRotated(100, rigidBodies[i].rotation.x, rigidBodies[i].rotation.y, rigidBodies[i].rotation.z);

		glScaled(rigidBodies[i].scale.x, rigidBodies[i].scale.y, rigidBodies[i].scale.z);	
		*/
		//glMaterialf(GL_FRONT, GL_SHININESS, 0.2);

		glBegin(GetDrawType(rbs[i]->type));

		for (int v = 0; v < rbs[i]->vertices.size(); v++)
		{
			glNormal3f(rbs[i]->vertices[v].normal.x, rbs[i]->vertices[v].normal.y, rbs[i]->vertices[v].normal.z);

			//GLfloat colour[] = { objects[i].vertices[v].colour.r, objects[i].vertices[v].colour.g, objects[i].vertices[v].colour.b,  objects[i].vertices[v].colour.a};
			//glMaterialfv(GL_FRONT, GL_DIFFUSE, colour);

			glColor3f(rbs[i]->vertices[v].colour.r, rbs[i]->vertices[v].colour.g, rbs[i]->vertices[v].colour.b);
			glVertex3f(rbs[i]->vertices[v].position.x, rbs[i]->vertices[v].position.y, rbs[i]->vertices[v].position.z);
		}

		glEnd();
		//glPopMatrix();
		
		if (drawBoundingVolumes) rbs[i]->boundingVolume->Draw();
	}
}

void Objects::Update()
{
	//use oct tree here

	for (int i = 0; i < rbs.size(); i++)
	{
		if (rbs[i]->enableCollision)
			collisions->DetectCoarse(rbs[i]);
	}
}

GLenum Objects::GetDrawType(RigidBody::Type objectType)
{
	switch (objectType)
	{
	case RigidBody::Type::CUBE:
		return GL_QUADS;
	default:
		return GL_QUADS;
	}
}
