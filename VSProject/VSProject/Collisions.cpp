#include "Collisions.h"

void Collisions::DetectFine(Primitive* prim1, Primitive* prim2)
{
	collisionFine->DetectContacts(prim1, prim2);
}
