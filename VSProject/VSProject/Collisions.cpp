#include "Collisions.h"
#include "RayCast.h"
#include <chrono>


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

	auto fineTime1 = std::chrono::high_resolution_clock::now();

	//Fine collision detection
	for (unsigned i = 0; i < potentialContacts.size(); i++)
	{
		if (potentialContacts[i].prim1->type == PrimitiveType::BOX && potentialContacts[i].prim2->type == PrimitiveType::BOX)
			sat->Test(std::move(potentialContacts[i].prim1), std::move(potentialContacts[i].prim2));
		else
			fine->DetectContacts(std::move(potentialContacts[i].prim1), std::move(potentialContacts[i].prim2));
	}
	potentialContacts.clear();

	auto fineTime2 = std::chrono::high_resolution_clock::now();

	if (recordData->detRecordTimes)
	{
		recordData->StoreDetTimes(std::chrono::duration_cast<std::chrono::microseconds>(fineTime2 - fineTime1).count());
	}
}

void Collisions::Resolution()
{
	if (data->contacts.size() <= 0) return;

	for (unsigned i = 0; i < data->contacts.size(); i++)
	{
		if (!data->contacts[i].body1->isStatic && !data->contacts[i].body2->isStatic
			&& i < 30)
		{
			contactDisplays[i].origin = data->contacts[i].point;
			contactDisplays[i].normal = data->contacts[i].normal;
		}

		data->contacts[i].PrepareResolution();
	}

	auto totalTime1 = std::chrono::high_resolution_clock::now();

	auto penTime1 = std::chrono::high_resolution_clock::now();
	resolution->SortContactsByPenetration(data->contacts);
	resolution->PenetrationResolution(data->contacts);
	auto penTime2 = std::chrono::high_resolution_clock::now();

	auto velTime1 = std::chrono::high_resolution_clock::now();
	resolution->SortContactsByVelocityMag(data->contacts);
	resolution->VelocityResolution(data->contacts);
	auto velTime2 = std::chrono::high_resolution_clock::now();

	auto totalTime2 = std::chrono::high_resolution_clock::now();

	if (recordData->resRecordTimes)
	{
		auto penDuration = std::chrono::duration_cast<std::chrono::microseconds>(penTime2 - penTime1).count();
		auto velDuration = std::chrono::duration_cast<std::chrono::microseconds>(velTime2 - velTime1).count();
		auto totalDuration = std::chrono::duration_cast<std::chrono::microseconds>(totalTime2 - totalTime1).count();

		recordData->StoreResTimes(penDuration, velDuration, totalDuration);
	}

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


RecordData::RecordData()
{
	if (detRecordTimes)
	{
		detFineTimes = new long long[100];
	}
	if (resRecordTimes)
	{
		penTimes = new long long[100];
		velTimes = new long long[100];
		totalTimes = new long long[100];
	}
}

RecordData::~RecordData()
{
	if (detRecordTimes)
	{
		delete[] detFineTimes;
	}
	if (resRecordTimes)
	{
		delete[] penTimes;
		delete[] velTimes;
		delete[] totalTimes;
	}
}

void RecordData::StoreDetTimes(long long f)
{
	detFineTimes[detRecordIndex] = f;

	//overwrite old values
	detRecordIndex++;
	if (detRecordIndex >= 100)
	{
		detRecordIndex = 0;
		std::cout << "100 values gathered, press C to save to file." << std::endl;

		if (Global::writeDataToFile)
		{
			//write to file
			std::ofstream file;
			file.open("stats_detection.csv");
			file.clear();

			std::cout << "Writing to file... ";

			for (uint16_t i = 0; i < 100; i++)
			{
				file << detFineTimes[i] << "\n";
			}

			file.close();
			std::cout << "Done!" << std::endl;
			Global::writeDataToFile = false;
		}
	}
}

void RecordData::StoreResTimes(long long p, long long v, long long t)
{
	penTimes[resRecordIndex] = p;
	velTimes[resRecordIndex] = v;
	totalTimes[resRecordIndex] = t;

	//overwrite old values
	resRecordIndex++;
	if (resRecordIndex >= 100)
	{
		resRecordIndex = 0;
		std::cout << "100 values gathered, press C to save to file." << std::endl;

		if (Global::writeDataToFile)
		{
			//write to file
			std::ofstream file;
			file.open("stats_resolution.csv");
			file.clear();

			std::cout << "Writing to file... ";

			for (uint16_t i = 0; i < 100; i++)
			{
				file << penTimes[i] << "," << velTimes[i] << "," << totalTimes[i] << "\n";
			}

			file.close();
			std::cout << "Done!" << std::endl;
			Global::writeDataToFile = false;
		}
	}
}