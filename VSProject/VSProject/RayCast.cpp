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
bool RayCast::TestBox(const Vector3& scale, const Matrix& transform, Ray& ray)
{
	Matrix inverse = transform;
	//inverse = inverse.Transpose();
	inverse.Inverse4x4();

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
