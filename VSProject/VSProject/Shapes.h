#pragma once
#include "Utilities.h"

class ShapeVertices
{
public:
	ShapeVertices() = default;
	~ShapeVertices() = default;

	std::vector<Vertex> GetCubeVertices(Colour* _colour = NULL);
	std::vector<Vertex> GetPlaneVertices(Colour* _colour = NULL);
	std::vector<Vertex> GetSphereVertices(float radius, Colour* _colour = NULL, int sectorCount = 10, int stackCount = 10);

	std::unique_ptr<ColourPresets> colourPresets = std::make_unique<ColourPresets>();

private:


	Colour GetColour(Colour _rainbow, Colour* _colour = NULL);
};