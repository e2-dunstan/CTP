#include "Shapes.h"

using namespace Shapes;

std::vector<Vertex> ShapeVertices::GetCubeVertices(const Colour& _colour)
{
	std::vector<Vertex> vertices;
	vertices.clear();

	//front
	Colour faceColour = GetColour(Colours::red, _colour);
	Vector3 normal = Vector3(0, 0, 1);
	vertices.push_back(Vertex(-1.0, 1.0, 1.0,	faceColour, normal));
	vertices.push_back(Vertex(-1.0, -1.0, 1.0,	faceColour, normal));
	vertices.push_back(Vertex(1.0, -1.0, 1.0,	faceColour, normal));
	vertices.push_back(Vertex(1.0, 1.0, 1.0,	faceColour, normal));
	
	//back
	faceColour = GetColour(Colours::green, _colour);
	normal = Vector3(0, 0, -1);
	vertices.push_back(Vertex(1.0, 1.0, -1.0,	faceColour, normal));
	vertices.push_back(Vertex(1.0, -1.0, -1.0,	faceColour, normal));
	vertices.push_back(Vertex(-1.0, -1.0, -1.0, faceColour, normal));
	vertices.push_back(Vertex(-1.0, 1.0, -1.0,	faceColour, normal));

	//right
	faceColour = GetColour(Colours::blue, _colour);
	normal = Vector3(1, 0, 0);
	vertices.push_back(Vertex(1.0, 1.0, 1.0,	faceColour, normal));
	vertices.push_back(Vertex(1.0, -1.0, 1.0,	faceColour, normal));
	vertices.push_back(Vertex(1.0, -1.0, -1.0,	faceColour, normal));
	vertices.push_back(Vertex(1.0, 1.0, -1.0,	faceColour, normal));

	//left
	faceColour = GetColour(Colours::yellow, _colour);
	normal = Vector3(-1, 0, 0);
	vertices.push_back(Vertex(-1.0, 1.0, -1.0,	faceColour, normal));
	vertices.push_back(Vertex(-1.0, -1.0, -1.0, faceColour, normal));
	vertices.push_back(Vertex(-1.0, -1.0, 1.0,	faceColour, normal));
	vertices.push_back(Vertex(-1.0, 1.0, 1.0,	faceColour, normal));

	//top
	faceColour = GetColour(Colours::cyan, _colour);
	normal = Vector3(0, 1, 0);
	vertices.push_back(Vertex(-1.0, 1.0, -1.0,	faceColour, normal));
	vertices.push_back(Vertex(-1.0, 1.0, 1.0,	faceColour, normal));
	vertices.push_back(Vertex(1.0, 1.0, 1.0,	faceColour, normal));
	vertices.push_back(Vertex(1.0, 1.0, -1.0,	faceColour, normal));

	//bottom
	faceColour = GetColour(Colours::magenta, _colour);
	normal = Vector3(0, -1, 0);
	vertices.push_back(Vertex(-1.0, -1.0, 1.0,	faceColour, normal));
	vertices.push_back(Vertex(-1.0, -1.0, -1.0, faceColour, normal));
	vertices.push_back(Vertex(1.0, -1.0, -1.0,	faceColour, normal));
	vertices.push_back(Vertex(1.0, -1.0, 1.0,	faceColour, normal));

	return vertices;
	vertices.clear();
}

std::vector<Vertex> ShapeVertices::GetPlaneVertices(const Colour& _colour)
{
	std::vector<Vertex> vertices;
	vertices.clear();

	vertices.push_back(Vertex(-1.0f, 0.0f, -1.0f, Colour(0.8f, 0.8f, 0.8f), Vector3(0, 1, 0)));
	vertices.push_back(Vertex(-1.0f, 0.0f, 1.0f, Colour(0.8f, 0.8f, 0.8f), Vector3(0, 1, 0)));
	vertices.push_back(Vertex(1.0f, 0.0f, 1.0f, Colour(0.8f, 0.8f, 0.8f), Vector3(0, 1, 0)));
	vertices.push_back(Vertex(1.0f, 0.0f, -1.0f, Colour(0.8f, 0.8f, 0.8f), Vector3(0, 1, 0)));

	return vertices;
	vertices.clear();
}

