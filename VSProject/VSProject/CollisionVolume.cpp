#include "CollisionVolume.h"
#include "Utilities.h"


//void CollisionVolume::Create(const Vector3& _centre, float _radius, 
//	const Vector3& _size, const Quaternion& _orientation,
//	const Vector3& _normal, float _length)
//{
//	normal = _normal;
//	length = _length;
//	Update(_centre, _radius, _size, _orientation);
//}
//void CollisionVolume::Update(const Vector3& _centre, float _radius, 
//	const Vector3& _size, const Quaternion& _orientation)
//{
//	centre = _centre;
//	if (_radius != 0 && radius == 0) radius = _radius;
//	halfSize = _size;// *2.0;
//	orientation = _orientation;
//
//	switch (type)
//	{
//	case PrimitiveType::BOX:
//		Box();
//		break;
//	case PrimitiveType::SPHERE:
//	case PrimitiveType::PLANE:
//	case PrimitiveType::CAPSULE:
//	case PrimitiveType::CYLINDER:
//	case PrimitiveType::COMPLEX:
//	default:
//		break;
//	}
//}

//void CollisionVolume::Draw()
//{
//	if (type != PrimitiveType::BOX) return;
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

//void CollisionVolume::Box()
//{
//	vertices[0] = Vector3(-halfSize.x, -halfSize.y, -halfSize.z);
//	vertices[1] = Vector3(-halfSize.x, -halfSize.y, halfSize.z);
//	vertices[2] = Vector3(-halfSize.x, halfSize.y, -halfSize.z);
//	vertices[3] = Vector3(-halfSize.x, halfSize.y, halfSize.z);
//	vertices[4] = Vector3(halfSize.x, -halfSize.y, -halfSize.z);
//	vertices[5] = Vector3(halfSize.x, -halfSize.y, halfSize.z);
//	vertices[6] = Vector3(halfSize.x, halfSize.y, -halfSize.z);
//	vertices[7] = Vector3(halfSize.x, halfSize.y, halfSize.z);
//	normals[0] = Vector3(0, 1, 0);
//	normals[1] = Vector3(0, -1, 0);
//	normals[2] = Vector3(1, 0, 0);
//	normals[3] = Vector3(-1, 0, 0);
//	normals[4] = Vector3(0, 0, 1);
//	normals[5] = Vector3(0, 0, -1);
//
//	//Matrix transform = Matrix(4, 4);
//	//Mathe::Translate(transform, centre.x, centre.y, centre.z);
//	//Mathe::Rotate(transform, orientation);
//	//mathe->Scale(transform, halfSize.x, halfSize.y, halfSize.z);
//
//	for (int v = 0; v < 8; v++)
//	{
//		Mathe::Transform(vertices[v], axisMat);
//		if (v < 6)
//		{
//			Mathe::Transform(normals[v], axisMat);
//			normals[v] -= centre;
//			normals[v] = normals[v].Normalise();
//		}
//	}
//}


void BoxCV::Update(const Vector3& _centre, const Vector3& _size)
{
	centre = _centre;
	halfSize = _size;

	vertices[0] = Vector3(-halfSize.x, -halfSize.y, -halfSize.z);
	vertices[1] = Vector3(-halfSize.x, -halfSize.y, halfSize.z);
	vertices[2] = Vector3(-halfSize.x, halfSize.y, -halfSize.z);
	vertices[3] = Vector3(-halfSize.x, halfSize.y, halfSize.z);
	vertices[4] = Vector3(halfSize.x, -halfSize.y, -halfSize.z);
	vertices[5] = Vector3(halfSize.x, -halfSize.y, halfSize.z);
	vertices[6] = Vector3(halfSize.x, halfSize.y, -halfSize.z);
	vertices[7] = Vector3(halfSize.x, halfSize.y, halfSize.z);
	normals[0] = Vector3(0, 1, 0);
	normals[1] = Vector3(0, -1, 0);
	normals[2] = Vector3(1, 0, 0);
	normals[3] = Vector3(-1, 0, 0);
	normals[4] = Vector3(0, 0, 1);
	normals[5] = Vector3(0, 0, -1);

	for (int v = 0; v < 8; v++)
	{
		Mathe::Transform(vertices[v], axisMat);
		if (v < 6)
		{
			Mathe::Transform(normals[v], axisMat);
			normals[v] -= centre;
			normals[v] = normals[v].Normalise();
		}
	}
}

void SphereCV::Update(const Vector3& _centre)
{
	centre = _centre;
}

void PlaneCV::Update(const Vector3& _centre, const Vector3& _normal)
{
	centre = _centre;
	normal = _normal;
}
