#include "CollisionResolution2.h"

void CollisionResolution2::ResolveContacts(const std::vector<Contact>& contacts)
{
	InitialiseContacts(contacts);
	CalculatePenetrationResolution(contacts);
	CalculateVelocityResolution(contacts);
}

void CollisionResolution2::InitialiseContacts(const std::vector<Contact>& contacts)
{
	for (auto contact : contacts)
	{
		contact.CalculateResolutionValues();
	}
}

void CollisionResolution2::CalculatePenetrationResolution(const std::vector<Contact>& contacts)
{
}

void CollisionResolution2::CalculateVelocityResolution(const std::vector<Contact>& contacts)
{
}
