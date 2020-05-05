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

		//static std::vector<Tri> GetSphereTris(float radius, const Colour& _colour = Colours::white, const uint16_t subdivisionCount = 1);
		static std::vector<Tri> GetSphereVertices(float radius, const Colour& _colour = Colours::white, uint16_t sectorCount = 10, uint16_t stackCount = 10);

		static std::vector<Tri> GetPlaneTris(const Colour& _colour = Colours::white);
		static std::vector<Vertex> GetCapsuleVertices(float radius, float straight, const Colour& _colour = Colours::white, int sectorCount = 10, int stackCount = 9);
		static std::vector<Vertex> GetCylinderVertices(float radius, float straight, const Colour& _colour = Colours::white, int sectorCount = 10);

	private:
		static Colour GetColour(const Colour& _rainbow, const Colour& _colour);


		//Sphere stuff
		//static void ComputeFaceNormal(const Vector3& v1, const Vector3& v2, const Vector3& v3, Vector3& normal);
		//static void ComputeHalfVertex(const float v1[3], const float v2[3], float length, float newV[3]);
		//static void ComputeFaceNormal(const float v1[3], const float v2[3], const float v3[3], Vector3& n);
	};
};