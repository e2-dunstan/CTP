#include "BoundingVolume.h"


//BoundingVolume::BoundingVolume(Vector3 _centre, float _radius, Vector3 _size)
//{
//	if (_radius != 0 && _size.x == 0 && _size.y == 0 && _size.z == 0)
//	{
//		sphere.active = true;
//		sphere.centre = _centre;
//		sphere.radius = _radius; 
//
//		drawMode = GL_SPHERE_MAP;
//
//		//Change to get sphere vertices
//		vertices = shapes->GetCubeVertices();
//	}
//	else if (_radius == 0 && _size.x != 0 && _size.y != 0 && _size.z != 0)
//	{
//		cube.active = true;
//		cube.centre = _centre;
//		cube.size = _size;
//
//		drawMode = GL_QUADS;
//		vertices = shapes->GetCubeVertices();
//	}
//	else
//	{
//		std::cout << "BOUNDING VOLUME CREATION ERROR: All bounding volumes must have an origin and a radius OR size greater than zero." << std::endl;
//	}
//}

void BoundingVolume::Create(Vector3 _centre, float _radius, Vector3 _size)
{
	if (_radius != 0 && _size.x == 0 && _size.y == 0 && _size.z == 0)
	{
		sphere.active = true;
		sphere.centre = _centre;
		sphere.radius = _radius;

		drawMode = GL_SPHERE_MAP;

		//Change to get sphere vertices
		vertices = shapes->GetCubeVertices();
	}
	else if (_radius == 0 && _size.x != 0 && _size.y != 0 && _size.z != 0)
	{
		cube.active = true;
		cube.centre = _centre;
		cube.size = _size;

		drawMode = GL_QUADS;
		vertices = shapes->GetCubeVertices();
	}
	else
	{
		std::cout << "BOUNDING VOLUME CREATION ERROR: All bounding volumes must have an origin and a radius OR size greater than zero." << std::endl;
	}
}

void BoundingVolume::Update(Vector3 _centre, float _radius, Vector3 _size)
{
	if (sphere.active)
	{
		if (_centre != sphere.centre || _radius != sphere.radius)
		{
			vertices.clear();
			//implement
		}
	}
	else if (cube.active)
	{
		if (_centre != cube.centre || _size != cube.size)
		{
			vertices.clear();
			vertices = shapes->GetCubeVertices();

			cube.centre = _centre;
			cube.size = _size;
			
			Matrix transform = Matrix(4, 4);

			mathe->Translate(transform, cube.centre.x, cube.centre.y, cube.centre.z);
			mathe->Scale(transform, cube.size.x, cube.size.y, cube.size.z);

			for (int v = 0; v < vertices.size(); v++)
			{
				mathe->Transform(vertices[v].position, transform);
				//transform normals
			}
		}
	}
}

void BoundingVolume::Draw()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPushMatrix();

	//glTranslated(translation.x, translation.y, translation.z);
	//glScaled(scale.x, scale.y, scale.z);
	//Transforms

	glBegin(drawMode);

	glColor3f(0, 0, 0);

	for (int v = 0; v < vertices.size(); v++)
	{
		glVertex3f(vertices[v].position.x, vertices[v].position.y, vertices[v].position.z);
	}
	//for (int v = 0; v < vertices->size(); v++)
	//{
	//	glNormal3f((*vertices)[v].normal.x, (*vertices)[v].normal.y, (*vertices)[v].normal.z);
	//	glColor3f(0, 0, 0);
	//	glVertex3f((*vertices)[v].position.x, (*vertices)[v].position.y, (*vertices)[v].position.z);
	//}

	glEnd();
	//glPopMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
