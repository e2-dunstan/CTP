#include "Quaternion.h"
#include "Vector.h"
#include <math.h>

void Quaternion::Normalise()
{
	float length = r * r + i * i + j * j + k * k;

	if (length == 0)
	{
		r = 1;
		return;
	}

	length = 1.0f / sqrtf(length);
	r *= length;
	i *= length;
	j *= length;
	k *= length;
}

void Quaternion::operator*=(const Quaternion& m)
{
	Quaternion q = *this;
	r = q.r * m.r - q.i * m.i - q.j * m.j - q.k * m.k;
	i = q.r * m.i + q.i * m.r + q.j * m.k - q.k * m.j;
	j = q.r * m.j + q.j * m.r + q.k * m.i - q.i * m.k;
	k = q.r * m.k + q.k * m.r + q.i * m.j - q.j * m.i;
}

Quaternion Quaternion::operator*(const Quaternion& m)
{
	Quaternion result;
	result.r = this->r * m.r - this->i * m.i - this->j * m.j - this->k * m.k;
	result.i = this->r * m.i + this->i * m.r + this->j * m.k - this->k * m.j;
	result.j = this->r * m.j + this->j * m.r + this->k * m.i - this->i * m.k;
	result.k = this->r * m.k + this->k * m.r + this->i * m.j - this->j * m.i;
	return result;
}

void Quaternion::RotateByVector(const Vector3& v)
{
	Quaternion q(0, v.x, v.y, v.z);
	(*this) *= q;

}

void Quaternion::DebugOutput()
{
	std::cout << "( " + std::to_string(r) + ", " + std::to_string(i) + "i, " + std::to_string(j) + "j, " + std::to_string(k) + " k)" << std::endl;
}
