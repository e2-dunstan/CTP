#pragma once
#include "Primitive.h"


class Collisions;
class CollisionResolution;

enum class BounceCombineType
{
	MAXIMUM, MULTIPLY, MINIMUM, AVERAGE
};

struct Contact
{
	friend struct CollisionData;
	friend class Collisions;
	friend class CollisionResolution;
	friend class CollisionResolution3;

public:
	//Contact() = default;
	Contact(Primitive* prim1, Primitive* prim2)
		: body1(prim1), body2(prim2)
	{
		//points.reserve(pointCount);
		//penetrationDepths.reserve(pointCount);
		if (body1->type != PrimitiveType::PLANE) body1->colliding = true;
		if (body2->type != PrimitiveType::PLANE) body2->colliding = true;
	};
	~Contact() = default;

	Vector3 point;
	Vector3 normal;
	float penetrationDepth = 0;

	Primitive* body1;
	Primitive* body2;

	float restitution = 0.0f;
	float friction_static = 0.0f;
	float friction_dynamic = 0.0f;


protected:

	//void Resolve();

	bool IsPenetrationResolved(float epsilon) { return abs(penetrationDepth) < epsilon; }
	bool IsVelocityResolved(float epsilon) { return abs(desiredDeltaVelocity) < epsilon; }

	void ResolvePenetration();
	void ResolveVelocity();

	void PrepareResolution();

	void ApplyAngularMoveLimit(float& linear, float& angular, const float projection/*const float objMag*/);

	Vector3 FrictionlessImpulse();
	Vector3 FrictionImpulse();

	void MatchRigidbodyAwakeStates();

	Vector3 closingVelocity = Vector3();

	Vector3 relContactPos1 = Vector3();
	Vector3 relContactPos2 = Vector3();

	Vector3 linearChange[2] = { Vector3(), Vector3() };
	Vector3 angularChange[2] = { Vector3(), Vector3() };

	Vector3 velocityChange[2] = { Vector3(), Vector3() };
	Vector3 rotationChange[2] = { Vector3(), Vector3() };

	Matrix3 worldToContact = Matrix3();

	unsigned int iterations = 0;
	const unsigned int maximumIterations = 30;

private:
	void CalculateContactBasisMatrices();
	void CalculateDesiredDeltaVelocity();
	void CalculateClosingVelocities();

	float desiredDeltaVelocity = 0;
	Matrix3 contactToWorld = Matrix3();

	BounceCombineType bounceCombineType = BounceCombineType::AVERAGE;
};