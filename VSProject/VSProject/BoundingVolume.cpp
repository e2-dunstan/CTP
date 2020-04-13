#include "BoundingVolume.h"


BoundingVolume::BoundingVolume(const std::vector<Vertex>& _vertices, Matrix4& _transform)
{
	Generate(_vertices, _transform);
}

BoundingVolume::BoundingVolume(Matrix4& _transform)
{
	GenerateForBox(_transform);
}

void BoundingVolume::Generate(const std::vector<Vertex>& _vertices, Matrix4& _transform)
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
}

void BoundingVolume::GenerateForBox(Matrix4& _transform)
{
	min = Vector3(10000, 10000, 10000);
	max = Vector3(-10000, -10000, -10000);

	Vector3 vertices[8] =
	{
		Vector3(-1.0, 1.0, 1.0),
		Vector3(-1.0, 1.0, -1.0),
		Vector3(-1.0, -1.0, -1.0),
		Vector3(-1.0, -1.0, 1.0),
		Vector3(1.0, 1.0, 1.0),	
		Vector3(1.0, 1.0, -1.0),
		Vector3(1.0, -1.0, -1.0),
		Vector3(1.0, -1.0, 1.0),
	};
	for (uint16_t v = 0; v < 8; v++)
	{
		Mathe::Transform(vertices[v], _transform); 
		
		if (vertices[v].x < min.x) min.x = vertices[v].x;
		if (vertices[v].x > max.x) max.x = vertices[v].x;
		if (vertices[v].y < min.y) min.y = vertices[v].y;
		if (vertices[v].y > max.y) max.y = vertices[v].y;
		if (vertices[v].z < min.z) min.z = vertices[v].z;
		if (vertices[v].z > max.z) max.z = vertices[v].z;
	}
}

void BoundingVolume::UpdateMinMax(const Vector3& minimum, const Vector3& maximum)
{
	min = minimum;
	max = maximum;
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
