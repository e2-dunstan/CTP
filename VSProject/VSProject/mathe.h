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

	void Transform(Vector3& vector, Matrix4& matrix);
	void Transform(Vector3& vector, Matrix3& matrix);
	void TransformTranspose(Vector3& vector, Matrix4& matrix);
	void TransformTranspose(Vector3& vector, Matrix3& matrix);

	Vector3 MatrixInverse(Matrix4& m, Vector3& v); //don't use this

	void Translate(Matrix4& m, double x, double y, double z);

	void Rotate(Matrix4& m, double x, double y, double z);
	void Rotate(Matrix4& m, const Quaternion& q);
	void Rotate(Matrix3& m, const Quaternion& q);

	void Scale (Matrix4& m, double x, double y, double z);

	Vector3 GetAxis(uint16_t col, Matrix4& mat);
	Vector3 GetAxis(uint16_t col, Matrix3& mat);

	Quaternion VectorToQuaternion(const Vector3& v);
	void AddScaledVector(Quaternion& q, const Vector3& v, double scale, bool toRadians);

	void TransformInverseInertiaTensor(Matrix3& tensorWorld, const Matrix3& tensorLocal, const Matrix3& rot);

	std::array<float, 2> SolveQuadraticFormula(float a, float b, float c, bool twoRealRoots = true);

	double ToRadians(const double deg);
	double ToDegrees(const double rad);

	bool IsVectorNAN(const Vector3& v);
};