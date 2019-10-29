#pragma once
#include <math.h>
#include "Vector.h"
#include "Matrix.h"

class Mathe
{
public:
	Mathe() = default;
	~Mathe() = default;

	void Transform(Vector3& vector, Matrix& matrix);

	void Translate(Matrix& m, double x, double y, double z);
	void Rotate(Matrix& m, double x, double y, double z);
	void Scale (Matrix& m, double x, double y, double z);
};