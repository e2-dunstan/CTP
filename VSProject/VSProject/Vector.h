#pragma once
#include <iostream>
#include <string>

struct Vector3
{
	//Defaults to (0,0,0)
	Vector3() : x(0), y(0), z(0) {}
	Vector3(double _x, double _y, double _z)
		: x(_x), y(_y), z(_z) {}

	~Vector3() = default;

	double x;
	double y;
	double z;

	double Distance(const Vector3& vec) const;

	double Magnitude();
	Vector3 Normalise();
	Vector3 Inverse();

	Vector3 ComponentProduct(const Vector3& vec) const;
	double ScalarProduct(const Vector3& vec) const;
	Vector3 VectorProduct(const Vector3& vec) const;

	// -- SCALAR -- //
	void operator*=(const float value);
	void operator*=(const Vector3& vec);
	Vector3 operator*(const float value) const;
	Vector3 operator*(const Vector3& vec) const;
	void operator/=(const float value);
	Vector3 operator/(const float value) const;

	
	// -- ADDITION -- //
	void operator+=(const Vector3& vec);
	Vector3 operator+(const Vector3& vec) const;


	// -- SUBTRACTION -- //
	void operator-=(const Vector3& vec);
	Vector3 operator-(const Vector3& vec) const;


	// -- VECTOR PRODUCT -- //
	void operator%=(const Vector3& vec);
	Vector3 operator%(const Vector3& vec) const;

	// -- COMPARISON -- //
	bool operator==(const Vector3& vec);
	bool operator!=(const Vector3& vec);

	void DebugOutput();
};