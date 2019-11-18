#include "CollisionFine.h"


CollisionFine::~CollisionFine()
{
	for (auto contact : data->contacts)
	{
		delete contact;
	}
	data->contacts.clear();
}

void CollisionFine::DetectContacts(Primitive* prim1, Primitive* prim2)
{
	data->contacts.clear();

	if (prim1->type == Primitive::Type::SPHERE && prim2->type == Primitive::Type::SPHERE)
	{
		SphereAndSphere(prim1, prim2,
			prim1->collisionVolume.centre, prim1->collisionVolume.radius,
			prim2->collisionVolume.centre, prim2->collisionVolume.radius);
	}
	if (prim1->type == Primitive::Type::SPHERE && prim2->type == Primitive::Type::PLANE)
	{
		SphereAndPlane(prim1, prim2,
			prim1->collisionVolume.centre, prim1->collisionVolume.radius,
			prim2->collisionVolume.centre, prim2->collisionVolume.normal);
	}
	if (prim1->type == Primitive::Type::PLANE && prim2->type == Primitive::Type::SPHERE)
	{
		SphereAndPlane(prim2, prim1,
			prim2->collisionVolume.centre, prim2->collisionVolume.radius,
			prim1->collisionVolume.centre, prim1->collisionVolume.normal);
	}
	if (prim1->type == Primitive::Type::BOX && prim2->type == Primitive::Type::PLANE)
	{
		BoxAndPlane(prim1, prim2, prim2->collisionVolume.centre, prim2->collisionVolume.normal);
	}
	if (prim1->type == Primitive::Type::PLANE && prim2->type == Primitive::Type::BOX)
	{
		BoxAndPlane(prim2, prim1, prim1->collisionVolume.centre, prim1->collisionVolume.normal);
	}
	if (prim1->type == Primitive::Type::SPHERE && prim2->type == Primitive::Type::BOX)
	{
		SphereAndBox(prim1, prim2, prim1->collisionVolume.centre, prim1->collisionVolume.radius);
	}
	if (prim1->type == Primitive::Type::BOX && prim2->type == Primitive::Type::SPHERE)
	{
		SphereAndBox(prim2, prim1, prim2->collisionVolume.centre, prim2->collisionVolume.radius);
	}
	if (prim1->type == Primitive::Type::BOX && prim2->type == Primitive::Type::BOX)
	{
		BoxAndBox(prim1, prim2);
	}
}

void CollisionFine::SphereAndSphere(Primitive* prim1, Primitive* prim2, Vector3 position1, float radius1, Vector3 position2, float radius2)
{
	if (data->contactsLeft <= 0) return;

	Vector3 midline = position1 - position2;
	float size = midline.Magnitude();

	if (size <= 0.0f || size >= radius1 + radius2) return;

	Vector3 normal = midline * 1.0f / size;

	Contact* contact = new Contact(prim1, prim2);
	contact->normal = normal;
	contact->point = position1 + midline * 0.5f;
	contact->penetrationDepth = radius1 + radius2 - size;

	data->contacts.push_back(contact);

	outputStr += "SPHERE and SPHERE \t";
}

void CollisionFine::SphereAndPlane(Primitive* sphere, Primitive* plane, Vector3 spherePosition, float radius, Vector3 planePosition, Vector3 normal)
{
	if (data->contactsLeft <= 0) return;
	
	float distance = normal.ScalarProduct(spherePosition) - radius - normal.ScalarProduct(planePosition);

	if (distance >= 0) return;

	Contact* contact = new Contact(sphere, plane);
	contact->normal = normal;
	contact->penetrationDepth = -distance;
	contact->point = spherePosition - normal * (distance + radius);

	data->contacts.push_back(contact);

	outputStr += "SPHERE and PLANE \t";
}

