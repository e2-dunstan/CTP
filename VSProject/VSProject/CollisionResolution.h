#pragma once
#include "CollisionData.h"
//#include "Primitive.h"

class CollisionResolution
{
public:
	CollisionResolution() = default;
	~CollisionResolution() = default;

	void ResolveInterpenetration(const Contact& contact);
	void ResolveCollision(const Contact& contact);

private:
	Matrix CalculateContactBasis(const Contact& contact);
	float CalculateChangeInVelocity(const Contact& contact);
	Vector3 CalculateClosingVelocity(const Contact& contact);
	float CalculateDesiredChangeInVelocity(const Contact& contact, const Vector3& contactVel);
	void CalculateVelocityChangesFromImpulse(const Contact& contact, Vector3 impulse);

	//Transforms
	Matrix contactToWorld = Matrix(4,4);
	Matrix worldToContact = Matrix(4,4);

	Vector3 relContactPos1 = Vector3();
	Vector3 relContactPos2 = Vector3();
};