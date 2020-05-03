#include "CollisionVolume.h"
#include "Utilities.h"

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
