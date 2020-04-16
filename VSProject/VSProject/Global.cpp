#include "Global.h"

namespace Global
{
	//time
	double deltaTime = 0;
	const double fixedDeltaTime = 1000.0 / 100.0; //basically number of physics updates per second

	//physics
	const Vector3 gravity(0, -9.81, 0);
	const double gravityMag = -9.81;
	const double airDensity = 1.225;

	bool shouldUpdate = true;

	bool writeContactDataToFile = false;

	const Vector3 skyColour = Vector3(0.529, 0.808, 0.922);
}

//double Global::GetDeltaTime()
//{
//	return deltaTime;
//}
//
//void Global::SetDeltaTime(const double d)
//{
//	deltaTime = d;
//}
