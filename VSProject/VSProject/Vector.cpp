#include "Vector.h"

float Vector3::Distance(const Vector3& vec) const
{
	Vector3 dir = Vector3(vec.x - x, vec.y - y, vec.z - z);
	return dir.Magnitude();
}

float Vector3::SquaredMagnitude()
{
	return (x * x) + (y * y) + (z * z);
}

float Vector3::Magnitude()
{
	return sqrt((x * x) + (y * y) + (z * z));
}

Vector3 Vector3::Normalise()
{
	float mag = Magnitude();
	Vector3 norm = Vector3(x / mag, y / mag, z / mag);
	//if (abs(norm.x) < 0.0001) norm.x = 0;
	//if (abs(norm.y) < 0.0001) norm.y = 0;
	//if (abs(norm.z) < 0.0001) norm.z = 0;
	return norm;
}

Vector3 Vector3::Inverse()
{
	return Vector3(x * -1.0f, y * -1.0f, z * -1.0f);
}

Vector3 Vector3::Clamp(float minMag, float maxMag)
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

float Vector3::ScalarProduct(const Vector3& vec) const
{
	return (x * vec.x) + (y * vec.y) + (z * vec.z);
}

Vector3 Vector3::VectorProduct(const Vector3& vec) const
{
	float _x = (y * vec.z) - (z * vec.y);
	float _y = (z * vec.x) - (x * vec.z);
	float _z = (x * vec.y) - (y * vec.x);

	return Vector3(_x, _y, _z);
}

float Vector3::SumComponents() const
{
	return x + y + z;
}

float& Vector3::operator[](const int i)
{
	if (i < 0 || i > 3) std::cout << "ERROR: index " << i << " does not exist in Vector3" << std::endl;

	if (i == 0) return x;
	if (i == 1) return y;
	if (i == 2) return z;
	else return x;
}

const float& Vector3::operator[](const int i) const
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

Vector3 Vector3::operator/(const Vector3& vec) const
{
	Vector3 ret = Vector3(x, y, z);
	if (vec.x != 0) ret.x /= vec.x;
	if (vec.y != 0) ret.y /= vec.y;
	if (vec.z != 0) ret.x /= vec.z;
	return ret;
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
	const double threshold = 0.001f;
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
