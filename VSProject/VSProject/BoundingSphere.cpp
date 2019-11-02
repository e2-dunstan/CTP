#include "BoundingSphere.h"

void BoundingSphere::Create(Vector3 _centre, float _radius)
{
	centre = _centre;
	radius = _radius;

	drawMode = GL_QUADS;
}

void BoundingSphere::Update(Vector3 _centre, float _radius)
{
	vertices.clear();
}

void BoundingSphere::SetVertices(std::vector<Vertex> verts)
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

	centre = minMaxCentre;
	/*radius = */
}
