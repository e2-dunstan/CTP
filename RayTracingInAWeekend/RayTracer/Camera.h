#pragma once
#include "Utilities.h"

class Camera
{
public:
	Camera(Vector3 lookFrom, Vector3 lookAt, Vector3 viewUp, double vFov, double aspect, double aperture, double focusDist)
	{
		origin = lookFrom;
		lensRadius = aperture / 2.0;

		auto theta = DegreesToRadians(vFov);
		auto halfHeight = tan(theta / 2.0);
		auto halfWidth = aspect * halfHeight;

		w = (lookFrom - lookAt).Normalise();
		u = viewUp.VectorProduct(w).Normalise();
		v = w.VectorProduct(u);

		lowerLeftCorner = origin 
						  - u * halfWidth * focusDist
						  - v * halfHeight * focusDist
						  - w * focusDist;
		horizontal = u * 2.0 * halfWidth * focusDist;
		vertical = v * 2.0 * halfHeight * focusDist;
	}

	Ray GetRay(double s, double t)
	{
		Vector3 rd = RandomInDisk() * lensRadius;
		Vector3 offset = u * rd.x + v * rd.y;
		return Ray(origin + offset, lowerLeftCorner + horizontal * s + vertical * t - origin - offset);
	}

	Vector3 lowerLeftCorner;
	Vector3 horizontal;
	Vector3 vertical;
	Vector3 origin;
	Vector3 u, v, w;
	double lensRadius;
};