#pragma once
#include <iostream>
#include <string>

struct Vector3
{
	//Defaults to (0,0,0)
	Vector3() : x(0), y(0), z(0) {}
	Vector3(float _x, float _y, float _z)
		: x(_x), y(_y), z(_z) {}

	~Vector3() = default;

	float x;
	float y;
	float z;

	float Distance(const Vector3& vec) const;

	float SquaredMagnitude();
	float Magnitude();
	Vector3 Normalise();
	Vector3 Inverse();

	Vector3 Clamp(float minMag, float maxMag);

	Vector3 ComponentProduct(const Vector3& vec) const;
	float ScalarProduct(const Vector3& vec) const;
	Vector3 VectorProduct(const Vector3& vec) const;

	float SumComponents() const;

	// -- INDEX GETTER/SETTER -- //
	float& operator[](const int i);
	const float& operator[](const int i) const;

	// -- SCALAR -- //
	void operator*=(const float value);
	void operator*=(const Vector3& vec);

	Vector3 operator*(const float value) const;
	Vector3 operator*(const Vector3& vec) const;

	void operator/=(const float value);
	Vector3 operator/(const float value) const;
	Vector3 operator/(const Vector3& vec) const;

	// -- ADDITION -- //
	void operator+=(const Vector3& vec);
	Vector3 operator+(const Vector3& vec) const;

	// -- SUBTRACTION -- //
	void operator-=(const Vector3& vec);
	Vector3 operator-(const Vector3& vec) const;

	// -- VECTOR/CROSS PRODUCT -- //
	void operator%=(const Vector3& vec);
	Vector3 operator%(const Vector3& vec) const;

	// -- COMPARISON -- //
	bool operator==(const Vector3& vec);
	bool operator!=(const Vector3& vec);
	bool IsRoughlyEqualTo(const Vector3& vec);

	void DebugOutput();
};