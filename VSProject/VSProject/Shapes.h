#pragma once
#include "Utilities.h"

class ShapeVertices
{
public:
	ShapeVertices() {
		baseBox = GetCubeVertices(&Colour(0, 0, 0));
	};
	~ShapeVertices() = default;

	std::vector<Vertex> GetCubeVertices(Colour* _colour = NULL);
	std::vector<Vertex> GetPlaneVertices(Colour* _colour = NULL);
	std::vector<Vertex> GetSphereVertices(float radius, Colour* _colour = NULL, int sectorCount = 10, int stackCount = 10);

	std::unique_ptr<ColourPresets> colourPresets = std::make_unique<ColourPresets>();

	std::vector<Vertex> baseBox;

private:

	Colour GetColour(Colour _rainbow, Colour* _colour = NULL);
};