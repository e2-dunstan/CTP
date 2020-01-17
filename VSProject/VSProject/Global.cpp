#include "Global.h"

namespace Global
{
	//time
	double deltaTime = 0;

	//physics
	const Vector3 gravity(0, -9.81, 0);
	const double gravityMag = -9.81;
	const double airDensity = 1.225;
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
