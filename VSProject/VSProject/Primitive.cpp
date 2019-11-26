#include "Primitive.h"


void Primitive::Update(double deltaTime)
{	
	//Update transforms if they have changed.
	if (updateTransform) UpdateTransform();

	rigidbody.CalculateVelocity(translation);
}

void Primitive::Tween(double deltaTime, float speed, const Vector3& direction, float approxDistance)
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

	updateTransform = true;
}

void Primitive::SetTweenOrigin()
{
	tweenOrigin = translation;
}

void Primitive::UpdateTransform()
{
	transform.Identity();

	//Update the transform matrix4x4 with the new transform vectors.
	Mathe::Translate(transform, translation.x, translation.y, translation.z);
	Mathe::Rotate(transform, rotation.x, rotation.y, rotation.z);
	collisionVolume.axisMat = transform;
	Mathe::Scale(transform, scale.x, scale.y, scale.z);

	//Define bounding volume based on the primitive's vertices.
	//Gets the min and max values.
	//boundingVolume->SetVertices(vertices);

	boundingVolume.Update(translation, 0, scale);
	collisionVolume.Update(translation, radius, scale / 2, rotation);

	updateTransform = false;
}
