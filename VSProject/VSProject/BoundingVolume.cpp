#include "BoundingVolume.h"


BoundingVolume::BoundingVolume(const std::vector<Vertex>& _vertices, Matrix& _transform)
{
	Generate(_vertices, _transform);
}

void BoundingVolume::Generate(const std::vector<Vertex>& _vertices, Matrix& _transform)
{
	min = Vector3(10000, 10000, 10000);
	max = Vector3(-10000, -10000, -10000);

	Vector3 vert = Vector3();
	for (unsigned v = 0; v < _vertices.size(); v++)
	{
		vert = _vertices[v].position;
		Mathe::Transform(vert, _transform);
			
		if (vert.x < min.x) min.x = vert.x;
		if (vert.x > max.x) max.x = vert.x;
		if (vert.y < min.y) min.y = vert.y;
		if (vert.y > max.y) max.y = vert.y;
		if (vert.z < min.z) min.z = vert.z;
		if (vert.z > max.z) max.z = vert.z;
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

	glVertex3f((GLfloat)max.x, (GLfloat)max.y, (GLfloat)min.z);
	glVertex3f((GLfloat)min.x, (GLfloat)max.y, (GLfloat)min.z);
	glVertex3f((GLfloat)min.x, (GLfloat)max.y, (GLfloat)max.z);
	glVertex3f((GLfloat)max.x, (GLfloat)max.y, (GLfloat)max.z);

	glVertex3f((GLfloat)max.x, (GLfloat)min.y, (GLfloat)max.z);
	glVertex3f((GLfloat)min.x, (GLfloat)min.y, (GLfloat)max.z);
	glVertex3f((GLfloat)min.x, (GLfloat)min.y, (GLfloat)min.z);
	glVertex3f((GLfloat)max.x, (GLfloat)min.y, (GLfloat)min.z);

	glVertex3f((GLfloat)max.x, (GLfloat)max.y, (GLfloat)max.z);
	glVertex3f((GLfloat)min.x, (GLfloat)max.y, (GLfloat)max.z);
	glVertex3f((GLfloat)min.x, (GLfloat)min.y, (GLfloat)max.z);
	glVertex3f((GLfloat)max.x, (GLfloat)min.y, (GLfloat)max.z);

	glVertex3f((GLfloat)max.x, (GLfloat)min.y, (GLfloat)min.z);
	glVertex3f((GLfloat)min.x, (GLfloat)min.y, (GLfloat)min.z);
	glVertex3f((GLfloat)min.x, (GLfloat)max.y, (GLfloat)min.z);
	glVertex3f((GLfloat)max.x, (GLfloat)max.y, (GLfloat)min.z);
							
	glVertex3f((GLfloat)min.x, (GLfloat)max.y, (GLfloat)max.z);
	glVertex3f((GLfloat)min.x, (GLfloat)max.y, (GLfloat)min.z);
	glVertex3f((GLfloat)min.x, (GLfloat)min.y, (GLfloat)min.z);
	glVertex3f((GLfloat)min.x, (GLfloat)min.y, (GLfloat)max.z);
						
	glVertex3f((GLfloat)max.x, (GLfloat)max.y, (GLfloat)min.z);
	glVertex3f((GLfloat)max.x, (GLfloat)max.y, (GLfloat)max.z);
	glVertex3f((GLfloat)max.x, (GLfloat)min.y, (GLfloat)max.z);
	glVertex3f((GLfloat)max.x, (GLfloat)min.y, (GLfloat)min.z);

	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
