#include "Matrix.h"
#include <iostream>
#include <string>

Matrix::Matrix(unsigned _row, unsigned _col)
{
	rowSize = _row;
	colSize = _col;

	//matrix4x4.resize(rowSize);
	//for (unsigned i = 0; i < matrix4x4.size(); i++)
	//{
	//	matrix4x4[i].resize(colSize, 0);
	//}
	//
	//Init as identity
	Identity();
}

Matrix::Matrix(double m4x4[16])
{
	rowSize = 4;
	colSize = 4;

	matrix4x4[0] = m4x4[0];
	matrix4x4[1] = m4x4[1];
	matrix4x4[2] = m4x4[2];
	matrix4x4[3] = m4x4[3];
	matrix4x4[4] = m4x4[4];
	matrix4x4[5] = m4x4[5];
	matrix4x4[6] = m4x4[6];
	matrix4x4[7] = m4x4[7];
	matrix4x4[8] = m4x4[8];
	matrix4x4[9] = m4x4[9];
	matrix4x4[10] = m4x4[10];
	matrix4x4[11] = m4x4[11];
	matrix4x4[12] = m4x4[12];
	matrix4x4[13] = m4x4[13];
	matrix4x4[14] = m4x4[14];
	matrix4x4[15] = m4x4[15];
}

Matrix::~Matrix()
{
	//if (matrix4x4 != NULL)
	//{
	//	delete[] matrix4x4;
	//	matrix4x4 = NULL;
	//}
}

void Matrix::Identity()
{
	for (unsigned r = 0; r < rowSize; r++)
	{
		for (unsigned c = 0; c < colSize; c++)
		{
			if (r == c)
			{
				//matrix4x4[r][c] = 1;
				matrix4x4[(r * colSize) + c] = 1;
			}
			else
			{
				//matrix4x4[r][c] = 0;
				matrix4x4[(r * colSize) + c] = 0;
			}
		}
	}
}

void Matrix::Inverse3x3()
{
	double t4 = this->matrix4x4[0] * this->matrix4x4[5];
	double t6 = this->matrix4x4[0] * this->matrix4x4[6];
	double t8 = this->matrix4x4[1] * this->matrix4x4[4];
	double t10 = this->matrix4x4[2] * this->matrix4x4[4];
	double t12 = this->matrix4x4[1] * this->matrix4x4[8];
	double t14 = this->matrix4x4[2] * this->matrix4x4[8];

	//determinant
	double t16 = (t4 * this->matrix4x4[10]) - (t6 * this->matrix4x4[9])
		- (t8 * this->matrix4x4[10]) + (t10 * this->matrix4x4[9])
		+ (t12 * this->matrix4x4[6]) - (t14 * this->matrix4x4[5]);

	if (t16 < 0.01) return;
	double t17 = 1 / t16;

	this->matrix4x4[0] = ((this->matrix4x4[5] * this->matrix4x4[10]) - (this->matrix4x4[6] * this->matrix4x4[9])) * t17;
	this->matrix4x4[1] = -((this->matrix4x4[1] * this->matrix4x4[10]) - (this->matrix4x4[2] * this->matrix4x4[9])) * t17;
	this->matrix4x4[2] = ((this->matrix4x4[1] * this->matrix4x4[6]) - (this->matrix4x4[2] * this->matrix4x4[5])) * t17;
	this->matrix4x4[3] = 0;
	this->matrix4x4[4] = -((this->matrix4x4[3] * this->matrix4x4[10]) - (this->matrix4x4[6] * this->matrix4x4[8])) * t17;
	this->matrix4x4[5] = ((this->matrix4x4[0] * this->matrix4x4[10]) - t14) * t17;
	this->matrix4x4[6] = -(t6 - t10) * t17;
	this->matrix4x4[7] = 0;
	this->matrix4x4[8] = ((this->matrix4x4[3] * this->matrix4x4[9]) - (this->matrix4x4[5] * this->matrix4x4[8])) * t17;
	this->matrix4x4[9] = -((this->matrix4x4[0] * this->matrix4x4[9]) - t12) * t17;
	this->matrix4x4[10] = (t4 - t8) * t17;
	this->matrix4x4[11] = 0;
	this->matrix4x4[12] = 0;
	this->matrix4x4[13] = 0;
	this->matrix4x4[14] = 0;
	this->matrix4x4[15] = 0;
}

