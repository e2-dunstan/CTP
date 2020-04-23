#include "Matrix.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <algorithm>

/*Matrix::Matrix()
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
	Identity();
}
Matrix4<>::Matrix4()
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
}

Matrix Matrix::Resize()
{
	Matrix mat;
	
	mat.matrix[0] = matrix[0];
	mat.matrix[1] = matrix[1];
	mat.matrix[2] = matrix[2];
	mat.matrix[4] = matrix[3];
	mat.matrix[5] = matrix[4];
	mat.matrix[6] = matrix[5];
	mat.matrix[8] = matrix[6];
	mat.matrix[9] = matrix[7];
	mat.matrix[10] = matrix[8];

	return mat;
}

void Matrix::Identity()
{
	for (unsigned r = 0; r < rowSize; r++)
	{
		for (unsigned c = 0; c < colSize; c++)
		{
			if (r == c)
			{
				matrix[(r * colSize) + c] = 1.0;
			}
			else
			{
				matrix[(r * colSize) + c] = 0.0;
			}
		}
	}
}*/

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

//double* MatrixHelper::GetTranspose(const uint16_t colSize, const uint16_t rowSize, double* arr)
//{
//	double* transpose = new double[rowSize * colSize];
//
//	for (uint16_t r = 0; r < rowSize; r++)
//	{
//		for (uint16_t c = 0; c < colSize; c++)
//		{
//			transpose[(r * colSize) + c] = arr[(c * rowSize) + r];
//		}
//	}
//	return transpose;
//}

/*Matrix3 MatrixHelper::Convert(const Matrix4& m4)
{
	double arr[9] =
	{
		m4.matrix[0],
		m4.matrix[1],
		m4.matrix[2],

		m4.matrix[4],
		m4.matrix[5],
		m4.matrix[6],

		m4.matrix[8],
		m4.matrix[9],
		m4.matrix[10],
	};
	return Matrix3(arr);
}

Matrix4 MatrixHelper::Convert(const Matrix3& m3)
{
	double arr[16] =
	{
		m3.matrix[0],
		m3.matrix[1],
		m3.matrix[2],
		0.0,
		m3.matrix[3],
		m3.matrix[4],
		m3.matrix[5],
		0.0,
		m3.matrix[6],
		m3.matrix[7],
		m3.matrix[8],
		0.0, 0.0, 0.0, 1.0
	};
	return Matrix4(arr);
}*/

//Matrix3 MatrixHelper::Multiply3(double* mat1, uint16_t colSize1, uint16_t rowSize1, double* mat2, uint16_t colSize2, uint16_t rowSize2)
//{
//	double multiplied[9];
//
//	if (colSize1 == rowSize2)
//	{
//		double temp = 0.0;
//
//		for (uint16_t r = 0; r < rowSize1; r++)
//		{
//			for (uint16_t c = 0; c < colSize2; c++)
//			{
//				temp = 0.0;
//				for (uint16_t k = 0; k < colSize1; k++)
//				{
//					temp += mat1[(r * colSize1) + k] * mat2[(k * colSize2) + c];
//				}
//				multiplied[(r * colSize2) + c] = temp;
//			}
//		}
//		return Matrix3(multiplied);
//	}
//	else
//	{
//		std::cout << "Invalid matrices to multiply! Returning Identity." << std::endl;
//		for (uint16_t r = 0; r < rowSize1; r++)
//		{
//			for (uint16_t c = 0; c < colSize2; c++)
//			{
//				if (r == c) multiplied[(r * colSize2) + c] = 1.0;
//				else multiplied[(r * colSize2) + c] = 0.0;
//			}
//		}
//		return Matrix3(multiplied);
//	}
//}
//Matrix4 MatrixHelper::Multiply4(double* mat1, uint16_t colSize1, uint16_t rowSize1, double* mat2, uint16_t colSize2, uint16_t rowSize2)
//{
//	double multiplied[16];
//
//	if (colSize1 == rowSize2)
//	{
//		double temp = 0.0;
//
//		for (uint16_t r = 0; r < rowSize1; r++)
//		{
//			for (uint16_t c = 0; c < colSize2; c++)
//			{
//				temp = 0.0;
//				for (uint16_t k = 0; k < colSize1; k++)
//				{
//					temp += mat1[(r * colSize1) + k] * mat2[(k * colSize2) + c];
//				}
//				multiplied[(r * colSize2) + c] = temp;
//			}
//		}
//		return Matrix4(multiplied);
//	}
//	else
//	{
//		std::cout << "Invalid matrices to multiply! Returning Identity." << std::endl;
//		for (uint16_t r = 0; r < rowSize1; r++)
//		{
//			for (uint16_t c = 0; c < colSize2; c++)
//			{
//				if (r == c) multiplied[(r * colSize2) + c] = 1.0;
//				else multiplied[(r * colSize2) + c] = 0.0;
//			}
//		}
//		return Matrix4(multiplied);
//	}
//}

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

/*Matrix Matrix::operator+(Matrix& m)
{
	Matrix sum(rowSize, colSize);

	for (unsigned r = 0; r < rowSize; r++)
	{
		for (unsigned c = 0; c < colSize; c++)
		{
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
	unsigned mColumns = m.GetColumns();
	Matrix multiplied = Matrix(this->rowSize, mColumns);

	if (this->colSize == m.GetRows())
	{
		double temp = 0.0;

		for (unsigned r = 0; r < this->rowSize; r++)
		{
			for (unsigned c = 0; c < mColumns; c++)
			{
				temp = 0.0;
				for (unsigned k = 0; k < this->colSize; k++)
				{
					//temp += matrix4x4[r][k] * m(k, c);
					temp += matrix[(r * colSize) + k] * m.matrix[(k * mColumns) + c];
				}
				//multiplied.matrix[(r * mColumns) + c] = temp;
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

Matrix4 Matrix4::Transpose()
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
}

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
}*/
