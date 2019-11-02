#include "Shapes.h"

std::vector<Vertex> ShapeVertices::GetCubeVertices(Colour* _colour)
{
	std::vector<Vertex> vertices;
	vertices.clear();

	//front
	Colour faceColour = GetColour(colourPresets->red, _colour);
	Vector3 normal = Vector3(0, 0, 1);
	vertices.push_back(Vertex(-1.0, 1.0, 1.0,	faceColour, normal));
	vertices.push_back(Vertex(-1.0, -1.0, 1.0,	faceColour, normal));
	vertices.push_back(Vertex(1.0, -1.0, 1.0,	faceColour, normal));
	vertices.push_back(Vertex(1.0, 1.0, 1.0,	faceColour, normal));
	
	//back
	faceColour = GetColour(colourPresets->green, _colour);
	normal = Vector3(0, 0, -1);
	vertices.push_back(Vertex(1.0, 1.0, -1.0,	faceColour, normal));
	vertices.push_back(Vertex(1.0, -1.0, -1.0,	faceColour, normal));
	vertices.push_back(Vertex(-1.0, -1.0, -1.0, faceColour, normal));
	vertices.push_back(Vertex(-1.0, 1.0, -1.0,	faceColour, normal));

	//right
	faceColour = GetColour(colourPresets->blue, _colour);
	normal = Vector3(1, 0, 0);
	vertices.push_back(Vertex(1.0, 1.0, 1.0,	faceColour, normal));
	vertices.push_back(Vertex(1.0, -1.0, 1.0,	faceColour, normal));
	vertices.push_back(Vertex(1.0, -1.0, -1.0,	faceColour, normal));
	vertices.push_back(Vertex(1.0, 1.0, -1.0,	faceColour, normal));

	//left
	faceColour = GetColour(colourPresets->yellow, _colour);
	normal = Vector3(-1, 0, 0);
	vertices.push_back(Vertex(-1.0, 1.0, -1.0,	faceColour, normal));
	vertices.push_back(Vertex(-1.0, -1.0, -1.0, faceColour, normal));
	vertices.push_back(Vertex(-1.0, -1.0, 1.0,	faceColour, normal));
	vertices.push_back(Vertex(-1.0, 1.0, 1.0,	faceColour, normal));

	//top
	faceColour = GetColour(colourPresets->cyan, _colour);
	normal = Vector3(0, 1, 0);
	vertices.push_back(Vertex(-1.0, 1.0, -1.0,	faceColour, normal));
	vertices.push_back(Vertex(-1.0, 1.0, 1.0,	faceColour, normal));
	vertices.push_back(Vertex(1.0, 1.0, 1.0,	faceColour, normal));
	vertices.push_back(Vertex(1.0, 1.0, -1.0,	faceColour, normal));

	//bottom
	faceColour = GetColour(colourPresets->magenta, _colour);
	normal = Vector3(0, -1, 0);
	vertices.push_back(Vertex(-1.0, -1.0, 1.0,	faceColour, normal));
	vertices.push_back(Vertex(-1.0, -1.0, -1.0, faceColour, normal));
	vertices.push_back(Vertex(1.0, -1.0, -1.0,	faceColour, normal));
	vertices.push_back(Vertex(1.0, -1.0, 1.0,	faceColour, normal));

	return vertices;
	vertices.clear();
}

std::vector<Vertex> ShapeVertices::GetPlaneVertices(Colour* _colour)
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

Colour ShapeVertices::GetColour(Colour _rainbow, Colour* _colour)
{
	if (_colour != NULL)
	{
		return *_colour;
	}
	else
	{
		return _rainbow;
	}
}
