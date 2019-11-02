#pragma once
#include "Utilities.h"

class ShapeVertices
{
public:
	ShapeVertices() = default;
	~ShapeVertices() = default;

	std::vector<Vertex> GetCubeVertices(Colour* _colour = NULL);
	std::vector<Vertex> GetPlaneVertices(Colour* _colour = NULL);

	std::unique_ptr<ColourPresets> colourPresets = std::make_unique<ColourPresets>();

private:


	Colour GetColour(Colour _rainbow, Colour* _colour = NULL);
};