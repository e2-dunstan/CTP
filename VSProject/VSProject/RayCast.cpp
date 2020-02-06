#include "RayCast.h"
#include "Primitive.h"

bool RayCast::Test(const Primitive& prim, Ray& ray)
{
	ray.direction = ray.direction.Normalise();

	switch (prim.type)
	{
	case Primitive::Type::PLANE:
		return TestPlane(prim.translation, prim.collisionVolume.normal, prim.scale, ray);

	case Primitive::Type::SPHERE:
		return TestSphere(prim.translation, prim.radius, ray);

	case Primitive::Type::BOX:
		//need full transform for box and the transform excluding scale
		return TestBox(prim.scale, prim.collisionVolume.axisMat, ray);

	default:
		std::cout << "Warning: primitive type not supported in ray tests." << std::endl;
		return false;
	}
}

bool RayCast::TestPlane(const Vector3& centre, const Vector3& normal, const Vector3& size, Ray& ray)
{
	//Vector3 B = ray.origin + ray.direction * maxRayLength;
	//Vector3 lineAB = B - ray.origin;
	//Simplified: 
	Vector3 lineAB = ray.direction * maxRayLength;
	float planePos = centre.ScalarProduct(normal);

	float divisor = normal.ScalarProduct(lineAB);
	if (divisor == 0) return false;
	
	float distAlongLine = (planePos - normal.ScalarProduct(ray.origin)) / divisor;
	if (distAlongLine >= 0 && distAlongLine <= 1)
	{
		//ray.origin + (intersection * direction) = ray.origin + (distAlongLine * lineAB)
		//intersection * direction = distAlongLine * lineAB
		//intersection * direction = distAlongLine * direction * maxRayLength, therefore...
		ray.intersection1 = distAlongLine * maxRayLength;
		return true;
	}
	else return false;
}

bool RayCast::TestSphere(const Vector3& centre, const float radius, Ray& ray)
{
	Vector3 m = ray.origin - centre;
	float b = 2 * (m.ScalarProduct(ray.direction));
	float c = (float)m.ScalarProduct(m) - (radius * radius);
	
	if (c > 0 && b > 0) return false;

	float discriminant = (b * b) - (4 * c); //2b - 4ac

	if (discriminant < 0) return false; //no intersection
	else if (discriminant == 0) //one intersection
	{
		std::array<float, 2> i = Mathe::SolveQuadraticFormula(1, b, c, false);
		ray.intersection1 = i[0];
		return true;
	}
	else //two intersections
	{
		std::array<float, 2> i = Mathe::SolveQuadraticFormula(1, b, c, true);
		ray.intersection1 = i[0];
		ray.intersection2 = i[1];
		return true;
	}
}

//Similar to SAT
bool RayCast::TestBox(const Vector3& scale, const Matrix& transform, Ray& ray)
{
	Matrix inverse = transform;
	inverse = inverse.Transpose();
	inverse.Inverse3x3();

	//Get ray relative to box
	Vector3 rayOriginAA = ray.origin;
	Mathe::Transform(rayOriginAA, inverse);
	Vector3 rayDirectionAA = ray.direction;
	Mathe::Transform(rayDirectionAA, inverse);
	rayDirectionAA = rayDirectionAA.Normalise();

	float tMin = 0.0f;
	float tMax = maxRayLength;
	Vector3 boxMax = scale;// / 2.0f;
	Vector3 boxMin = boxMax * -1.0f;

	//3 slabs
	for (int i = 0; i < 3; i++)
	{
		if (abs(rayDirectionAA[i]) < 0.00000001f) //parallel to slab
		{
			//not in slab, can't hit
			if (rayOriginAA[i] < boxMin[i] || rayOriginAA[i] > boxMax[i]) return false;
		}
		else
		{
			float inverseDirection = 1.0f / rayDirectionAA[i];
			float intersectionMin = abs((boxMin[i] - rayOriginAA[i]) * inverseDirection);
			float intersectionMax = abs((boxMax[i] - rayOriginAA[i]) * inverseDirection);

			//intersection min = near plane, max = far plane
			if (intersectionMin > intersectionMax)
			{
				float temp = intersectionMin;
				intersectionMin = intersectionMax;
				intersectionMax = temp;
			}

			tMin = (tMin > intersectionMin) ? tMin : intersectionMin;
			tMax = (tMax < intersectionMax) ? tMax : intersectionMax;

			if (tMin > tMax) return false;
		}
	}

	ray.intersection1 = tMin;
	ray.intersection2 = tMax; //if ray goes through obj

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
