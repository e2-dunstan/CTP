#pragma once
#include "Primitive.h"


class Collisions;
class CollisionResolution2;
class CollisionResolution3;

enum class BounceCombineType
{
	MAXIMUM, MULTIPLY, MINIMUM, AVERAGE
};

struct Contact
{
	friend struct CollisionData;
	friend class Collisions;
	friend class CollisionResolution2;
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
	float penetrationDepth;

	Primitive* body1;
	Primitive* body2;

	float restitution = 0.0f;
	float friction = 0.0f;


protected:

	void ResolvePenetration();
	//Vector3 pseudoLinearVelocities[2] = { Vector3() };
	//Vector3 pseudoAngularVelocities[2] = { Vector3() };
	//float pseudoLambdas[2] = { 0.01f, 0.01f };

	void ResolveVelocity();
	//Vector3 linearImpulses[2] = { Vector3() };
	//Vector3 angularImpulses[2] = { Vector3() };
	//float lambdas[2] = { 0.01f, 0.01f };

	void PrepareResolution();

	void ApplyAngularMoveLimit(float& linear, float& angular, const float projection/*const float objMag*/);

	Vector3 FrictionlessImpulse();
	Vector3 FrictionImpulse();

	void MatchRigidbodyAwakeStates();

	Matrix3 worldToContact = Matrix3();

	Vector3 closingVelocity = Vector3();

	Vector3 relContactPos1 = Vector3();
	Vector3 relContactPos2 = Vector3();

	Vector3 linearChange[2] = { Vector3(), Vector3() };
	Vector3 angularChange[2] = { Vector3(), Vector3() };

	Vector3 velocityChange[2] = { Vector3(), Vector3() };
	Vector3 rotationChange[2] = { Vector3(), Vector3() };

private:
	void CalculateContactBasisMatrices();
	void CalculateDesiredDeltaVelocity();
	void CalculateClosingVelocities();

	float desiredDeltaVelocity = 0;
	Matrix3 contactToWorld = Matrix3();

	BounceCombineType bounceCombineType = BounceCombineType::AVERAGE;

	uint16_t numIterations = 0;
	uint16_t maxIterations = 5; //minimum

	bool penetrationResolved = false;
	double minPenetration = 0.0;
	float bias = 0.3f;

	float storedNormalImpulseScale = 0.0f;
};