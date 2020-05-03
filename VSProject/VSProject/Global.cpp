#include "Global.h"

namespace Global
{
	//time
	float deltaTime = 0;
	const double fixedDeltaTime = 1000.0 / 100.0; //basically number of physics updates per second

	//physics
	const Vector3 gravity(0, -9.81f, 0);
	const float gravityMag = -9.81f;
	const float airDensity = 1.225f;

	bool shouldUpdate = true;

	bool writeContactDataToFile = false;

	const Vector3 skyColour = Vector3(0.529f, 0.808f, 0.922f);
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
