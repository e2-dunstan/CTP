#pragma once
#include "Camera.h"

struct RayCameraPrimitive
{
	RayCameraPrimitive(std::vector<Tri>& t, Matrix4* trans)
		: tris(t), transform(*trans) {}

	std::vector<Tri>& tris;
	Matrix4& transform;
};

class RayCamera
{
public:
	RayCamera();
	~RayCamera() = default;


	// -- RAYCAST -- //
	void AddPrimitive(std::vector<Tri>& tris, Matrix4* trans);
	void CastRays(const Vector3& camPos, const uint16_t width, const uint16_t height);
	void DrawLatestRay();

private:

	Ray GetRayAt(int pX, int pY, const float width, const float height, const Vector3& cameraPos);
	void SetModelViewMatrix();

	void SavePixelsToFile(const unsigned char* pixels, const uint16_t arrSize, const uint16_t width, const uint16_t height);

	std::vector<RayCameraPrimitive> triPrimitives;

	Ray ray = Ray(Vector3(), Vector3());

	bool store = true;

	bool draw = false;
	Vector3 cPosTemp;

	Matrix4 modelViewMatrix;
};