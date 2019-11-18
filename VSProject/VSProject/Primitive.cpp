#include "Primitive.h"

void Primitive::Tween(double deltaTime, float speed, Vector3 direction, float approxDistance)
{
	if (!initialised) return;

	if (!tweenMaxSet)
	{
		tweenMax = tweenOrigin + (direction * approxDistance);
		tweenMaxSet = true;
	}

	translation += (direction * (deltaTime * speed)) * (moveTowards ? 1 : -1);

	//Reverse movement
	if ((moveTowards && translation.Distance(tweenMax) < 0.2)
		|| (!moveTowards && translation.Distance(tweenOrigin) < 0.2))
	{
		moveTowards = !moveTowards;
	}

	updateTransforms = true;
}

void Primitive::SetTweenOrigin()
{
	tweenOrigin = translation;
}
