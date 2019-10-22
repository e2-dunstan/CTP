#pragma once
#include "Utilities.h"

class Shapes
{
public:
	Shapes() = default;
	~Shapes() = default;

	std::vector<Vertex> GetCubeVertices(Colour* _colour = NULL);

	ColourPresets* colourPresets = new ColourPresets();

private:


	Colour GetColour(Colour _rainbow, Colour* _colour = NULL);
};