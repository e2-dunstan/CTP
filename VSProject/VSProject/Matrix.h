#pragma once
#include <cstdint>

/*struct Matrix
{
	Matrix();
	Matrix(double arr[]);
	Matrix(unsigned _row, unsigned _col);
	~Matrix() = default;

	//Matrix Resize();

	void Identity();
	Matrix Transpose();
	
	void Inverse3x3();
	void Inverse4x4();
	
	Matrix operator+(Matrix& m);
	Matrix operator+(double scalar);
	Matrix operator-(Matrix& m);
	Matrix operator-(double scalar);
	
	Matrix operator*(Matrix& m);
	Matrix operator*(double scalar);
	
	Matrix operator/(double scalar);

	double& operator()(const unsigned& _row, const unsigned& _col);
	double Get(int i) const;
	unsigned GetRows() const;
	unsigned GetColumns() const;

	void DebugOutput();

	unsigned rowSize = 4;
	unsigned colSize = 4;

	double matrix[16] = { 1.0, 0.0, 0.0, 0.0,
						  0.0, 1.0, 0.0, 0.0,
						  0.0, 0.0, 1.0, 0.0,
						  0.0, 0.0, 0.0, 1.0};
	//double* matrixPtr = nullptr;
};*/

class MatrixHelper
{
	friend struct Matrix3;
	friend struct Matrix4;

public:
	MatrixHelper() = default;
	~MatrixHelper() = default;

	//static Matrix3 Multiply3(double* mat1, uint16_t colSize1, uint16_t rowSize1, double* mat2, uint16_t colSize2, uint16_t rowSize2);
	//static Matrix4 Multiply4(double* mat1, uint16_t colSize1, uint16_t rowSize1, double* mat2, uint16_t colSize2, uint16_t rowSize2);

	//protected:
	static void Identity(const uint16_t colSize, const uint16_t rowSize, double* arr);
	static double* GetTranspose(const uint16_t colSize, const uint16_t rowSize, double* arr);
	static void Transpose(const uint16_t colSize, const uint16_t rowSize, double* arr)
	{
		arr = GetTranspose(colSize, rowSize, arr);
	}

	//static Matrix3 Convert(const Matrix4& m4);
	//static Matrix4 Convert(const Matrix3& m3);
};

struct Matrix3
{
	Matrix3(double matrixValues[9]);
	Matrix3() = default;
	~Matrix3() = default;

	//void operator=(const Matrix3& m) { *this = m; };

	double matrix[9] = { 1.0, 0.0, 0.0,
						0.0, 1.0, 0.0,
						0.0, 0.0, 1.0 };
	uint16_t rowSize = 3;
	uint16_t colSize = 3;

	void Identity() { MatrixHelper::Identity(colSize, rowSize, matrix); }
	void Transpose() { MatrixHelper::Transpose(colSize, rowSize, matrix); }
	Matrix3 GetTranspose() { return Matrix3(MatrixHelper::GetTranspose(colSize, rowSize, matrix)); }
	void Inverse();

	double& operator()(const uint16_t row, const uint16_t column)
	{
		return this->matrix[(row * colSize) + column];
	}

	Matrix3 operator*(Matrix3& m);// { return MatrixHelper::Multiply3(this->matrix, colSize, rowSize, m.matrix, m.colSize, m.rowSize); }
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

	//void operator=(const Matrix4& m) { *this = m; };

	double matrix[16] = { 1.0, 0.0, 0.0, 0.0,
						  0.0, 1.0, 0.0, 0.0,
						  0.0, 0.0, 1.0, 0.0,
						  0.0, 0.0, 0.0, 1.0 };
	uint16_t rowSize = 4;
	uint16_t colSize = 4;

	void Identity() { MatrixHelper::Identity(colSize, rowSize, matrix); }
	void Transpose() { MatrixHelper::Transpose(colSize, rowSize, matrix); }
	Matrix4 GetTranspose() { return Matrix4(MatrixHelper::GetTranspose(colSize, rowSize, matrix)); }
	void Inverse();

	double& operator()(const uint16_t row, const uint16_t column)
	{
		return this->matrix[(row * colSize) + column];
	}

	Matrix4 operator*(Matrix4& m);// { return MatrixHelper::Multiply4(matrix, colSize, rowSize, m.matrix, m.colSize, m.rowSize); }
	Matrix4 operator*(double scalar);

	Matrix4 operator+(Matrix4& m);

	void DebugOutput();
};