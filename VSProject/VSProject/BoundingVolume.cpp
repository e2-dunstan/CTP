#include "BoundingVolume.h"


BoundingVolume::BoundingVolume(const std::vector<Vertex>& _vertices, Matrix& _transform)
{
	Generate(_vertices, _transform);
}

void BoundingVolume::Generate(const std::vector<Vertex>& _vertices, Matrix& _transform)
{
	min = Vector3(10000, 10000, 10000);
	max = Vector3(-10000, -10000, -10000);

	for (auto v : _vertices)
	{
		Mathe::Transform(v.position, _transform);
			
		if (v.position.x < min.x) min.x = v.position.x;
		if (v.position.x > max.x) max.x = v.position.x;
		if (v.position.y < min.y) min.y = v.position.y;
		if (v.position.y > max.y) max.y = v.position.y;
		if (v.position.z < min.z) min.z = v.position.z;
		if (v.position.z > max.z) max.z = v.position.z;
	}

	halfSize = Vector3((max.x - min.x) / 2,
					   (max.y - min.y) / 2,
					   (max.z - min.z) / 2);

	centre = Vector3(min.x + halfSize.x,
					 min.y + halfSize.y,
					 min.z + halfSize.z);

}

void BoundingVolume::Draw()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_QUADS);     

	glColor3f(1, 0, 0);

	glVertex3f(max.x, max.y, min.z);
	glVertex3f(min.x, max.y, min.z);
	glVertex3f(min.x, max.y, max.z);
	glVertex3f(max.x, max.y, max.z);

	glVertex3f(max.x, min.y, max.z);
	glVertex3f(min.x, min.y, max.z);
	glVertex3f(min.x, min.y, min.z);
	glVertex3f(max.x, min.y, min.z);

	glVertex3f(max.x, max.y, max.z);
	glVertex3f(min.x, max.y, max.z);
	glVertex3f(min.x, min.y, max.z);
	glVertex3f(max.x, min.y, max.z);

	glVertex3f(max.x, min.y, min.z);
	glVertex3f(min.x, min.y, min.z);
	glVertex3f(min.x, max.y, min.z);
	glVertex3f(max.x, max.y, min.z);

	glVertex3f(min.x, max.y, max.z);
	glVertex3f(min.x, max.y, min.z);
	glVertex3f(min.x, min.y, min.z);
	glVertex3f(min.x, min.y, max.z);

	glVertex3f(max.x, max.y, min.z);
	glVertex3f(max.x, max.y, max.z);
	glVertex3f(max.x, min.y, max.z);
	glVertex3f(max.x, min.y, min.z);

	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
