#include "Contact.h"


Contact::Contact(Primitive* prim1, Primitive* prim2)
{
	bodies[0] = prim1;
	bodies[1] = prim2;
}

Contact::~Contact()
{
	if (bodies)
		delete[] bodies;

}
