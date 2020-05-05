#pragma once
#include "mathe.h"
#include "RayCast.h"

namespace Global
{
	//time
	extern float deltaTime;
	extern const double fixedDeltaTime;
	//double GetDeltaTime();
	//void SetDeltaTime(const double d);

	//physics
	extern const Vector3 gravity;
	extern const float gravityMag;
	extern const float airDensity;

	extern bool shouldUpdate;

	extern bool writeDataToFile;

	extern const Vector3 skyColour;
}