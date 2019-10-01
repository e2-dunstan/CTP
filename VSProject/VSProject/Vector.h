#pragma once

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


	double Magnitude();
	Vector3 Normalised();
	Vector3 Inverse();

	Vector3 ComponentProduct(const Vector3& vec) const;
	double ScalarProduct(const Vector3& vec) const;
	Vector3 VectorProduct(const Vector3& vec) const;

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
};