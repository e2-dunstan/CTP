#include "BoundingBox.h"

using namespace Shapes;

void BoundingBox::Create(Vector3 _centre, Vector3 _size)
{
	centre = _centre;
	size = _size;

	drawMode = GL_QUADS;

	std::cout << "Bounding box override function create" << std::endl;
}

void BoundingBox::Update(Vector3 _centre, Vector3 _size, Vector3 _rotation)
{
	vertices.clear();
	vertices = ShapeVertices::GetCubeVertices();

	//cube.centre = _centre;
	//cube.size = _size;

	//if (_rotation != Vector3())
	//{
	//	cube.size *= 1.5f;
	//}

	Matrix transform = Matrix(4, 4);

	Mathe::Translate(transform, centre.x, centre.y, centre.z);
	Mathe::Scale(transform, size.x, size.y, size.z);

	for (int v = 0; v < vertices.size(); v++)
	{
		Mathe::Transform(vertices[v].position, transform);
		//transform normals
	}
	std::cout << "Bounding box override function update" << std::endl;
}

void BoundingBox::SetVertices(std::vector<Vertex> verts)
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
	size = (max - min) / 2.0f;

	std::cout << "Bounding box override function set verts" << std::endl;
}