void Matrix::Inverse4x4()
{
	float A2323 = this->matrix4x4[10] * this->matrix4x4[15] - this->matrix4x4[11] * this->matrix4x4[14];
	float A1323 = this->matrix4x4[9] * this->matrix4x4[15] - this->matrix4x4[11] * this->matrix4x4[13];
	float A1223 = this->matrix4x4[9] * this->matrix4x4[14] - this->matrix4x4[10] * this->matrix4x4[13];
	float A0323 = this->matrix4x4[8] * this->matrix4x4[15] - this->matrix4x4[11] * this->matrix4x4[12];
	float A0223 = this->matrix4x4[8] * this->matrix4x4[14] - this->matrix4x4[10] * this->matrix4x4[12];
	float A0123 = this->matrix4x4[8] * this->matrix4x4[13] - this->matrix4x4[9] * this->matrix4x4[12];
	float A2313 = this->matrix4x4[6] * this->matrix4x4[15] - this->matrix4x4[7] * this->matrix4x4[14];
	float A1313 = this->matrix4x4[5] * this->matrix4x4[15] - this->matrix4x4[7] * this->matrix4x4[13];
	float A1213 = this->matrix4x4[5] * this->matrix4x4[14] - this->matrix4x4[6] * this->matrix4x4[13];
	float A2312 = this->matrix4x4[6] * this->matrix4x4[11] - this->matrix4x4[7] * this->matrix4x4[10];
	float A1312 = this->matrix4x4[5] * this->matrix4x4[11] - this->matrix4x4[7] * this->matrix4x4[9];
	float A1212 = this->matrix4x4[5] * this->matrix4x4[10] - this->matrix4x4[6] * this->matrix4x4[9];
	float A0313 = this->matrix4x4[4] * this->matrix4x4[15] - this->matrix4x4[7] * this->matrix4x4[12];
	float A0213 = this->matrix4x4[4] * this->matrix4x4[14] - this->matrix4x4[6] * this->matrix4x4[12];
	float A0312 = this->matrix4x4[4] * this->matrix4x4[11] - this->matrix4x4[7] * this->matrix4x4[8];
	float A0212 = this->matrix4x4[4] * this->matrix4x4[10] - this->matrix4x4[6] * this->matrix4x4[8];
	float A0113 = this->matrix4x4[4] * this->matrix4x4[13] - this->matrix4x4[5] * this->matrix4x4[12];
	float A0112 = this->matrix4x4[4] * this->matrix4x4[9] - this->matrix4x4[5] * this->matrix4x4[8];

	float det = this->matrix4x4[0] * (this->matrix4x4[5] * A2323 - this->matrix4x4[6] * A1323 + this->matrix4x4[7] * A1223)
		- this->matrix4x4[1] * (this->matrix4x4[4] * A2323 - this->matrix4x4[6] * A0323 + this->matrix4x4[7] * A0223)
		+ this->matrix4x4[2] * (this->matrix4x4[4] * A1323 - this->matrix4x4[5] * A0323 + this->matrix4x4[7] * A0123)
		- this->matrix4x4[3] * (this->matrix4x4[4] * A1223 - this->matrix4x4[5] * A0223 + this->matrix4x4[6] * A0123);
	det = 1.0f / det;

	this->matrix4x4[0] = det * (this->matrix4x4[5] * A2323 - this->matrix4x4[6] * A1323 + this->matrix4x4[7]* A1223);
	this->matrix4x4[1] = det * -(this->matrix4x4[1] * A2323 - this->matrix4x4[2] * A1323 + this->matrix4x4[3] * A1223);
	this->matrix4x4[2] = det * (this->matrix4x4[1] * A2313 - this->matrix4x4[2] * A1313 + this->matrix4x4[3] * A1213);
	this->matrix4x4[3] = det * -(this->matrix4x4[1] * A2312 - this->matrix4x4[2] * A1312 + this->matrix4x4[3] * A1212);
	this->matrix4x4[4] = det * -(this->matrix4x4[4] * A2323 - this->matrix4x4[6] * A0323 + this->matrix4x4[7]* A0223);
	this->matrix4x4[5] = det * (this->matrix4x4[0] * A2323 - this->matrix4x4[2] * A0323 + this->matrix4x4[3] * A0223);
	this->matrix4x4[6] = det * -(this->matrix4x4[0] * A2313 - this->matrix4x4[2] * A0313 + this->matrix4x4[3] * A0213);
	this->matrix4x4[7] = det * (this->matrix4x4[0] * A2312 - this->matrix4x4[2] * A0312 + this->matrix4x4[3] * A0212);
	this->matrix4x4[8] = det * (this->matrix4x4[4] * A1323 - this->matrix4x4[5] * A0323 + this->matrix4x4[7]* A0123);
	this->matrix4x4[9] = det * -(this->matrix4x4[0] * A1323 - this->matrix4x4[1] * A0323 + this->matrix4x4[3] * A0123);
	this->matrix4x4[10] = det * (this->matrix4x4[0] * A1313 - this->matrix4x4[1] * A0313 + this->matrix4x4[3] * A0113);
	this->matrix4x4[11] = det * -(this->matrix4x4[0] * A1312 - this->matrix4x4[1] * A0312 + this->matrix4x4[3] * A0112);
	this->matrix4x4[12] = det * -(this->matrix4x4[4] * A1223 - this->matrix4x4[5] * A0223 + this->matrix4x4[6] * A0123);
	this->matrix4x4[13] = det * (this->matrix4x4[0] * A1223 - this->matrix4x4[1] * A0223 + this->matrix4x4[2] * A0123);
	this->matrix4x4[14] = det * -(this->matrix4x4[0] * A1213 - this->matrix4x4[1] * A0213 + this->matrix4x4[2] * A0113);
	this->matrix4x4[15] = det * (this->matrix4x4[0] * A1212 - this->matrix4x4[1] * A0212 + this->matrix4x4[2] * A0112);
}

