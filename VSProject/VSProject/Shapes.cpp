#include "Shapes.h"

std::vector<Vertex> Shapes::GetCubeVertices(Colour* _colour)
{
	std::vector<Vertex> vertices;
	vertices.clear();

	//front
	Colour faceColour = GetColour(colourPresets->red, _colour);
	vertices.push_back(Vertex(-1.0, 1.0, 1.0,	faceColour));
	vertices.push_back(Vertex(-1.0, -1.0, 1.0,	faceColour));
	vertices.push_back(Vertex(1.0, -1.0, 1.0,	faceColour));
	vertices.push_back(Vertex(1.0, 1.0, 1.0,	faceColour));
	
	//back
	faceColour = GetColour(colourPresets->green, _colour);
	vertices.push_back(Vertex(1.0, 1.0, -1.0,	faceColour));
	vertices.push_back(Vertex(1.0, -1.0, -1.0,	faceColour));
	vertices.push_back(Vertex(-1.0, -1.0, -1.0, faceColour));
	vertices.push_back(Vertex(-1.0, 1.0, -1.0,	faceColour));

	//right
	faceColour = GetColour(colourPresets->blue, _colour);
	vertices.push_back(Vertex(1.0, 1.0, 1.0,	faceColour));
	vertices.push_back(Vertex(1.0, -1.0, 1.0,	faceColour));
	vertices.push_back(Vertex(1.0, -1.0, -1.0,	faceColour));
	vertices.push_back(Vertex(1.0, 1.0, -1.0,	faceColour));

	//left
	faceColour = GetColour(colourPresets->yellow, _colour);
	vertices.push_back(Vertex(-1.0, 1.0, -1.0,	faceColour));
	vertices.push_back(Vertex(-1.0, -1.0, -1.0, faceColour));
	vertices.push_back(Vertex(-1.0, -1.0, 1.0,	faceColour));
	vertices.push_back(Vertex(-1.0, 1.0, 1.0,	faceColour));

	//top
	faceColour = GetColour(colourPresets->cyan, _colour);
	vertices.push_back(Vertex(-1.0, 1.0, -1.0,	faceColour));
	vertices.push_back(Vertex(-1.0, 1.0, 1.0,	faceColour));
	vertices.push_back(Vertex(1.0, 1.0, 1.0,	faceColour));
	vertices.push_back(Vertex(1.0, 1.0, -1.0,	faceColour));

	//bottom
	faceColour = GetColour(colourPresets->magenta, _colour);
	vertices.push_back(Vertex(-1.0, -1.0, -1.0, faceColour));
	vertices.push_back(Vertex(-1.0, -1.0, 1.0,	faceColour));
	vertices.push_back(Vertex(1.0, -1.0, 1.0,	faceColour));
	vertices.push_back(Vertex(1.0, -1.0, -1.0,	faceColour));

	return vertices;
	vertices.clear();
}

Colour Shapes::GetColour(Colour _rainbow, Colour* _colour)
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