std::vector<Vertex> ShapeVertices::GetSphereVertices(float radius, const Colour& _colour, int sectorCount, int stackCount)
{
	Colour sphereColour = Colour(1, 1, 1);
	if (_colour != Colours::white) sphereColour = _colour;

	std::vector<Vertex> vertices;
	vertices.clear();

	std::vector<Vector3> positions;
	std::vector<Vector3> normals;
	std::vector<unsigned> indices;

	Vector3 pos;
	float xy;
	float lengthInverse = 1.0f / radius;
	Vector3 normal = Vector3(lengthInverse, lengthInverse, lengthInverse);

	float PI = 3.141592653589f;
	float sectorStep = 2 * PI / sectorCount;
	float stackStep = PI / stackCount;
	float sectorAngle;
	float stackAngle;

	for (unsigned i = 0; i <= (unsigned)stackCount; ++i)
	{
		stackAngle = PI / 2 - (i * stackStep);
		xy = radius * cosf(stackAngle);
		pos.z = (double)radius * (double)sin(stackAngle);

		for (unsigned j = 0; j <= (unsigned)sectorCount; ++j)
		{
			sectorAngle = j * sectorStep;

			pos.x = (double)xy * (double)cos(sectorAngle);
			pos.y = (double)xy * (double)sin(sectorAngle);

			normal = pos * lengthInverse;

			positions.push_back(pos);
			normals.push_back(normal);
			//vertices.push_back(Vertex(pos.x, pos.y, pos.z, Colour(cylinderColour), normal));
		}
	}

	unsigned index1;
	unsigned index2;
	for (unsigned i = 0; i < (unsigned)stackCount; i++)
	{
		index1 = i * (sectorCount + 1);
		index2 = index1 + sectorCount + 1;

		for (unsigned j = 0; j < (unsigned)sectorCount; ++j, ++index1, ++index2)
		{
			if (i != 0)
			{
				indices.push_back(index1);
				indices.push_back(index2);
				indices.push_back(index1 + 1);
			}
			if (i != (stackCount - 1))
			{
				indices.push_back(index1 + 1);
				indices.push_back(index2);
				indices.push_back(index2 + 1);
			}
		}
	}

	for (unsigned in = 0; in < indices.size(); in++)
	{
		unsigned index = indices[in];
		Vector3 indexPos = positions[index];
		Vector3 indexNor = normals[index];
		vertices.push_back(Vertex(indexPos.x, indexPos.y, indexPos.z, sphereColour, indexNor));
	}
	return vertices;
}

std::vector<Vertex> Shapes::ShapeVertices::GetCapsuleVertices(float radius, float straight, const Colour& _colour, int sectorCount, int stackCount)
{
	Colour capsuleColour = Colour(1, 1, 1);
	if (_colour != Colours::white) capsuleColour = _colour;

	if (stackCount % 2 == 0)
	{
		std::cout << "WARNING: stack count for capsules must be odd! Adding 1 to stack count." << std::endl;
		stackCount += 1;
	}

	std::vector<Vertex> vertices;
	vertices.clear();

	std::vector<Vector3> positions;
	std::vector<Vector3> normals;
	std::vector<unsigned> indices;

	Vector3 pos;
	float xy;
	float lengthInverse = 1.0f / radius;
	Vector3 normal = Vector3(lengthInverse, lengthInverse, lengthInverse);

	float PI = 3.141592653589f;
	float sectorStep = 2 * PI / sectorCount;
	float stackStep = PI / stackCount;
	float sectorAngle;
	float stackAngle;

	bool topSection = false;

	for (int i = 0; i <= stackCount; ++i)
	{
		topSection = i > stackCount / 2;
		float capsuleOffset = topSection ? -straight / 2 : straight / 2;

		stackAngle = PI / 2 - (i * stackStep);
		xy = radius * cosf(stackAngle);
		pos.z = (double)radius * (double)sin(stackAngle) + capsuleOffset;

		for (int j = 0; j <= sectorCount; ++j)
		{
			sectorAngle = j * sectorStep;

			pos.x = (double)xy * (double)cos(sectorAngle);
			pos.y = (double)xy * (double)sin(sectorAngle);

			normal = pos * lengthInverse;

			positions.push_back(pos);
			normals.push_back(normal);
			//vertices.push_back(Vertex(pos.x, pos.y, pos.z, Colour(cylinderColour), normal));
		}
	}

	unsigned index1;
	unsigned index2;
	for (unsigned i = 0; i < (unsigned)stackCount; i++)
	{
		index1 = i * (sectorCount + 1);
		index2 = index1 + sectorCount + 1;

		for (unsigned j = 0; j < (unsigned)sectorCount; ++j, ++index1, ++index2)
		{
			if (i != 0)
			{
				indices.push_back(index1);
				indices.push_back(index2);
				indices.push_back(index1 + 1);
			}
			if (i != (stackCount - 1))
			{
				indices.push_back(index1 + 1);
				indices.push_back(index2);
				indices.push_back(index2 + 1);
			}
		}
	}

	for (unsigned in = 0; in < indices.size(); in++)
	{
		unsigned index = indices[in];
		Vector3 indexPos = positions[index];
		Vector3 indexNor = normals[index];
		vertices.push_back(Vertex(indexPos.x, indexPos.y, indexPos.z, capsuleColour, indexNor));
	}
	return vertices;
}

