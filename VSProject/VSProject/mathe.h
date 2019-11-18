#pragma once
#include <math.h>
#include "Vector.h"
#include "Matrix.h"

//Custom maths library by Ellie Dunstan
class Mathe
{
public:
	Mathe() = default;
	~Mathe() = default;

	//Functions for matrix and vector math, mostly transformations.

	void Transform(Vector3& vector, Matrix& matrix);

	Vector3 matrixInverse(Matrix& m, Vector3& v);

	void Translate(Matrix& m, double x, double y, double z);
	void Rotate(Matrix& m, double x, double y, double z);
	void Scale (Matrix& m, double x, double y, double z);

	Vector3 GetAxis(unsigned col, Matrix mat);
};