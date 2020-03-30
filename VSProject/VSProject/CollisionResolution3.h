#pragma once
#include "CollisionData.h"

// 1. Integrate applied forces using semi-implicit Euler
//		- Yields approximate velocities
//		- Correct the velocities if they violate the contraint
//
// 2. Apply impulses sequentially for all contraints to correct the velocity errors
//		- Repeat for several iterations or until impulses become small
//
// 3. Use the new velocities to simulate motion and update positions
//		- Again using semi-implicit Euler

class CollisionResolution3
{
public:
	CollisionResolution3() = default;
	~CollisionResolution3() = default;

	void Update(std::vector<Contact>& contacts);

private:

	//void ComputeConstraintForces();



	const uint16_t maxIterationsPerFrame = 3;

};