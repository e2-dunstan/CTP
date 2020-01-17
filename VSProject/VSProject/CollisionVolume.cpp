#include "CollisionVolume.h"
#include "Utilities.h"


void CollisionVolume::Create(Type _type, const Vector3& _centre, float _radius, 
	const Vector3& _size, const Quaternion& _orientation,
	const Vector3& _normal, float _length)
{
	type = _type;
	normal = _normal;
	length = _length;
	Update(_centre, _radius, _size, _orientation);
}


void CollisionVolume::Update(const Vector3& _centre, float _radius, 
	const Vector3& _size, const Quaternion& _orientation)
{
	centre = _centre;
	if (_radius != 0 && radius == 0) radius = _radius;
	halfSize = _size * 2;
	orientation = _orientation;

	switch (type)
	{
	case Type::BOX:
		Box();
		break;
	case Type::SPHERE:
	case Type::PLANE:
	case Type::CAPSULE:
	case Type::CYLINDER:
	case Type::COMPLEX:
	default:
		break;
	}
}


//void CollisionVolume::Draw()
//{
//	if (type != Type::BOX) return;
//
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//
//	glBegin(GL_QUADS);
//	glColor3f(1, 0, 0);
//	for (int v = 0; v < vertices.size(); v++)
//	{
//		glVertex3f(vertices[v].x, vertices[v].y, vertices[v].z);
//	}
//	glEnd();
//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//}


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
	//Mathe::Rotate(transform, rotation.x, rotation.y, rotation.z);
	Mathe::Rotate(transform, orientation);
	//mathe->Scale(transform, halfSize.x, halfSize.y, halfSize.z);

	for (int v = 0; v < 8; v++)
	{
		Mathe::Transform(vertices[v], transform);
	}
}
