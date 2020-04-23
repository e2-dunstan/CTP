#include "Primitive.h"
#include "Utilities.h"

/*bool RayCast::Test(Primitive* prim, Ray& ray)
{
	ray.direction = ray.direction.Normalise();

	switch (prim->type)
	{
	case PrimitiveType::PLANE:
	{
		Plane* p = dynamic_cast<Plane*>(prim);
		return TestPlane(p->translation, p->collisionVolume.normal, p->scale, ray);
	}
	case PrimitiveType::SPHERE:
	{
		Sphere* s = dynamic_cast<Sphere*>(prim);
		return TestSphere(s->translation, s->radius, ray);
	}
	case PrimitiveType::BOX:
	{
		//need full transform for box and the transform excluding scale
		Box* b = dynamic_cast<Box*>(prim);
		return TestBox(b->scale, b->collisionVolume.axisMat, ray);
	}
	default:
	{
		std::cout << "Warning: primitive type not supported in ray tests." << std::endl;
		return false;
	}
	}
}*/

bool RayCast::TestTriangle(const Tri& tri, Matrix4& transform, Ray& ray)
{
	// Möller–Trumbore intersection algorithm
	
	const float epsilon = 0.000001f;

	Vector3 v0 = tri.positions[0];
	Vector3 v1 = tri.positions[1];
	Vector3 v2 = tri.positions[2];
	Mathe::Transform(v0, transform);
	Mathe::Transform(v1, transform);
	Mathe::Transform(v2, transform);

	Vector3 edge1 = v1 - v0;
	Vector3 edge2 = v2 - v0;
	Vector3 h = ray.direction.VectorProduct(edge2);
	float a = edge1.ScalarProduct(h);

	if (a > -epsilon && a < epsilon) return false; //ray parallel

	float f = 1.0f / a;
	Vector3 s = ray.origin - v0;
	float u = f * s.ScalarProduct(h);

	if (u < 0.0f || u > 1.0f) return false;

	Vector3 q = s.VectorProduct(edge1);
	float v = f * ray.direction.ScalarProduct(q);

	if (v < 0.0f || u + v > 1.0f) return false;

	float t = f * edge2.ScalarProduct(q);
	if (t > epsilon) //ray intersection
	{
		ray.intersection1 = t;
		return true;
	}
	else return false; //line intersection but not ray
}

bool RayCast::TestPlane(const Vector3& centre, const Vector3& normal, const Vector3& size, Ray& ray)
{
	//Vector3 B = ray.origin + ray.direction * maxRayLength;
	//Vector3 lineAB = B - ray.origin;
	//Simplified: 
	Vector3 lineAB = ray.direction * maxRayLength;
	double planePos = centre.ScalarProduct(normal);

	double divisor = normal.ScalarProduct(lineAB);
	if (divisor == 0) return false;
	
	double distAlongLine = (planePos - normal.ScalarProduct(ray.origin)) / divisor;
	if (distAlongLine >= 0 && distAlongLine <= 1)
	{
		//ray.origin + (intersection * direction) = ray.origin + (distAlongLine * lineAB)
		//intersection * direction = distAlongLine * lineAB
		//intersection * direction = distAlongLine * direction * maxRayLength, therefore...
		ray.intersection1 = (float)distAlongLine * maxRayLength;
		return true;
	}
	else return false;
}

bool RayCast::TestSphere(const Vector3& centre, const float radius, Ray& ray)
{
	Vector3 m = ray.origin - centre;
	float b = 2.0f * (float)(m.ScalarProduct(ray.direction));
	float c = (float)m.ScalarProduct(m) - (radius * radius);
	
	if (c > 0 && b > 0) return false;

	float discriminant = (b * b) - (4.0f * c); //2b - 4ac

	if (discriminant < 0) return false; //no intersection
	else if (discriminant == 0) //one intersection
	{
		std::array<float, 2> i = Mathe::SolveQuadraticFormula(1.0f, b, c, false);
		ray.intersection1 = i[0];
		return true;
	}
	else //two intersections
	{
		std::array<float, 2> i = Mathe::SolveQuadraticFormula(1.0f, b, c, true);
		ray.intersection1 = i[0];
		ray.intersection2 = i[1];
		return true;
	}
}

