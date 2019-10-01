#include "Vector.h"
#include <math.h>


Vector3 Vector3::Normalised()
{
	float mag = Magnitude();
	return Vector3(x / mag, y / mag, z / mag);
}

double Vector3::Magnitude()
{
	return sqrt((x * x) + (y * y) + (z * z));

}

Vector3 Vector3::Inverse()
{
	return Vector3(-x, -y, -z);
}

Vector3 Vector3::ComponentProduct(const Vector3& vec) const
{
	return Vector3(x * vec.x, y * vec.y, z * vec.z);
}

double Vector3::ScalarProduct(const Vector3& vec) const
{
	return (x * vec.x) + (y * vec.y) + (z * vec.z);
}

Vector3 Vector3::VectorProduct(const Vector3& vec) const
{
	return Vector3((y * vec.z) - (z * vec.y),
					(z * vec.x) - (x * vec.z),
					(x * vec.y) - (y * vec.x));
}
