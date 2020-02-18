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
	Vector3 CalculateClosingVelocity(const Primitive& prim);
	float CalculateDesiredChangeInVelocity(const Contact& contact, const Vector3& contactVel);
	Vector3 CalculateFrictionlessImpulse(const Contact& contact);
	Vector3 CalculateFrictionImpulse(const Contact& contact);
	void CalculateVelocityChangesFromImpulse(const Contact& contact, Vector3 impulse);

	void ApplyAngularMoveLimit(float& linear, float& angular, const float objMag);

	//Transforms
	Matrix contactToWorld = Matrix();
	Matrix worldToContact = Matrix();

	Vector3 relContactPos1 = Vector3();
	Vector3 relContactPos2 = Vector3();
};