#pragma once
#include <cstdint>

class MatrixHelper
{
	friend struct Matrix3;
	friend struct Matrix4;

public:
	MatrixHelper() = default;
	~MatrixHelper() = default;

	static void Identity(const uint16_t colSize, const uint16_t rowSize, double* arr);
};

struct Matrix3
{
	Matrix3(double matrixValues[9]);
	Matrix3() = default;
	~Matrix3() = default;

	double matrix[9] = { 1.0, 0.0, 0.0,
						0.0, 1.0, 0.0,
						0.0, 0.0, 1.0 };
	uint16_t rowSize = 3;
	uint16_t colSize = 3;

	void Identity() { MatrixHelper::Identity(colSize, rowSize, matrix); }
	void Transpose() { *this = GetTranspose(); }
	Matrix3 GetTranspose();
	void Inverse();

	double& operator()(const uint16_t row, const uint16_t column)
	{
		return this->matrix[(row * colSize) + column];
	}

	Matrix3 operator*(Matrix3& m);
	Matrix3 operator*(double scalar);

	Matrix3 operator+(Matrix3& m);
		
	void DebugOutput();
};

struct Matrix4
{
public:
	Matrix4(double matrixValues[16]);
	Matrix4() = default;
	~Matrix4() = default;

	double matrix[16] = { 1.0, 0.0, 0.0, 0.0,
						  0.0, 1.0, 0.0, 0.0,
						  0.0, 0.0, 1.0, 0.0,
						  0.0, 0.0, 0.0, 1.0 };
	uint16_t rowSize = 4;
	uint16_t colSize = 4;

	void Identity() { MatrixHelper::Identity(colSize, rowSize, matrix); }
	void Transpose() { *this = GetTranspose(); }
	Matrix4 GetTranspose();
	void Inverse();

	Matrix3 ToMatrix3();

	double& operator()(const uint16_t row, const uint16_t column)
	{
		return this->matrix[(row * colSize) + column];
	}

	Matrix4 operator*(Matrix4& m);
	Matrix4 operator*(double scalar);

	Matrix4 operator+(Matrix4& m);

	void DebugOutput();
};