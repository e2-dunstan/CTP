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
