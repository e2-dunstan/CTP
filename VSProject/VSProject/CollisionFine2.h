#pragma once
#include "CollisionVolume.h"
#include "CollisionData.h"
#include "SeparatingAxisTheorem.h"

class CollisionFine2
{
public:
	CollisionFine2() = default;
	~CollisionFine2() = default;

	void DetectContacts(Primitive* prim1, Primitive* prim2);

private:
	std::unique_ptr<SAT> sat = std::make_unique<SAT>();

	std::vector<Contact>& contacts;

	void PlanePoint(); //hopefully generic func
	void PlaneBox(Primitive* prim1, Primitive* prim2);
	void PlaneSphere(Primitive* prim1, Primitive* prim2);
	//void PlaneCylinder(Primitive* prim1, Primitive* prim2);
	//void PlaneCapsule(Primitive* prim1, Primitive* prim2);

	void BoxSphere(Primitive* prim1, Primitive* prim2);
	void SphereSphere(Primitive* prim1, Primitive* prim2);
};