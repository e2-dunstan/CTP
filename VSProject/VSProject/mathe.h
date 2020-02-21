#pragma once
#include <math.h>
#include <array>
#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"

//Custom maths library by Ellie Dunstan
namespace Mathe
{
	//Functions for matrix and vector math, mostly transformations.

	void Transform(Vector3& vector, Matrix& matrix);
	void TransformTranspose(Vector3& vector, Matrix& matrix);

	Vector3 MatrixInverse(Matrix& m, Vector3& v); //don't use this

	void Translate(Matrix& m, double x, double y, double z);

	void Rotate(Matrix& m, double x, double y, double z);
	void Rotate(Matrix& m, const Quaternion& q);

	void Scale (Matrix& m, double x, double y, double z);

	Vector3 GetAxis(unsigned col, Matrix& mat);

	Quaternion VectorToQuaternion(const Vector3& v);
	void AddScaledVector(Quaternion& q, const Vector3& v, double scale);

	void TransformInverseInertiaTensor(Matrix& tensorWorld, const Matrix& tensorLocal, const Matrix& rot);

	std::array<float, 2> SolveQuadraticFormula(float a, float b, float c, bool twoRealRoots = true);

	double ToRadians(const double deg);
	double ToDegrees(const double rad);

	bool IsVectorNAN(const Vector3& v);
};