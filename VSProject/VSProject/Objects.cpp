#include "Objects.h"


void Objects::Create(Object::Type type, Vector3 scale, Vector3 translation, Vector3 rotation)
{
	//default shape
	Object newObj = Object(shapes->GetCubeVertices());

	switch (type)
	{
	case Object::Type::CUBE:
		newObj = Object(shapes->GetCubeVertices());
		break;
	default:
		break;
	}

	newObj.scale = scale;
	newObj.translation = translation;
	newObj.rotation = rotation;

	//Custom transformations
	/*newObj.transform = mathe->Translate(newObj.transform, 0, 0, -2);
	newObj.transform = mathe->Scale(newObj.transform, 0.5, 0.5, 0.5);

	for (unsigned v = 0; v < _vertices.size(); v++)
	{
		newObj.vertices[v].position = mathe->Transform(newObj.vertices[v].position, newObj.transform);
		newObj.vertices[v].position.DebugOutput();
	}*/

	objects.push_back(newObj);
}

void Objects::Animate()
{	
}

void Objects::Draw()
{
	if (objects.size() <= 0) return;

	//std::cout << "drawing" << std::endl;

	for (int i = 0; i < objects.size(); i++)
	{		
		glTranslated(objects[i].translation.x, objects[i].translation.y, objects[i].translation.z);
		glRotated(100, objects[i].rotation.x, objects[i].rotation.y, objects[i].rotation.z);
		glScaled(objects[i].scale.x, objects[i].scale.y, objects[i].scale.z);	
		
		glMaterialf(GL_FRONT, GL_SHININESS, 0.2);

		glBegin(GetDrawType(objects[i].type));

		for (int v = 0; v < objects[i].vertices.size(); v++)
		{
			glNormal3f(objects[i].vertices[v].normal.x, objects[i].vertices[v].normal.y, objects[i].vertices[v].normal.z);

			GLfloat colour[] = { objects[i].vertices[v].colour.r, objects[i].vertices[v].colour.g, objects[i].vertices[v].colour.b,  objects[i].vertices[v].colour.a};
			glMaterialfv(GL_FRONT, GL_DIFFUSE, colour);

			//glColor3f(objects[i].vertices[v].colour.r, objects[i].vertices[v].colour.g, objects[i].vertices[v].colour.b);
			glVertex3f(objects[i].vertices[v].position.x, objects[i].vertices[v].position.y, objects[i].vertices[v].position.z);
		}

		glEnd();
	}
}

GLenum Objects::GetDrawType(Object::Type objectType)
{
	switch (objectType)
	{
	case Object::Type::CUBE:
		return GL_QUADS;
	default:
		return GL_QUADS;
	}
}
