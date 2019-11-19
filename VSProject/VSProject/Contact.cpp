#include "Contact.h"


Contact::Contact(Primitive* prim1, Primitive* prim2)
{
	body1 = prim1;
	body2 = prim2;
}

Contact::~Contact()
{
	//delete body1;
	//delete body2;
}
