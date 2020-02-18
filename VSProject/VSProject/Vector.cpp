#include "Vector.h"

double Vector3::Distance(const Vector3& vec) const
{
	Vector3 dir = Vector3(vec.x - x, vec.y - y, vec.z - z);
	return dir.Magnitude();
}

double Vector3::SquaredMagnitude()
{
	return (x * x) + (y * y) + (z * z);
}

double Vector3::Magnitude()
{
	return sqrt((x * x) + (y * y) + (z * z));
}

Vector3 Vector3::Normalise()
{
	double mag = Magnitude();
	Vector3 norm = Vector3(x / mag, y / mag, z / mag);
	if (abs(norm.x) < 0.0001) norm.x = 0;
	if (abs(norm.y) < 0.0001) norm.y = 0;
	if (abs(norm.z) < 0.0001) norm.z = 0;
	return norm;
}

Vector3 Vector3::Inverse()
{
	return Vector3(-x, -y, -z);
}

Vector3 Vector3::Clamp(double minMag, double maxMag)
{
	if (this->Magnitude() > maxMag)
	{
		return this->Normalise() * maxMag;
	}
	else if (this->Magnitude() < minMag)
	{
		return this->Normalise() * minMag;
	}
	else
	{
		return *this;
	}
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
	double _x = (y * vec.z) - (z * vec.y);
	double _y = (z * vec.x) - (x * vec.z);
	double _z = (x * vec.y) - (y * vec.x);

	return Vector3(_x, _y, _z);
}

double Vector3::SumComponents() const
{
	return x + y + z;
}

double& Vector3::operator[](const int i)
{
	if (i < 0 || i > 3) std::cout << "ERROR: index " << i << " does not exist in Vector3" << std::endl;

	if (i == 0) return x;
	if (i == 1) return y;
	if (i == 2) return z;
	else return x;
}

const double& Vector3::operator[](const int i) const
{
	if (i < 0 || i > 3) std::cout << "ERROR: index " << i << " does not exist in Vector3" << std::endl;

	if (i == 0) return x;
	if (i == 1) return y;
	if (i == 2) return z;
	else return x;
}

void Vector3::operator*=(const float value)
{
	x *= value;
	y *= value;
	z *= value;
}

void Vector3::operator*=(const double value)
{
	x *= value;
	y *= value;
	z *= value;
}

void Vector3::operator*=(const Vector3& vec)
{
	x *= vec.x;
	y *= vec.y;
	z *= vec.z;
}

Vector3 Vector3::operator*(const float value) const
{
	return Vector3(x * value, y * value, z * value);
}

Vector3 Vector3::operator*(const double value) const
{
	return Vector3(x * value, y * value, z * value);
}

Vector3 Vector3::operator*(const Vector3& vec) const
{
	return Vector3(x * vec.x, y * vec.y, z * vec.z);
}

void Vector3::operator/=(const float value)
{
	x /= value;
	y /= value;
	z /= value;
}

Vector3 Vector3::operator/(const float value) const
{
	return Vector3(x / value, y / value, z / value);
}

void Vector3::operator+=(const Vector3& vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
}
Vector3 Vector3::operator+(const Vector3& vec) const
{
	return Vector3(x + vec.x, y + vec.y, z + vec.z);
}

void Vector3::operator-=(const Vector3& vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
}
Vector3 Vector3::operator-(const Vector3& vec) const
{
	return Vector3(x - vec.x, y - vec.y, z - vec.z);
}

void Vector3::operator%=(const Vector3& vec)
{
	*this = VectorProduct(vec);
}
Vector3 Vector3::operator%(const Vector3& vec) const
{
	return VectorProduct(vec);
}

bool Vector3::operator==(const Vector3& vec)
{
	if (x == vec.x && y == vec.y && z == vec.z)
		return true;
	else
		return false;
}
bool Vector3::operator!=(const Vector3& vec)
{
	if (x != vec.x || y != vec.y || z != vec.z)
		return true;
	else
		return false;
}

bool Vector3::IsRoughlyEqualTo(const Vector3& vec)
{
	const double threshold = 0.01f;
	if (abs(x - vec.x) < threshold
		&& abs(y - vec.y) < threshold
		&& abs(z - vec.z) < threshold)
		return true;
	else
		return false;
}


void Vector3::DebugOutput() 
{ 
	std::cout << "( " + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + " )" << std::endl; 
}
