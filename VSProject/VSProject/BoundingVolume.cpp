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

void BoundingVolume::Create(std::vector<Vertex> rbVerts, Type type)
{
	if (type == Type::CUBE)
	{
		cube.active = true;		
		drawMode = GL_QUADS;
		vertices = shapes->GetCubeVertices();
	}
	else if (type == Type::SPHERE)
	{
		sphere.active = true;
	}

	SetCentreAndSizeFromVertices(rbVerts);
}

void BoundingVolume::SetCentreAndSizeFromVertices(std::vector<Vertex> verts)
{
	Vector3 min = Vector3(10000, 10000, 10000);
	Vector3 max = Vector3();

	for (int v = 0; v < verts.size(); v++)
	{
		if (verts[v].position.x > max.x) max.x = verts[v].position.x;
		if (verts[v].position.y > max.y) max.y = verts[v].position.y;
		if (verts[v].position.z > max.z) max.z = verts[v].position.z;

		if (verts[v].position.x < min.x) min.x = verts[v].position.x;
		if (verts[v].position.y < min.y) min.y = verts[v].position.y;
		if (verts[v].position.z < min.z) min.z = verts[v].position.z;
	}

	Vector3 minMaxCentre = min + ((max - min) / 2.0f);

	if (cube.active)
	{
		cube.centre = minMaxCentre;
		cube.size = (max - min) / 2.0f;
	}
	else if (sphere.active)
	{

	}

}

void BoundingVolume::Update(Vector3 _centre, float _radius, Vector3 _size, Vector3 _rotation)
{
	if (sphere.active)
	{
		vertices.clear();
		//implement
	}
	else if (cube.active)
	{
		vertices.clear();
		vertices = shapes->GetCubeVertices();

		//cube.centre = _centre;
		//cube.size = _size;

		//if (_rotation != Vector3())
		//{
		//	cube.size *= 1.5f;
		//}
		cube.size.DebugOutput();
		cube.centre.DebugOutput();

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
