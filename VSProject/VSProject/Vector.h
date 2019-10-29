#pragma once
#include <iostream>
#include <string>

class Vector3
{
public:
	//Defaults to (0,0,0)
	Vector3() : x(0), y(0), z(0) {}
	Vector3(double _x, double _y, double _z)
		: x(_x), y(_y), z(_z) {}

	~Vector3() = default;

	double x;
	double y;
	double z;


	double Magnitude()
	{
		return sqrt((x * x) + (y * y) + (z * z));
	}
	Vector3 Normalise()
	{
		double mag = Magnitude();
		return Vector3(x / mag, y / mag, z / mag);
	}
	Vector3 Inverse()
	{
		return Vector3(-x, -y, -z);
	}

	Vector3 ComponentProduct(const Vector3& vec) const
	{
		return Vector3(x * vec.x, y * vec.y, z * vec.z);
	}
	double ScalarProduct(const Vector3& vec) const
	{
		return (x * vec.x) + (y * vec.y) + (z * vec.z);
	}
	Vector3 VectorProduct(const Vector3& vec) const
	{
		return Vector3((y * vec.z) - (z * vec.y),
			(z * vec.x) - (x * vec.z),
			(x * vec.y) - (y * vec.x));
	}

	// -- SCALAR -- //
	void operator*=(const float value)
	{
		x *= value;
		y *= value;
		z *= value;
	}
	Vector3 operator*(const float value) const
	{
		return Vector3(x * value, y * value, z * value);
	}
	Vector3 operator/(const float value) const
	{
		return Vector3(x / value, y / value, z / value);
	}

	
	// -- ADDITION -- //
	void operator+=(const Vector3& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
	}
	Vector3 operator+(const Vector3& vec) const
	{
		return Vector3(x + vec.x, y + vec.y, z + vec.z);
	}


	// -- SUBTRACTION -- //
	void operator-=(const Vector3& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
	}
	Vector3 operator-(const Vector3& vec) const
	{
		return Vector3(x - vec.x, y - vec.y, z - vec.z);
	}


	// -- VECTOR PRODUCT -- //
	void operator%=(const Vector3& vec)
	{
		*this = VectorProduct(vec);
	}
	Vector3 operator%(const Vector3& vec) const
	{
		return VectorProduct(vec);
	}

	// -- COMPARISON -- //
	bool operator==(const Vector3& vec)
	{
		if (x == vec.x && y == vec.y && z == vec.z)
			return true;
		else
			return false;
	}
	bool operator!=(const Vector3& vec)
	{
		if (x != vec.x || y != vec.y || z != vec.z)
			return true;
		else
			return false;
	}

	void DebugOutput() { std::cout << "( " + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + " )" << std::endl; }
};