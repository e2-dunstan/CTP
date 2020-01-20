#pragma once

struct Vector3;

struct Quaternion
{
	Quaternion(double _r, double _i, double _j, double _k)
		: r(_r), i(_i), j(_j), k(_k) {}
	Quaternion() = default;
	~Quaternion() = default;

	//real and three complex components
	double r = 1;
	double i = 0;
	double j = 0;
	double k = 0;

	void Normalise();

	void operator*=(const Quaternion& m);
	Quaternion operator*(const Quaternion& m);
	
	//void AddScaledVector(const Vector3& v, double scale);
	void RotateByVector(const Vector3& v);
};