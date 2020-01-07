#include "Global.h"

namespace Global
{
	//time
	double deltaTime = 0;

	//physics
	const Vector3 gravity(0, -0.0981, 0);
	const double gravityMag = -0.0981;
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
