#pragma once
#include "Utilities.h"

namespace Shapes
{
	class ShapeVertices
	{
	public:
		ShapeVertices() = default;
		~ShapeVertices() = default;

		static std::vector<Vertex> GetCubeVertices(const Colour& _colour = Colours::white);
		static std::vector<Tri> GetCubeTris(const Colour& _colour = Colours::white);

		static std::vector<Tri> GetPlaneTris(const Colour& _colour = Colours::white);
		static std::vector<Vertex> GetSphereVertices(float radius, const Colour& _colour = Colours::white, int sectorCount = 10, int stackCount = 10);
		static std::vector<Vertex> GetCapsuleVertices(float radius, float straight, const Colour& _colour = Colours::white, int sectorCount = 10, int stackCount = 9);
		static std::vector<Vertex> GetCylinderVertices(float radius, float straight, const Colour& _colour = Colours::white, int sectorCount = 10);

	private:
		static Colour GetColour(const Colour& _rainbow, const Colour& _colour);
	};
};