#pragma once
#include <vector>
#include <array>


struct Matrix
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
};

//struct Matrix4 : Matrix
//{
//	Matrix4(std::array<double, 16> m4x4);
//	Matrix4();
//	~Matrix4() = default;
//
//	std::array<double, 16> matrix = { 0.0 };
//
//	void Inverse();
//	Matrix4 Transpose();
//};
//
//struct Matrix3 : Matrix
//{
//	Matrix3(std::array<double, 9> m3x3);
//	Matrix3();
//	~Matrix3() = default;
//
//	std::array<double, 9> matrix = { 0.0 };
//	
//	void Inverse();
//	Matrix3 Transpose();
//};