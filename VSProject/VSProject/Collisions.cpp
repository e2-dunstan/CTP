#include "Collisions.h"

void Collisions::DetectFine(Primitive* prim1, Primitive* prim2, bool output)
{
	collisionFine->outputStr = "Collision detected: ";
	collisionFine->DetectContacts(prim1, prim2);

	if (output && outputCollisionData && collisionFine->outputStr != "Collision detected: ")
	{
		std::cout << collisionFine->outputStr << std::endl;
	}
}
