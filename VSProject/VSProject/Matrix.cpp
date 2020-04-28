#include "Matrix.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <algorithm>


void MatrixHelper::Identity(const uint16_t colSize, const uint16_t rowSize, double* arr)
{
	for (uint16_t c = 0; c < colSize; c++)
	{
		for (uint16_t r = 0; r < rowSize; r++)
		{
			if (c == r) arr[(r * colSize) + c] = 1.0;
			else arr[(r * colSize) + c] = 0.0;
		}
	}
}

Matrix3::Matrix3(double matrixValues[9])
{
	matrix[0] = matrixValues[0];
	matrix[1] = matrixValues[1];
	matrix[2] = matrixValues[2];
	matrix[3] = matrixValues[3];
	matrix[4] = matrixValues[4];
	matrix[5] = matrixValues[5];
	matrix[6] = matrixValues[6];
	matrix[7] = matrixValues[7];
	matrix[8] = matrixValues[8];
}

Matrix3 Matrix3::GetTranspose()
{
	Matrix3 transpose;
	for (uint16_t r = 0; r < 3; r++)
	{
		for (uint16_t c = 0; c < 3; c++)
		{
			transpose.matrix[(r * colSize) + c] = this->matrix[(c * rowSize) + r];
		}
	}
	return transpose;
}

void Matrix3::Inverse()
{	
	double det = this->matrix[0] * (this->matrix[4] * this->matrix[8] - this->matrix[7] * this->matrix[5]) -
				 this->matrix[1] * (this->matrix[3] * this->matrix[8] - this->matrix[5] * this->matrix[6]) +
				 this->matrix[2] * (this->matrix[3] * this->matrix[7] - this->matrix[4] * this->matrix[6]);
	det = 1.0 / det;

	Matrix3 inv;
	inv.matrix[0] = (this->matrix[4] * this->matrix[8] - this->matrix[7] * this->matrix[5]) * det;
	inv.matrix[1] = (this->matrix[2] * this->matrix[7] - this->matrix[1] * this->matrix[8]) * det;
	inv.matrix[2] = (this->matrix[1] * this->matrix[5] - this->matrix[2] * this->matrix[4]) * det;
	inv.matrix[3] = (this->matrix[5] * this->matrix[6] - this->matrix[3] * this->matrix[8]) * det;
	inv.matrix[4] = (this->matrix[0] * this->matrix[8] - this->matrix[2] * this->matrix[6]) * det;
	inv.matrix[5] = (this->matrix[3] * this->matrix[2] - this->matrix[0] * this->matrix[5]) * det;
	inv.matrix[6] = (this->matrix[3] * this->matrix[7] - this->matrix[6] * this->matrix[4]) * det;
	inv.matrix[7] = (this->matrix[6] * this->matrix[1] - this->matrix[0] * this->matrix[7]) * det;
	inv.matrix[8] = (this->matrix[0] * this->matrix[4] - this->matrix[3] * this->matrix[1]) * det;

	*this = inv;
}

Matrix3 Matrix3::operator*(Matrix3& m)
{
	double multiplied[9];

	if (colSize == m.rowSize)
	{
		double temp = 0.0;

		for (uint16_t r = 0; r < rowSize; r++)
		{
			for (uint16_t c = 0; c < m.colSize; c++)
			{
				temp = 0.0;
				for (uint16_t k = 0; k < colSize; k++)
				{
					temp += this->matrix[(r * colSize) + k] * m.matrix[(k * m.colSize) + c];
				}
				multiplied[(r * m.colSize) + c] = temp;
			}
		}
		return Matrix3(multiplied);
	}
	else
	{
		std::cout << "Invalid matrices to multiply! Returning Identity." << std::endl;
		for (uint16_t r = 0; r < rowSize; r++)
		{
			for (uint16_t c = 0; c < m.colSize; c++)
			{
				if (r == c) multiplied[(r * m.colSize) + c] = 1.0;
				else multiplied[(r * m.colSize) + c] = 0.0;
			}
		}
		return Matrix3(multiplied);
	}
}

Matrix3 Matrix3::operator*(double scalar)
{
	Matrix3 result;
	for (unsigned r = 0; r < rowSize; r++)
	{
		for (unsigned c = 0; c < colSize; c++)
		{
			result(r, c) = this->matrix[(r * colSize) + c] * scalar;
		}
	}
	return result;
}