std::vector<Vertex> Shapes::ShapeVertices::GetCylinderVertices(float radius, float straight, const Colour& _colour, int sectorCount)
{
	Colour cylinderColour = Colour(1, 1, 1);
	if (_colour != Colours::white) cylinderColour = _colour;

	std::vector<Vertex> vertices;
	vertices.clear();
	float PI = 3.141592653589f;
	float halfLength = straight / 2;

	//Top
	for (int i = 0; i < sectorCount; i++)
	{
		float theta = ((float)i / sectorCount) * 2 * PI;
		float nextTheta = ((float)(i + 1) / sectorCount) * 2 * PI;

		vertices.push_back(Vertex(0, halfLength, 0, cylinderColour, Vector3(0, 1, 0)));
		vertices.push_back(Vertex((double)radius * (double)sin(theta), halfLength, (double)radius * (double)cos(theta),
			cylinderColour, Vector3(cos(theta), 0.0, sin(theta))));
		if (i != sectorCount - 1) vertices.push_back(Vertex(0, halfLength, 0, cylinderColour, Vector3(0, 1, 0)));
		vertices.push_back(Vertex((double)radius * (double)sin(nextTheta), halfLength, (double)radius * (double)cos(nextTheta),
			cylinderColour, Vector3(cos(nextTheta), 0.0, sin(nextTheta))));
	}
	//Sides
	for (int i = 0; i < sectorCount; i++)
	{
		float theta = ((float)i / sectorCount) * 2 * PI;
		float nextTheta = ((float)(i + 1) / sectorCount) * 2 * PI;

		vertices.push_back(Vertex((double)radius * (double)sin(theta), -halfLength, (double)radius * (double)cos(theta),
			cylinderColour, Vector3(-cos(theta), 0.0, -sin(theta))));		
		vertices.push_back(Vertex((double)radius * (double)sin(theta), halfLength, (double)radius * (double)cos(theta),
			cylinderColour, Vector3(cos(theta), 0.0, sin(theta))));
		vertices.push_back(Vertex((double)radius * (double)sin(nextTheta), -halfLength, (double)radius * (double)cos(nextTheta),
				cylinderColour, Vector3(-cos(nextTheta), 0.0, -sin(nextTheta))));
		vertices.push_back(Vertex((double)radius * (double)sin(theta), halfLength, (double)radius * (double)cos(theta),
			cylinderColour, Vector3(cos(theta), 0.0, sin(theta))));
	}
	//Bottom
	for (int i = 0; i < sectorCount; i++)
	{
		float theta = ((float)i / sectorCount) * 2 * PI;
		float nextTheta = ((float)(i + 1) / sectorCount) * 2 * PI;

		vertices.push_back(Vertex((double)radius * (double)sin(theta), -halfLength, (double)radius * (double)cos(theta),
			cylinderColour, Vector3(cos(theta), 0.0, sin(theta))));
		if (i != 0) vertices.push_back(Vertex(0, -halfLength, 0, cylinderColour, Vector3(0, -1, 0)));
		vertices.push_back(Vertex((double)radius * (double)sin(nextTheta), -halfLength, (double)radius * (double)cos(nextTheta),
			cylinderColour, Vector3(cos(nextTheta), 0.0, sin(nextTheta))));
		vertices.push_back(Vertex(0, -halfLength, 0, cylinderColour, Vector3(0, -1, 0)));
	}
	return vertices;
}

Colour ShapeVertices::GetColour(const Colour& _rainbow, const Colour& _colour)
{
	if (_rainbow != Colours::white) return _rainbow;
	else return _colour;
}
