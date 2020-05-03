#pragma once

struct Vector3;

struct Quaternion
{
	Quaternion(float _r, float _i, float _j, float _k)
		: r(_r), i(_i), j(_j), k(_k) {}
	Quaternion() = default;
	~Quaternion() = default;

	//real and three complex components
	float r = 1;
	float i = 0;
	float j = 0;
	float k = 0;

	void Normalise();

	void operator*=(const Quaternion& m);
	Quaternion operator*(const Quaternion& m);
	
	void RotateByVector(const Vector3& v);

	bool NoRotation() { return i == 0 && j == 0 && k == 0; }

	void DebugOutput();
};