//Similar to SAT
bool RayCast::TestBox(const Vector3& scale, const Matrix4& transform, Ray& ray)
{
	Matrix4 inverse = transform;
	//inverse = inverse.Transpose();
	inverse.Inverse();

	//Get ray relative to box
	Vector3 rayOriginAA = ray.origin;
	Mathe::Transform(rayOriginAA, inverse);
	Vector3 rayDirectionAA = ray.direction;
	//Mathe::Transform(rayDirectionAA, inverse);
	rayDirectionAA = rayDirectionAA.Normalise();
	//Vector3 rayInverseDirectionAA = rayDirectionAA.Inverse();

	/*
	//Vector3 boxMinMax[2] = { scale * -1.0, scale };

	float tMin = (boxMinMax[ray.sign[0]].x - rayOriginAA.x) * rayInverseDirectionAA.x;
	float tMax = (boxMinMax[1 - ray.sign[0]].x - rayOriginAA.x) * rayInverseDirectionAA.x;
	float tyMin = (boxMinMax[ray.sign[1]].y - rayOriginAA.y) * rayInverseDirectionAA.y;
	float tyMax = (boxMinMax[1 - ray.sign[1]].y - rayOriginAA.y) * rayInverseDirectionAA.y;
	
	//float temp = tMin;
	//if (tMin > tMax)
	//{
	//	tMin = tMax;
	//	tMax = temp;
	//}

	if (tMin > tyMax || tyMin > tMax) return false;
	if (tyMin > tMin) tMin = tyMin;
	if (tyMax < tMax) tMax = tyMax;

	float tzMin = (boxMinMax[ray.sign[2]].z - rayOriginAA.z) * rayInverseDirectionAA.z;
	float tzMax = (boxMinMax[1 - ray.sign[2]].z - rayOriginAA.z) * rayInverseDirectionAA.z;

	if (tMin > tzMax || tzMin > tMax) return false;
	if (tzMin > tMin) tMin = tzMin;
	if (tzMax < tMax) tMax = tzMax;

	if (tMin < 0)
	{
		tMin = tMax;
		if (tMin < 0) return false;
	}

	ray.intersection1 = tMin;
	ray.intersection2 = tMax;

	return true;
	*/
	
	float tMin = -maxRayLength;
	float tMax = maxRayLength;
	Vector3 boxMax = scale;// / 2.0f;
	Vector3 boxMin = boxMax * -1.0f;
	
	//3 slabs
	for (int i = 0; i < 3; i++)
	{
		if (abs(rayDirectionAA[i]) < 0.001f) //parallel to slab
		{
			//not in slab, can't hit
			if (rayOriginAA[i] < boxMin[i] || rayOriginAA[i] > boxMax[i]) return false;
		}
		else
		{
			float intersectionMin = (boxMin[i] - rayOriginAA[i]) / rayDirectionAA[i];
			float intersectionMax = (boxMax[i] - rayOriginAA[i]) / rayDirectionAA[i];

			//intersection min = near plane, max = far plane
			if (intersectionMin > intersectionMax)
			{
				float temp = intersectionMin;
				intersectionMin = intersectionMax;
				intersectionMax = temp;
			}

			tMin = (tMin > intersectionMin) ? tMin : intersectionMin;
			tMax = (tMax < intersectionMax) ? tMax : intersectionMax;

			if (tMin > tMax && (tMin >= 0 || tMax >= 0))		
				return false;
			
		}
	}

	ray.intersection1 = abs(tMin);
	ray.intersection2 = abs(tMax); //if ray goes through obj

	//Convert back to world space
	//Vector3 localIntersection1 = rayOriginAA + (rayDirectionAA.Normalise() * tMin);
	//Vector3 localIntersection2 = rayOriginAA + (rayDirectionAA.Normalise() * tMax);
	//Matrix trans = transform;
	//Mathe::Transform(localIntersection1, trans);
	//Mathe::Transform(localIntersection2, trans);
	////localIntersection = ray.origin - ray.direction * x
	////x = (ray.origin - localIntersection) / ray.direction
	//ray.intersection1 = 1 / (ray.origin - localIntersection1).ScalarProduct(ray.direction);
	//ray.intersection2 = 1 / (ray.origin - localIntersection2).ScalarProduct(ray.direction);

	return true;
}

Vector3 Ray::IntersectionPoint()
{
	direction = direction.Normalise();

	if (intersection1 == -1) return origin + (direction * 100.0f);

	Vector3 pointA = origin + (direction * intersection1);
	if (intersection2 == -1) return pointA;

	Vector3 pointB = origin + (direction * intersection2);
	return (pointA.Magnitude() < pointB.Magnitude()) ? pointA : pointB;
}

void Ray::Draw(const Colour& colour)
{
	glBegin(GL_LINES);

	glColor3f(colour.r, colour.g, colour.b);
	glVertex3f(origin.x, origin.y, origin.z);
	Vector3 point2 = IntersectionPoint();
	glVertex3f(point2.x, point2.y, point2.z);

	glEnd();
}

void Ray::DebugOutput()
{
	std::string str = "Ray origin: (" + 
		std::to_string(origin.x) + ", " + std::to_string(origin.y) + ", " + std::to_string(origin.z) + ") "
		+ "direction: " + 
		std::to_string(direction.x) + ", " + std::to_string(direction.y) + ", " + std::to_string(direction.z) + ") ";
	std::cout << str << std::endl;
}
