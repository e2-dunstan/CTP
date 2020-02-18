#include "Matrix.h"
#include <iostream>
#include <string>
#include <algorithm>

Matrix::Matrix()
{
	Matrix(4,4);
}

Matrix::Matrix(double arr[])
{
	for (unsigned i = 0; i < 16; i++)
	{
		matrix[i] = arr[i];
	}
}

Matrix::Matrix(unsigned _row, unsigned _col)
{
	rowSize = _row;
	colSize = _col;

	//Init as identity
	Identity();
}


/*Matrix4<>::Matrix4()
{
	rowSize = 4;
	colSize = 4;

	matrix = matrix.data();

	Identity();
}

Matrix3::Matrix3()
{
	rowSize = 3;
	colSize = 3;

	matrix = matrix.data();

	Identity();
}

Matrix4::Matrix4(std::array<double, 16> m4x4)
{
	rowSize = 4;
	colSize = 4;

	matrix = m4x4;
	matrix = matrix.data();

	//matrix[0] = m4x4[0];
	//matrix[1] = m4x4[1];
	//matrix[2] = m4x4[2];
	//matrix[3] = m4x4[3];
	//matrix[4] = m4x4[4];
	//matrix[5] = m4x4[5];
	//matrix[6] = m4x4[6];
	//matrix[7] = m4x4[7];
	//matrix[8] = m4x4[8];
	//matrix[9] = m4x4[9];
	//matrix[10] = m4x4[10];
	//matrix[11] = m4x4[11];
	//matrix[12] = m4x4[12];
	//matrix[13] = m4x4[13];
	//matrix[14] = m4x4[14];
	//matrix[15] = m4x4[15];
}

Matrix3::Matrix3(std::array<double, 9> m3x3)
{
	rowSize = 3;
	colSize = 3;

	matrix = m3x3;
	matrix = matrix.data();


	//matrix[0] = m3x3[0];
	//matrix[1] = m3x3[1];
	//matrix[2] = m3x3[2];
	//matrix[3] = m3x3[3];
	//matrix[4] = m3x3[4];
	//matrix[5] = m3x3[5];
	//matrix[6] = m3x3[6];
	//matrix[7] = m3x3[7];
	//matrix[8] = m3x3[8];
}*/

//Matrix Matrix::Resize()
//{
//	Matrix mat;
//	
//	mat.matrix[0] = matrix[0];
//	mat.matrix[1] = matrix[1];
//	mat.matrix[2] = matrix[2];
//	mat.matrix[4] = matrix[3];
//	mat.matrix[5] = matrix[4];
//	mat.matrix[6] = matrix[5];
//	mat.matrix[8] = matrix[6];
//	mat.matrix[9] = matrix[7];
//	mat.matrix[10] = matrix[8];
//
//	return mat;
//}

void Matrix::Identity()
{
	for (unsigned r = 0; r < rowSize; r++)
	{
		for (unsigned c = 0; c < colSize; c++)
		{
			if (r == c)
			{
				matrix[(r * colSize) + c] = 1;
			}
			else
			{
				matrix[(r * colSize) + c] = 0;
			}
		}
	}
}

void Matrix::Inverse3x3()
{
	double t4 = this->matrix[0] * this->matrix[5];
	double t6 = this->matrix[0] * this->matrix[6];
	double t8 = this->matrix[1] * this->matrix[4];
	double t10 = this->matrix[2] * this->matrix[4];
	double t12 = this->matrix[1] * this->matrix[8];
	double t14 = this->matrix[2] * this->matrix[8];

	//determinant
	double t16 = (t4 * this->matrix[10]) - (t6 * this->matrix[9])
		- (t8 * this->matrix[10]) + (t10 * this->matrix[9])
		+ (t12 * this->matrix[6]) - (t14 * this->matrix[5]);

	if (t16 < 0.01) return;
	double t17 = 1.0 / t16;

	this->matrix[0] = ((this->matrix[5] * this->matrix[10]) - (this->matrix[6] * this->matrix[9])) * t17;
	this->matrix[1] = -((this->matrix[1] * this->matrix[10]) - (this->matrix[2] * this->matrix[9])) * t17;
	this->matrix[2] = ((this->matrix[1] * this->matrix[6]) - (this->matrix[2] * this->matrix[5])) * t17;
	this->matrix[3] = -((this->matrix[4] * this->matrix[10]) - (this->matrix[6] * this->matrix[8])) * t17;
	this->matrix[4] = ((this->matrix[0] * this->matrix[10]) - t14) * t17;
	this->matrix[5] = -(t6 - t10) * t17;
	this->matrix[6] = ((this->matrix[4] * this->matrix[9]) - (this->matrix[5] * this->matrix[8])) * t17;
	this->matrix[7] = -((this->matrix[0] * this->matrix[9]) - t12) * t17;
	this->matrix[8] = (t4 - t8) * t17;
}

