#pragma once
#include <SFML/Graphics.hpp>
#include "Camera.h"

struct RayCameraPrimitive
{
	RayCameraPrimitive(std::vector<Tri> t, Matrix4& trans, float sphereRadius = 0, Vector3 sphereOrigin = Vector3())
		: tris(t)//, radius(sphereRadius), origin(sphereOrigin)
	{
		for (uint16_t i = 0; i < tris.size(); i++)
		{
			Mathe::Transform(tris[i].positions[0], trans);
			Mathe::Transform(tris[i].positions[1], trans);
			Mathe::Transform(tris[i].positions[2], trans);
			Mathe::Transform(tris[i].normal, trans);
			tris[i].normal = tris[i].normal.Normalise();
		}
	}

	std::vector<Tri> tris;
	//Matrix4& transform;
	//only used if sphere
	//float radius = 0;
	//Vector3 origin;
};

class RayCamera
{
public:
	RayCamera() = default;
	~RayCamera() = default;


	// -- RAYCAST -- //
	void AddPrimitive(std::vector<Tri>& tris, Matrix4& trans, float sphereRadius = 0, const Vector3& sphereOrigin = Vector3());
	void CastRays(const Vector3& camPos, const uint16_t width, const uint16_t height);
	void DrawLatestRay();

private:

	Ray GetRayAt(const int pX, const int pY, const float width, const float height, const Vector3& cameraPos);
	Vector3 ComputeRayHit(Vector3& pathThroughput, const Vector3& normal, const Vector3& point, uint16_t pathLength);

	void SetModelViewMatrix();
	void SavePixelsToFile(const sf::Uint8* pixels, const uint16_t arrSize, const uint16_t width, const uint16_t height);

	std::vector<RayCameraPrimitive> triPrimitives;

	Ray ray = Ray(Vector3(), Vector3());
	//Ray debugRays[3];
	//unsigned int debugRaysCounter = 0;

	bool store = true;

	const bool smoothSpheres = true;

	const bool draw = false;
	Vector3 cPosTemp;

	Matrix4 modelViewMatrix;
};
