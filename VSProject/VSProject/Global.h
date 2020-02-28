#pragma once
#include "mathe.h"

namespace Global
{
	//time
	extern double deltaTime;
	//double GetDeltaTime();
	//void SetDeltaTime(const double d);

	//physics
	extern const Vector3 gravity;
	extern const double gravityMag;
	extern const double airDensity;

	extern bool shouldUpdate;
}