void Matrix::Inverse4x4()
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

	this->matrix[0] = det * (this->matrix[5] * A2323 - this->matrix[6] * A1323 + this->matrix[7]* A1223);
	this->matrix[1] = det * -(this->matrix[1] * A2323 - this->matrix[2] * A1323 + this->matrix[3] * A1223);
	this->matrix[2] = det * (this->matrix[1] * A2313 - this->matrix[2] * A1313 + this->matrix[3] * A1213);
	this->matrix[3] = det * -(this->matrix[1] * A2312 - this->matrix[2] * A1312 + this->matrix[3] * A1212);
	this->matrix[4] = det * -(this->matrix[4] * A2323 - this->matrix[6] * A0323 + this->matrix[7]* A0223);
	this->matrix[5] = det * (this->matrix[0] * A2323 - this->matrix[2] * A0323 + this->matrix[3] * A0223);
	this->matrix[6] = det * -(this->matrix[0] * A2313 - this->matrix[2] * A0313 + this->matrix[3] * A0213);
	this->matrix[7] = det * (this->matrix[0] * A2312 - this->matrix[2] * A0312 + this->matrix[3] * A0212);
	this->matrix[8] = det * (this->matrix[4] * A1323 - this->matrix[5] * A0323 + this->matrix[7]* A0123);
	this->matrix[9] = det * -(this->matrix[0] * A1323 - this->matrix[1] * A0323 + this->matrix[3] * A0123);
	this->matrix[10] = det * (this->matrix[0] * A1313 - this->matrix[1] * A0313 + this->matrix[3] * A0113);
	this->matrix[11] = det * -(this->matrix[0] * A1312 - this->matrix[1] * A0312 + this->matrix[3] * A0112);
	this->matrix[12] = det * -(this->matrix[4] * A1223 - this->matrix[5] * A0223 + this->matrix[6] * A0123);
	this->matrix[13] = det * (this->matrix[0] * A1223 - this->matrix[1] * A0223 + this->matrix[2] * A0123);
	this->matrix[14] = det * -(this->matrix[0] * A1213 - this->matrix[1] * A0213 + this->matrix[2] * A0113);
	this->matrix[15] = det * (this->matrix[0] * A1212 - this->matrix[1] * A0212 + this->matrix[2] * A0112);
}

Matrix Matrix::operator+(Matrix& m)
{
	Matrix sum(rowSize, colSize);

	for (unsigned r = 0; r < rowSize; r++)
	{
		for (unsigned c = 0; c < colSize; c++)
		{
			//sum(r, c) = this->matrix4x4[r][c] + m(r, c);
			sum(r, c) = this->matrix[(r * colSize) + c] + m(r, c);
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
			result(r, c) = this->matrix[(r * colSize) + c] + scalar;
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
			sub(r, c) = this->matrix[(r * colSize) + c] - m(r, c);
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
			result(r, c) = this->matrix[(r * colSize) + c] - scalar;
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
					temp += matrix[(r * colSize) + k] * m(k, c);
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
			result(r, c) = this->matrix[(r * colSize) + c] * scalar;
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
			result(r, c) = this->matrix[(r * colSize) + c] / scalar;
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
			transposed(r, c) = this->matrix[(c * rowSize) + r];
		}
	}
	return transposed;
}

/*Matrix4 Matrix4::Transpose()
{
	Matrix4 transposed;

	for (unsigned r = 0; r < rowSize; r++)
	{
		for (unsigned c = 0; c < colSize; c++)
		{
			transposed(r, c) = this->matrix[(c * rowSize) + r];
		}
	}
	return transposed;
}

Matrix3 Matrix3::Transpose()
{
	Matrix3 transposed;

	for (unsigned r = 0; r < rowSize; r++)
	{
		for (unsigned c = 0; c < colSize; c++)
		{
			transposed(r, c) = this->matrix[(c * rowSize) + r];
		}
	}
	return transposed;
}*/

double& Matrix::operator()(const unsigned& _row, const unsigned& _col)
{
	return this->matrix[(_row * colSize) + _col];
}

double Matrix::Get(int i) const
{
	return this->matrix[i];
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