Matrix3 Matrix3::operator+(Matrix3& m)
{
	Matrix3 sum;
	for (unsigned r = 0; r < rowSize; r++)
	{
		for (unsigned c = 0; c < colSize; c++)
		{
			sum(r, c) = this->matrix[(r * colSize) + c] + m(r, c);
		}
	}
	return sum;
}

void Matrix3::DebugOutput()
{
	std::string str = "\n[ ";

	for (uint16_t r = 0; r < rowSize; r++)
	{
		for (uint16_t c = 0; c < colSize; c++)
		{
			str += std::to_string(matrix[(r * colSize) + c]);
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

Matrix4::Matrix4(double matrixValues[16])
{
	matrix[0] = matrixValues[0];
	matrix[1] = matrixValues[1];
	matrix[2] = matrixValues[2];
	matrix[3] = matrixValues[3];
	matrix[4] = matrixValues[4];
	matrix[5] = matrixValues[5];
	matrix[6] = matrixValues[6];
	matrix[7] = matrixValues[7];
	matrix[8] = matrixValues[8];
	matrix[9] = matrixValues[9];
	matrix[10] = matrixValues[10];
	matrix[11] = matrixValues[11];
	matrix[12] = matrixValues[12];
	matrix[13] = matrixValues[13];
	matrix[14] = matrixValues[14];
	matrix[15] = matrixValues[15];
}

Matrix4 Matrix4::GetTranspose()
{
	Matrix4 transpose;
	for (uint16_t r = 0; r < 4; r++)
	{
		for (uint16_t c = 0; c < 4; c++)
		{
			transpose.matrix[(r * colSize) + c] = this->matrix[(c * rowSize) + r];
		}
	}
	return transpose;
}

void Matrix4::Inverse()
{
	double A2323 = this->matrix[10] * this->matrix[15] - this->matrix[11] * this->matrix[14];
	double A1323 = this->matrix[9] * this->matrix[15] - this->matrix[11] * this->matrix[13];
	double A1223 = this->matrix[9] * this->matrix[14] - this->matrix[10] * this->matrix[13];
	double A0323 = this->matrix[8] * this->matrix[15] - this->matrix[11] * this->matrix[12];
	double A0223 = this->matrix[8] * this->matrix[14] - this->matrix[10] * this->matrix[12];
	double A0123 = this->matrix[8] * this->matrix[13] - this->matrix[9] * this->matrix[12];
	double A2313 = this->matrix[6] * this->matrix[15] - this->matrix[7] * this->matrix[14];
	double A1313 = this->matrix[5] * this->matrix[15] - this->matrix[7] * this->matrix[13];
	double A1213 = this->matrix[5] * this->matrix[14] - this->matrix[6] * this->matrix[13];
	double A2312 = this->matrix[6] * this->matrix[11] - this->matrix[7] * this->matrix[10];
	double A1312 = this->matrix[5] * this->matrix[11] - this->matrix[7] * this->matrix[9];
	double A1212 = this->matrix[5] * this->matrix[10] - this->matrix[6] * this->matrix[9];
	double A0313 = this->matrix[4] * this->matrix[15] - this->matrix[7] * this->matrix[12];
	double A0213 = this->matrix[4] * this->matrix[14] - this->matrix[6] * this->matrix[12];
	double A0312 = this->matrix[4] * this->matrix[11] - this->matrix[7] * this->matrix[8];
	double A0212 = this->matrix[4] * this->matrix[10] - this->matrix[6] * this->matrix[8];
	double A0113 = this->matrix[4] * this->matrix[13] - this->matrix[5] * this->matrix[12];
	double A0112 = this->matrix[4] * this->matrix[9] - this->matrix[5] * this->matrix[8];

	double det = this->matrix[0] * (this->matrix[5] * A2323 - this->matrix[6] * A1323 + this->matrix[7] * A1223)
		- this->matrix[1] * (this->matrix[4] * A2323 - this->matrix[6] * A0323 + this->matrix[7] * A0223)
		+ this->matrix[2] * (this->matrix[4] * A1323 - this->matrix[5] * A0323 + this->matrix[7] * A0123)
		- this->matrix[3] * (this->matrix[4] * A1223 - this->matrix[5] * A0223 + this->matrix[6] * A0123);
	det = 1.0 / det;

	Matrix4 inv;
	inv.matrix[0] = det * (this->matrix[5] * A2323 - this->matrix[6] * A1323 + this->matrix[7]* A1223);
	inv.matrix[1] = det * -(this->matrix[1] * A2323 - this->matrix[2] * A1323 + this->matrix[3] * A1223);
	inv.matrix[2] = det * (this->matrix[1] * A2313 - this->matrix[2] * A1313 + this->matrix[3] * A1213);
	inv.matrix[3] = det * -(this->matrix[1] * A2312 - this->matrix[2] * A1312 + this->matrix[3] * A1212);
	inv.matrix[4] = det * -(this->matrix[4] * A2323 - this->matrix[6] * A0323 + this->matrix[7]* A0223);
	inv.matrix[5] = det * (this->matrix[0] * A2323 - this->matrix[2] * A0323 + this->matrix[3] * A0223);
	inv.matrix[6] = det * -(this->matrix[0] * A2313 - this->matrix[2] * A0313 + this->matrix[3] * A0213);
	inv.matrix[7] = det * (this->matrix[0] * A2312 - this->matrix[2] * A0312 + this->matrix[3] * A0212);
	inv.matrix[8] = det * (this->matrix[4] * A1323 - this->matrix[5] * A0323 + this->matrix[7]* A0123);
	inv.matrix[9] = det * -(this->matrix[0] * A1323 - this->matrix[1] * A0323 + this->matrix[3] * A0123);
	inv.matrix[10] = det * (this->matrix[0] * A1313 - this->matrix[1] * A0313 + this->matrix[3] * A0113);
	inv.matrix[11] = det * -(this->matrix[0] * A1312 - this->matrix[1] * A0312 + this->matrix[3] * A0112);
	inv.matrix[12] = det * -(this->matrix[4] * A1223 - this->matrix[5] * A0223 + this->matrix[6] * A0123);
	inv.matrix[13] = det * (this->matrix[0] * A1223 - this->matrix[1] * A0223 + this->matrix[2] * A0123);
	inv.matrix[14] = det * -(this->matrix[0] * A1213 - this->matrix[1] * A0213 + this->matrix[2] * A0113);
	inv.matrix[15] = det * (this->matrix[0] * A1212 - this->matrix[1] * A0212 + this->matrix[2] * A0112);

	*this = inv;
}

Matrix3 Matrix4::ToMatrix3()
{
	double vals[9] = {
		matrix[0],
		matrix[1],
		matrix[2],

		matrix[4],
		matrix[5],
		matrix[6],

		matrix[8],
		matrix[9],
		matrix[10]
	};
	return Matrix3(vals);
}

Matrix4 Matrix4::operator*(Matrix4& m)
{
	double multiplied[16];

	if (colSize == m.rowSize)
	{
		double temp = 0.0;

		for (uint16_t r = 0; r < rowSize; r++)
		{
			for (uint16_t c = 0; c < m.colSize; c++)
			{
				temp = 0.0;
				for (uint16_t k = 0; k < colSize; k++)
				{
					temp += this->matrix[(r * colSize) + k] * m.matrix[(k * m.colSize) + c];
				}
				multiplied[(r * m.colSize) + c] = temp;
			}
		}
		return Matrix4(multiplied);
	}
	else
	{
		std::cout << "Invalid matrices to multiply! Returning Identity." << std::endl;
		for (uint16_t r = 0; r < 4; r++)
		{
			for (uint16_t c = 0; c < 4; c++)
			{
				if (r == c) multiplied[(r * m.colSize) + c] = 1.0;
				else multiplied[(r * m.colSize) + c] = 0.0;
			}
		}
		return Matrix4(multiplied);
	}
}

Matrix4 Matrix4::operator*(double scalar)
{
	Matrix4 result;
	for (unsigned r = 0; r < rowSize; r++)
	{
		for (unsigned c = 0; c < colSize; c++)
		{
			result(r, c) = this->matrix[(r * colSize) + c] * scalar;
		}
	}
	return result;
}

Matrix4 Matrix4::operator+(Matrix4& m)
{
	Matrix4 sum;
	for (unsigned r = 0; r < rowSize; r++)
	{
		for (unsigned c = 0; c < colSize; c++)
		{
			sum(r, c) = this->matrix[(r * colSize) + c] + m(r, c);
		}
	}
	return sum;
}

void Matrix4::DebugOutput()
{
	std::string str = "\n[ ";

	for (uint16_t r = 0; r < rowSize; r++)
	{
		for (uint16_t c = 0; c < colSize; c++)
		{
			str += std::to_string(matrix[(r * colSize) + c]);
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