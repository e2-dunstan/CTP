#include "Collisions.h"
#include "RayCast.h"

Collisions::Collisions()
{
	fine = std::make_unique<CollisionFine>(data->contacts);
	sat = std::make_unique<SAT>(data->contacts);
}

void Collisions::DetectCoarse(Primitive* prim1, Primitive* prim2)
{
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
		{
			//if (dynamic_cast<Box*>(potentialContacts[i].prim1)->scale.SquaredMagnitude() >= dynamic_cast<Box*>(potentialContacts[i].prim2)->scale.SquaredMagnitude())
				sat->Test(potentialContacts[i].prim1, potentialContacts[i].prim2);
			//else
			//	sat->Test(potentialContacts[i].prim2, potentialContacts[i].prim1);
		}
		else
			fine->DetectContacts(potentialContacts[i].prim1, potentialContacts[i].prim2);
	}
	potentialContacts.clear();
}

void Collisions::Resolution()
{
	if (data->contacts.size() <= 0) return;

	for (unsigned i = 0; i < data->contacts.size(); i++)
	{
		contactDisplays[i].origin = data->contacts[i].point;
		contactDisplays[i].normal = data->contacts[i].normal;

		data->contacts[i].PrepareResolution();
	}
	//resolution3->Update(data->contacts);

	data->SortContactsByPenetration();
	resolution2->PenetrationResolution(data->contacts);
	data->SortContactsByVelocityMag();
	resolution2->VelocityResolution(data->contacts);

	data->contacts.clear();
}

void Collisions::DrawContacts()
{
	//if (data->contacts.size() <= 0) return;

	for (unsigned i = 0; i < 30; i++)
	{
		if (contactDisplays[i].origin == Vector3()) continue;

		//glLoadIdentity();

		glBegin(GL_LINES);

		glColor3f(1, 0, 0);
		glVertex3f(contactDisplays[i].origin.x, contactDisplays[i].origin.y, contactDisplays[i].origin.z);
		Vector3 point2 = contactDisplays[i].origin + contactDisplays[i].normal * 2.0;
		glVertex3f(point2.x, point2.y, point2.z);

		glEnd();

		glPushMatrix();
		glTranslated(contactDisplays[i].origin.x, contactDisplays[i].origin.y, contactDisplays[i].origin.z);
		glutWireSphere(0.15, 4, 4);
		glPopMatrix();

		//if (i >= 4) Global::shouldUpdate = false;

		glFlush();
	}
	
}
