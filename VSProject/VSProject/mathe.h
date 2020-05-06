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

	void Translate(Matrix4& m, float x, float y, float z);

	void Rotate(Matrix4& m, float x, float y, float z);
	void Rotate(Matrix4& m, Quaternion& q);
	void Rotate(Matrix3& m, Quaternion& q);

	void Scale (Matrix4& m, const float x, const float y, const float z);

	Vector3 GetAxis(const uint16_t col, Matrix4& matrix);
	Vector3 GetAxis(const uint16_t col, Matrix3& matrix);

	Quaternion VectorToQuaternion(const Vector3& v, bool toRadians = true);
	void AddScaledVector(Quaternion& q, const Vector3& v, const float scale, const bool toRadians);

	void TransformInverseInertiaTensor(Matrix3& tensorWorld, const Matrix3& tensorLocal, const Matrix3& rot);

	struct QuadraticFormulaResult
	{
		QuadraticFormulaResult(const float _one, const float _two) : one(_one), two(_two) {}
		float one = 0;
		float two = 0;
	};
	QuadraticFormulaResult SolveQuadraticFormula(const float a, const float b, const float c, const bool twoRealRoots = true);

	float ToRadians(const float deg);
	Vector3 ToRadians(const Vector3& vec);
	float ToDegrees(const float rad);

	bool IsVectorNAN(const Vector3& v);

	double ClampDouble(const double value, const double min, const double max);
	float ClampFloat(const float value, const float min, const float max);

	const float PI = 3.1415927410125732421875f;

};