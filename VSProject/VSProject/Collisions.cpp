#include "Collisions.h"
#include "RayCast.h"

Collisions::Collisions()
{
	fine = std::make_unique<CollisionFine>(data->contacts);
	sat = std::make_unique<SAT>(data->contacts);
}

//Using shared ptr ensures that it's not null
void Collisions::DetectCoarse(Primitive* prim1, Primitive* prim2)
{
	if (prim1->freeze && prim2->freeze) return;
	if (coarse->Overlapping(prim1->boundingVolume, prim2->boundingVolume))
	{
		potentialContacts.push_back(PotentialContact(prim1, prim2));
	}
}

void Collisions::DetectFine()
{
	if (potentialContacts.size() <= 0) return;

	for (unsigned i = 0; i < potentialContacts.size(); i++)
	{
		if (potentialContacts[i].prim1->type == PrimitiveType::BOX && potentialContacts[i].prim2->type == PrimitiveType::BOX)
			sat->Test(std::move(potentialContacts[i].prim1), std::move(potentialContacts[i].prim2));
		else
			fine->DetectContacts(std::move(potentialContacts[i].prim1), std::move(potentialContacts[i].prim2));
	}
	potentialContacts.clear();
}

void Collisions::Resolution()
{
	if (data->contacts.size() <= 0) return;

	for (unsigned i = 0; i < data->contacts.size(); i++)
	{
		if (!data->contacts[i].body1->isStatic && !data->contacts[i].body2->isStatic)
		{
			contactDisplays[i].origin = data->contacts[i].point;
			contactDisplays[i].normal = data->contacts[i].normal;
		}

		data->contacts[i].PrepareResolution();
	}

	resolution->SortContactsByPenetration(data->contacts);
	resolution->PenetrationResolution(data->contacts);
	resolution->SortContactsByVelocityMag(data->contacts);
	resolution->VelocityResolution(data->contacts);

	data->contacts.clear();

	//data->BatchContacts();
	//for (unsigned int i = 0; i < data->batchedContacts.size(); i++)
	//{
	//}
	//data->batchedContacts.clear();
	//for (unsigned i = 0; i < data->contacts.size();)
	//{
	//	if (data->contacts[i].IsPenetrationResolved(0.0001f) && data->contacts[i].IsVelocityResolved(0.001f))
	//	{
	//		data->contacts.erase(data->contacts.begin() + i);
	//	}
	//	else i++;
	//}
}

void Collisions::DrawContacts()
{
	if (data->contacts.size() <= 0) return;

	for (unsigned i = 0; i < 30; i++)
	{
		if (contactDisplays[i].origin == Vector3()) continue;

		glBegin(GL_LINES);

		glColor3f(1, 0, 0);

		glVertex3f((GLfloat)contactDisplays[i].origin.x, (GLfloat)contactDisplays[i].origin.y, (GLfloat)contactDisplays[i].origin.z);

		Vector3 point2 = contactDisplays[i].origin + contactDisplays[i].normal * 2.0;
		glVertex3f((GLfloat)point2.x, (GLfloat)point2.y, (GLfloat)point2.z);

		glEnd();

		glPushMatrix();
		glTranslated(contactDisplays[i].origin.x, contactDisplays[i].origin.y, contactDisplays[i].origin.z);
		glutWireSphere(0.15, 4, 4);
		glPopMatrix();

		glFlush();
	}
}
