#pragma once
#include <SFML/Graphics.hpp>
#include "Camera.h"

struct RayCameraPrimitive
{
	RayCameraPrimitive() = default;
	RayCameraPrimitive(std::vector<Tri> t, Matrix4& trans, float sphereRadius = 0)
		: tris(t), transform(trans)/*, radius(sphereRadius)*/ {}

	std::vector<Tri> GetTransformedTris()
	{
		for (uint16_t i = 0; i < tris.size(); i++)
		{
			Mathe::Transform(tris[i].positions[0], transform);
			Mathe::Transform(tris[i].positions[1], transform);
			Mathe::Transform(tris[i].positions[2], transform);
			Mathe::Transform(tris[i].normal, transform);
			tris[i].normal = tris[i].normal.Normalise();
		}
	}

	std::vector<Tri>& tris;
	Matrix4& transform;
	//only used if sphere
	//float radius;
};

class RayCamera
{
public:
	RayCamera() = default;
	~RayCamera() = default;


	// -- RAYCAST -- //
	void AddPrimitive(std::vector<Tri>& tris, Matrix4& trans/*, float sphereRadius = 0*/);
	void CastRays(const Vector3& camPos, const uint16_t width, const uint16_t height);
	void DrawLatestRay();

private:

	Ray GetRayAt(const int pX, const int pY, const float width, const float height, const Vector3& cameraPos);
	Vector3 ComputeRayHit(Vector3& pathThroughput, const Vector3& normal, const Vector3& point, unsigned int pathLength);

	void SetModelViewMatrix();
	void SavePixelsToFile(const sf::Uint8* pixels, const uint16_t arrSize, const uint16_t width, const uint16_t height);

	Vector3 GetSphereNormal(const Tri& tri, const float radius, const Vector3& intersection, Matrix4* trans);

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