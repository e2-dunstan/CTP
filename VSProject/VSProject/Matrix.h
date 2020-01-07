#pragma once
#include <vector>

struct Matrix
{
	Matrix(unsigned _row, unsigned _col);
	Matrix(double m4x4[16]);
	~Matrix();

	void Identity();

	void Inverse3x3();

	Matrix operator+(Matrix& m);
	Matrix operator+(double scalar);
	Matrix operator-(Matrix& m);
	Matrix operator-(double scalar);

	Matrix operator*(Matrix& m);
	Matrix operator*(double scalar);

	Matrix operator/(double scalar);

	Matrix Transpose();

	double& operator()(const unsigned& _row, const unsigned& _col);
	double Get(int i) const;
	unsigned GetRows() const;
	unsigned GetColumns() const;

	void DebugOutput();

	unsigned rowSize;
	unsigned colSize;

	//std::vector<std::vector<double>> matrix4x4;
	
	double matrix4x4[16] = { 0 };
};