void CollisionFine::SphereAndBox(Primitive* sphere, Primitive* box, Vector3 spherePosition, float radius)
{
	Vector3 halfSize = box->collisionVolume.halfSize;
	Vector3 relCentre = mathe->matrixInverse(box->collisionVolume.axisMat, spherePosition);

	double x = abs(relCentre.x) - radius;
	double y = abs(relCentre.y) - radius;
	double z = abs(relCentre.z) - radius;

	if (abs(relCentre.x) - radius > halfSize.x
		|| abs(relCentre.y) - radius > halfSize.y
		|| abs(relCentre.z) - radius > halfSize.z)
		return; //not in contact

	Vector3 closestPoint;
	float distance;

	distance = relCentre.x;
	if (distance > halfSize.x) distance = halfSize.x;
	if (distance < -halfSize.x) distance = -halfSize.x;
	closestPoint.x = distance;

	distance = relCentre.y;
	if (distance > halfSize.y) distance = halfSize.y;
	if (distance < -halfSize.y) distance = -halfSize.y;
	closestPoint.y = distance;

	distance = relCentre.z;
	if (distance > halfSize.z) distance = halfSize.z;
	if (distance < -halfSize.z) distance = -halfSize.z;
	closestPoint.z = distance;

	distance = (closestPoint - relCentre).Magnitude();
	if (distance > radius* radius) return;	//not in contact
	
	Contact* contact = new Contact(sphere, box);
	contact->normal = (spherePosition - closestPoint).Normalise();
	contact->point = closestPoint;
	contact->penetrationDepth = radius - sqrt(distance);

	data->contacts.push_back(contact);

	outputStr += "SPHERE and BOX \t";
}

float CollisionFine::PositionOnAxis(Primitive* box, Vector3 axis)
{
	return
		box->collisionVolume.halfSize.x * abs(axis.ScalarProduct(mathe->GetAxis(0, box->collisionVolume.axisMat)))
		+ box->collisionVolume.halfSize.y * abs(axis.ScalarProduct(mathe->GetAxis(1, box->collisionVolume.axisMat)))
		+ box->collisionVolume.halfSize.z * abs(axis.ScalarProduct(mathe->GetAxis(2, box->collisionVolume.axisMat)));
}

bool CollisionFine::BoxesOverlapOnAxis(Primitive* box1, Primitive* box2, Vector3 toCentre, Vector3 axis)
{
	float box1AxisPos = PositionOnAxis(box1, axis);
	float box2AxisPos = PositionOnAxis(box2, axis);

	float distance = abs(toCentre.ScalarProduct(axis));// -data->tolerance;

	return (distance <= box1AxisPos + box2AxisPos);
}

//Separating axis theorem
void CollisionFine::BoxAndBox(Primitive* box1, Primitive* box2)
{
	Vector3 toCentre = box2->collisionVolume.centre - box1->collisionVolume.centre;

	//Early-outs if any axes are found to be not overlapping.
	for (int n1 = 0; n1 < 3; n1++)
	{
		//Test primitive axes.
		if (!BoxesOverlapOnAxis(box1, box2, toCentre, mathe->GetAxis(n1, box1->collisionVolume.axisMat))
			|| !BoxesOverlapOnAxis(box1, box2, toCentre, mathe->GetAxis(n1, box2->collisionVolume.axisMat)))
		{
			return;
		}
		for (int n2 = 0; n2 < 3; n2++)
		{
			//Test cross products of primitives axes.
			Vector3 vecProd = mathe->GetAxis(n1, box1->collisionVolume.axisMat) % mathe->GetAxis(n2, box2->collisionVolume.axisMat);
			if (!BoxesOverlapOnAxis(box1, box2, toCentre, vecProd))
			{
				return;
			}
		}
	}
	//Debug
	outputStr += "BOX and BOX \t";
}

void CollisionFine::BoxAndPlane(Primitive* box, Primitive* plane, Vector3 planePosition, Vector3 normal)
{
	//Intersection test
	Vector3 boxCentre = box->collisionVolume.centre;
	int projectedRadius = (boxCentre.x * normal.x + boxCentre.y * normal.y + boxCentre.z * normal.z) * 10;
	//Work out how far the box is from the origin
	int boxDistance = ((normal * boxCentre).Magnitude() * 10) - projectedRadius;
	float planeOffset = planePosition.x * normal.x + planePosition.y * normal.y + planePosition.z * normal.z;

	if (((float)boxDistance / 10) > planeOffset) return;

	for (int v = 0; v < 8; v++)
	{
		//distance from vertex to plane
		float distance = (box->collisionVolume.vertices[v] * normal).Magnitude();

		if (distance <= planeOffset + data->tolerance)
		{
			Contact* contact = new Contact(box, plane);
			contact->point = normal;
			contact->point *= distance - (planePosition * normal).Magnitude();
			contact->point += box->collisionVolume.vertices[v];
			contact->normal = normal;
			contact->penetrationDepth = (planePosition * normal).Magnitude() - distance + data->tolerance;

			data->contacts.push_back(contact);

			outputStr += "BOX and PLANE \t";
		}
	}
}
