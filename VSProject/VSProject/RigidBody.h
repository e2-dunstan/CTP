#pragma once

//Will be used for collision response.
struct RigidBody
{
	RigidBody() = default;
	~RigidBody() = default;

	//Inverse is used to avoid division by 0..
	float inverseMass = 0;
	float drag = 0;
	float friction = 0;

};