#pragma once
#include <vector>

struct Matrix
{
	Matrix(unsigned _row, unsigned _col);
	~Matrix() = default;

	void Identity();

	double Get(unsigned _row, unsigned _col);

	Matrix operator+(Matrix& m);
	Matrix operator+(double scalar);
	Matrix operator-(Matrix& m);
	Matrix operator-(double scalar);

	Matrix operator*(Matrix& m);
	Matrix operator*(double scalar);

	Matrix operator/(double scalar);

	Matrix Transpose();

	double& operator()(const unsigned& _row, const unsigned& _col);
	unsigned GetRows() const;
	unsigned GetColumns() const;

	void DebugOutput();

	unsigned rowSize;
	unsigned colSize;

	std::vector<std::vector<double>> matrix;
};