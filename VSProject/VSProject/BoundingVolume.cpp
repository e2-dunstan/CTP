#include "BoundingVolume.h"


void BoundingVolume::Create(Type _type, Vector3 _centre, float _radius, Vector3 _size)
{
	type = _type;
	centre = _centre;
	radius = _radius;
	size = _size;

	drawMode = GL_QUADS;
}

void BoundingVolume::Update(Vector3 _centre, float _radius, Vector3 _size)
{
	switch (type)
	{
	case Type::BOX:
		UpdateBox(_centre, _size);
		break;
	case Type::SPHERE:
		UpdateSphere(_centre, _radius);
		break;
	}
}

void BoundingVolume::SetVertices(std::vector<Vertex> verts)
{
	switch (type)
	{
	case Type::BOX:
		SetVerticesBox(verts);
		break;
	case Type::SPHERE:
		SetVerticesSphere(verts);
		break;
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

	glEnd();
	//glPopMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void BoundingVolume::UpdateBox(Vector3 _centre, Vector3 _size)
{
	vertices = shapes->baseBox;

	Matrix transform = Matrix(4, 4);

	//No rotation for bounding volumes.
	mathe->Translate(transform, centre.x, centre.y, centre.z);
	mathe->Scale(transform, size.x, size.y, size.z);

	for (int v = 0; v < vertices.size(); v++)
	{
		mathe->Transform(vertices[v].position, transform);
		//transform normals
	}
}

void BoundingVolume::UpdateSphere(Vector3 _centre, float _radius)
{
}

void BoundingVolume::SetVerticesBox(std::vector<Vertex> verts)
{
	Vector3 min = Vector3(10000, 10000, 10000);
	Vector3 max = Vector3();

	//Get the bounding volume based on the min and max vertices.
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

	centre = minMaxCentre;
	size = (max - min) / 2.0f;
}

void BoundingVolume::SetVerticesSphere(std::vector<Vertex> verts)
{
}