Matrix Matrix::operator+(Matrix& m)
{
	Matrix sum(rowSize, colSize);

	for (unsigned r = 0; r < rowSize; r++)
	{
		for (unsigned c = 0; c < colSize; c++)
		{
			//sum(r, c) = this->matrix4x4[r][c] + m(r, c);
			sum(r, c) = this->matrix4x4[(r * colSize) + c] + m(r, c);
		}
	}
	return sum;
}

Matrix Matrix::operator+(double scalar)
{
	Matrix result(rowSize, colSize);

	for (unsigned r = 0; r < rowSize; r++)
	{
		for (unsigned c = 0; c < colSize; c++)
		{
			//result(r, c) = this->matrix4x4[r][c] + scalar;
			result(r, c) = this->matrix4x4[(r * colSize) + c] + scalar;
		}
	}
	return result;
}

Matrix Matrix::operator-(Matrix& m)
{
	Matrix sub(rowSize, colSize);
	for (unsigned r = 0; r < rowSize; r++)
	{
		for (unsigned c = 0; c < colSize; c++)
		{
			//sub(r, c) = this->matrix4x4[r][c] - m(r, c);
			sub(r, c) = this->matrix4x4[(r * colSize) + c] - m(r, c);
		}
	}
	return sub;
}

Matrix Matrix::operator-(double scalar)
{
	Matrix result(rowSize, colSize);

	for (unsigned r = 0; r < rowSize; r++)
	{
		for (unsigned c = 0; c < colSize; c++)
		{
			//result(r, c) = this->matrix4x4[r][c] - scalar;
			result(r, c) = this->matrix4x4[(r * colSize) + c] - scalar;
		}
	}
	return result;
}

Matrix Matrix::operator*(Matrix& m)
{
	Matrix multiplied(this->rowSize, m.GetColumns());

	if (this->colSize == m.GetRows())
	{
		double temp = 0.0;

		for (unsigned r = 0; r < this->rowSize; r++)
		{
			for (unsigned c = 0; c < m.GetColumns(); c++)
			{
				temp = 0.0;
				for (unsigned k = 0; k < this->colSize; k++)
				{
					//temp += matrix4x4[r][k] * m(k, c);
					temp += matrix4x4[(r * colSize) + k] * m(k, c);
				}
				multiplied(r, c) = temp;
			}
		}
		return multiplied;
	}
	else
	{
		std::cout << "Invalid matrices to multiply! Returning Identity." << std::endl;
		return multiplied;
	}
}

Matrix Matrix::operator*(double scalar)
{
	Matrix result(rowSize, colSize);

	for (unsigned r = 0; r < rowSize; r++)
	{
		for (unsigned c = 0; c < colSize; c++)
		{
			result(r, c) = this->matrix4x4[(r * colSize) + c] * scalar;
		}
	}
	return result;
}

Matrix Matrix::operator/(double scalar)
{
	Matrix result(rowSize, colSize);

	for (unsigned r = 0; r < rowSize; r++)
	{
		for (unsigned c = 0; c < colSize; c++)
		{
			result(r, c) = this->matrix4x4[(r * colSize) + c] / scalar;
		}
	}
	return result;
}

Matrix Matrix::Transpose()
{
	Matrix transposed(rowSize, colSize);

	for (unsigned r = 0; r < rowSize; r++)
	{
		for (unsigned c = 0; c < colSize; c++)
		{
			transposed(r, c) = this->matrix4x4[(c * rowSize) + r];
		}
	}
	return transposed;
}

double& Matrix::operator()(const unsigned& _row, const unsigned& _col)
{
	return this->matrix4x4[(_row * colSize) + _col];
}

double Matrix::Get(int i) const
{
	return this->matrix4x4[i];
}

unsigned Matrix::GetRows() const
{
	return this->rowSize;
}

unsigned Matrix::GetColumns() const
{
	return this->colSize;
}

void Matrix::DebugOutput()
{
	std::string str = "\n[ ";

	for (unsigned r = 0; r < rowSize; r++)
	{
		for (unsigned c = 0; c < colSize; c++)
		{
			str += std::to_string(matrix4x4[(r * colSize) + c]);
			if (c < colSize - 1)
			{
				str += " , ";
			}
			else
			{
				str += " ]\n";
			}
		}
		if (r < rowSize - 1)
		{
			str += "[ ";
		}
	}

	std::cout << str << std::endl;
}
