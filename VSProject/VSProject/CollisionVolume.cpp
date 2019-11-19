#include "CollisionVolume.h"
#include "Utilities.h"


void CollisionVolume::Create(Type _type, Vector3 _centre, float _radius, Vector3 _size, Vector3 _rotation, Vector3 _normal)
{
	type = _type;
	normal = _normal;
	Update(_centre, _radius, _size, _rotation);
}


void CollisionVolume::Update(Vector3 _centre, float _radius, Vector3 _size, Vector3 _rotation)
{
	centre = _centre;
	if (_radius != 0 && radius == 0) radius = _radius;
	halfSize = _size * 2;
	rotation = _rotation;

	switch (type)
	{
	case Type::BOX:
		Box();
		break;
	case Type::SPHERE:
		//Sphere();
		break;
	case Type::PLANE:
		//Plane();
		break;
	case Type::COMPLEX:
		//Complex();
		break;
	}
}


void CollisionVolume::Draw()
{
	if (type != Type::BOX) return;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_QUADS);
	glColor3f(1, 0, 0);
	for (int v = 0; v < vertices.size(); v++)
	{
		glVertex3f(vertices[v].x, vertices[v].y, vertices[v].z);
	}
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


void CollisionVolume::Box()
{
	if (vertices.size() <= 0)
	{
		vertices.push_back(Vector3(-halfSize.x, -halfSize.y, -halfSize.z));
		vertices.push_back(Vector3(-halfSize.x, -halfSize.y, halfSize.z));
		vertices.push_back(Vector3(-halfSize.x, halfSize.y, -halfSize.z));
		vertices.push_back(Vector3(-halfSize.x, halfSize.y, halfSize.z));
		vertices.push_back(Vector3(halfSize.x, -halfSize.y, -halfSize.z));
		vertices.push_back(Vector3(halfSize.x, -halfSize.y, halfSize.z));
		vertices.push_back(Vector3(halfSize.x, halfSize.y, -halfSize.z));
		vertices.push_back(Vector3(halfSize.x, halfSize.y, halfSize.z));
	}
	else
	{
		vertices[0] = Vector3(-halfSize.x, -halfSize.y, -halfSize.z);
		vertices[1] = Vector3(-halfSize.x, -halfSize.y, halfSize.z);
		vertices[2] = Vector3(-halfSize.x, halfSize.y, -halfSize.z);
		vertices[3] = Vector3(-halfSize.x, halfSize.y, halfSize.z);
		vertices[4] = Vector3(halfSize.x, -halfSize.y, -halfSize.z);
		vertices[5] = Vector3(halfSize.x, -halfSize.y, halfSize.z);
		vertices[6] = Vector3(halfSize.x, halfSize.y, -halfSize.z);
		vertices[7] = Vector3(halfSize.x, halfSize.y, halfSize.z);
	}

	Matrix transform = Matrix(4, 4);
	Mathe::Translate(transform, centre.x, centre.y, centre.z);
	Mathe::Rotate(transform, rotation.x, rotation.y, rotation.z);
	//mathe->Scale(transform, halfSize.x, halfSize.y, halfSize.z);

	for (int v = 0; v < 8; v++)
	{
		Mathe::Transform(vertices[v], transform);
	}
}
