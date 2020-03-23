#pragma once
#include "CollisionData.h"

class CollisionResolution3
{
public:
	CollisionResolution3() = default;
	~CollisionResolution3() = default;

	void Update(const std::vector<Contact>& contacts);

private:

	std::vector<Jacobian> nonPenetrationJacobians;
	//std::vector<Jacobian> frictionJacobians;

	std::vector<Vector3> lambdas;
};