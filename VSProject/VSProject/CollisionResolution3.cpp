#include "CollisionResolution3.h"


void CollisionResolution3::Update(std::vector<Contact>& contacts)
{
	for (uint16_t c = 0; c < contacts.size(); c++)
	{
		if (!contacts[c].penetrationResolved)
		{
			contacts[c].ResolvePenetration();
			//for (uint16_t i = 0; i < 3; i++)
			//{
			//}
			contacts[c].penetrationResolved = true;
		}
		contacts[c].ResolveVelocity();
		//for (uint16_t j = 0; j < contacts[c].maxIterations; j++)
		//{
		//}
	}
}






	/*for (uint16_t c = 0; c < contacts.size(); c++)
	{
		//apply non penetration constraint

		double bias = (1.0 / Global::deltaTime);

		//velocities multiplied by the jacobian
		Vector3 v1 = contacts[c].body1->rigidbody.velocity.VectorProduct(contacts[c].normal * -1.0f);
		Vector3 a1 = contacts[c].body1->rigidbody.angularVelocity.VectorProduct(contacts[c].relContactPos1.VectorProduct(contacts[c].normal) * -1.0f);
		Vector3 v2 = contacts[c].body2->rigidbody.velocity.VectorProduct(contacts[c].normal);
		Vector3 a2 = contacts[c].body2->rigidbody.angularVelocity.VectorProduct(contacts[c].relContactPos2.VectorProduct(contacts[c].normal));

		//JnV + (bias * penetration) = 0
		Vector3 sum = v1 + v2 + a1 + a2 + (contacts[c].normal * contacts[c].penetrationDepth * bias);
		std::cout << "Sum (should be zero) with bias of " << bias << " = ";
		sum.DebugOutput();

		if (contacts[c].restitution > 0)
		{
			v1 *= 1.0 + contacts[c].restitution;
			a1 *= 1.0 + contacts[c].restitution;
			v2 *= 1.0 + contacts[c].restitution;
			a2 *= 1.0 + contacts[c].restitution;
		}

		contacts[c].body1->rigidbody.AddConstraintForce(v1);
		contacts[c].body1->rigidbody.AddConstraintTorque(a1);

		contacts[c].body2->rigidbody.AddConstraintForce(v2);
		contacts[c].body2->rigidbody.AddConstraintTorque(a2);





		double bias = 0;
		Vector3 constraintForce = (contacts[c].normal * contacts[c].body1->rigidbody.velocity) + (contacts[c].normal * bias);
		constraintForce *= -1.0f;
		constraintForce = constraintForce / (contacts[c].normal * contacts[c].body1->rigidbody.inverseMass);
		
		Vector3 constraintTorque = (contacts[c].normal * contacts[c].body1->rigidbody.angularVelocity) + (contacts[c].normal * bias);
		constraintTorque *= -1.0f;
		constraintTorque = constraintTorque / (contacts[c].normal * contacts[c].body1->rigidbody.inverseMass);

		contacts[c].body1->rigidbody.AddConstraintForce(constraintForce);
		contacts[c].body1->rigidbody.AddConstraintTorque(constraintTorque);


		bias = 0;
		constraintForce = (contacts[c].normal * contacts[c].body2->rigidbody.velocity) + (contacts[c].normal * bias);
		constraintForce = constraintForce / (contacts[c].normal * contacts[c].body2->rigidbody.inverseMass);

		constraintTorque = (contacts[c].normal * contacts[c].body2->rigidbody.angularVelocity) + (contacts[c].normal * bias);
		constraintTorque = constraintTorque / (contacts[c].normal * contacts[c].body2->rigidbody.inverseMass);

		contacts[c].body2->rigidbody.AddConstraintForce(constraintForce);
		contacts[c].body2->rigidbody.AddConstraintTorque(constraintTorque);


		// acceleration approximation
		// = vel change / delta time

		//vel = prev vel + (delta time * inverse mass) * (constraint force + gravity)




		//for impulse:
		// normal * inverse mass * mag = -(normal * estVel + bias)
		//mag = -(normal * estVel + bias) / (normal * inverseMass)
		
		//Jacobian
		//derivative of C wrt time
		// = J * vel and angular vel
		// C = (pos2 + rotMat2 * relpos2 - pos1 - rotMat1 * relpos1) dot n
		// J

		//if (contacts[c].friction != 0)
		//{
			//apply friction constraint
		//}
	}



	//don't need to clear non pen as they'll be overwritten in the next step
	//nonPenetrationJacobians.resize(contacts.size());
	//lambdas.resize(numPrimitives);
	////frictionJacobians.clear();

	//for (uint16_t c = 0; c < contacts.size(); c++)
	//{
	//	nonPenetrationJacobians[c] = Jacobian(contacts[c].normal, contacts[c].point, contacts[c].body1->translation, contacts[c].body2->translation);
	//	//if (contacts[c].friction != 0)
	//	//	frictionJacobians.push_back(Jacobian(contacts[c].point, Vector3(), Vector3())); //to be determined
	//}


	//Vector3 estVel_linear, newVel_linear, estVel_angular, newVel_angular;

	//for (uint16_t v = 0; v < numPrimitives; v++)
	//{
	//	RigidBody* body = v % 2 == 0 ? &contacts[v / 2].body1->rigidbody : &contacts[v / 2].body2->rigidbody;

	//	//q2 est = q1 + delta time * M inverse * external forces
	//	estVel_linear = body->velocity + (Global::gravity * Global::deltaTime);
	//	//q2 = q2 est + M inverse * constraint impulse
	//	newVel_linear = estVel_linear + nonPenetrationJacobians[v / 2].Multiply(lambdas[v]);

	//	//What about angular velocities
	//	body->AddVelocityChange(newVel_linear * Global::deltaTime);
	//}
}

void CollisionResolution3::ComputeConstraintForces()
{
	const uint16_t bodyCount = constraintCount * 2;

	for (uint16_t i = 0; i < bodyCount; i++)
	{
		//constraintForces[i].reactionForce = Vector3();
		//constraintForces[i].reactionTorque = Vector3();
	}

	for (uint16_t i = 0; i < constraintCount; i++)
	{

	}
}*/
