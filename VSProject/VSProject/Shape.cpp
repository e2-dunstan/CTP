#include "Shape.h"


void Shape::Draw(std::vector<Vertex> _vertices)
{
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < _vertices.size(); i++)
	{
		glVertex3f(_vertices[i].position.x, _vertices[i].position.y, _vertices[i].position.z);
	}

	glEnd();
}
