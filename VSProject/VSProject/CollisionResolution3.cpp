#include "CollisionResolution3.h"


void CollisionResolution3::Update(const std::vector<Contact>& contacts)
{
	const uint16_t numPrimitives = contacts.size() * 2;

	//don't need to clear non pen as they'll be overwritten in the next step
	nonPenetrationJacobians.resize(contacts.size());
	lambdas.resize(numPrimitives);
	//frictionJacobians.clear();

	for (uint16_t c = 0; c < contacts.size(); c++)
	{
		nonPenetrationJacobians[c] = Jacobian(contacts[c].normal, contacts[c].point, contacts[c].body1->translation, contacts[c].body2->translation);
		//if (contacts[c].friction != 0)
		//	frictionJacobians.push_back(Jacobian(contacts[c].point, Vector3(), Vector3())); //to be determined
	}


	Vector3 estVel_linear, newVel_linear, estVel_angular, newVel_angular;

	for (uint16_t v = 0; v < numPrimitives; v++)
	{
		RigidBody* body = v % 2 == 0 ? &contacts[v / 2].body1->rigidbody : &contacts[v / 2].body2->rigidbody;

		//q2 est = q1 + delta time * M inverse * external forces
		estVel_linear = body->velocity + (Global::gravity * Global::deltaTime);
		//q2 = q2 est + M inverse * constraint impulse
		newVel_linear = estVel_linear + nonPenetrationJacobians[v / 2].Multiply(lambdas[v]);

		//What about angular velocities
		body->AddVelocityChange(newVel_linear * Global::deltaTime);
	}
	
}