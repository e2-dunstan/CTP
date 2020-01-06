#pragma once
#include "CollisionData.h"
//#include "Primitive.h"

class CollisionResolution
{
public:
	CollisionResolution() = default;
	~CollisionResolution() = default;

	void Resolve(Contact& contact);

private:
	Matrix CalculateContactBasis();
	float CalculateChangeInVelocity();

	Contact& contact;
	//Transforms
	Matrix contactToWorld;
	Matrix worldToContact;

};