#include "Objects.h"


void Objects::CreateObject(std::vector<Vertex> _vertices)
{
	Object newObj = Object(_vertices);
	newObj.scale = Vector3(2, 2, 2);
	newObj.translation = Vector3(0, 0, -10);
	newObj.rotation = Vector3(1, 1, 1);
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
	//objects[0].transform = mathe->Rotate(0.0, 0.1, 0.0);
	//for (unsigned v = 0; v < objects[0].vertices.size(); v++)
	//{
	//	objects[0].vertices[v].position = mathe->Transform(objects[0].vertices[v].position, objects[0].transform);
	//}
}

void Objects::Draw()
{
	if (objects.size() <= 0) return;

	//Resets the origin every frame
	glLoadIdentity();

	for (int i = 0; i < objects.size(); i++)
	{
		glTranslated(objects[i].translation.x, objects[i].translation.y, objects[i].translation.z);
		glRotated(100, objects[i].rotation.x, objects[i].rotation.y, objects[i].rotation.z);
		glScaled(objects[i].scale.x, objects[i].scale.y, objects[i].scale.z);

		glBegin(GL_POLYGON);

		for (int v = 0; v < objects[i].vertices.size(); v++)
		{
			glColor3f(objects[i].vertices[v].colour.r, objects[i].vertices[v].colour.g, objects[i].vertices[v].colour.b);
			glVertex3f(objects[i].vertices[v].position.x, objects[i].vertices[v].position.y, objects[i].vertices[v].position.z);
		}

		glEnd();
	}
}
