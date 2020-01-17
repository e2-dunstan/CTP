#pragma once
#include <math.h>
#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"

//Custom maths library by Ellie Dunstan
namespace Mathe
{
	//Functions for matrix4x4 and vector math, mostly transformations.

	void Transform(Vector3& vector, Matrix& matrix4x4);

	Vector3 matrixInverse(Matrix& m, Vector3& v);

	void Translate(Matrix& m, double x, double y, double z);
	void Rotate(Matrix& m, double x, double y, double z);
	void Rotate(Matrix& m, const Quaternion& q);
	void Scale (Matrix& m, double x, double y, double z);

	Vector3 GetAxis(unsigned col, Matrix mat);

	Quaternion VectorToQuaternion(const Vector3& v);
	void AddScaledVector(Quaternion& q, const Vector3& v, double scale);

	void TransformInverseInertiaTensor(Matrix& tensorWorld, const Matrix& tensorLocal, const Matrix& rot);
};