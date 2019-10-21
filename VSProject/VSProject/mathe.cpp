#include "mathe.h"

#define PI 3.14159265

//https://www.slideshare.net/DelwarHossain8/3d-transformation-computer-graphics

/*Vector3 Mathe::Transform(Vector3& vector, Matrix& matrix)
{
	Matrix vectorToMatrix = Matrix(4, 1);
	vectorToMatrix(0, 0) = vector.x;
	vectorToMatrix(1, 0) = vector.y;
	vectorToMatrix(2, 0) = vector.z;
	vectorToMatrix(3, 0) = 1;
	Matrix multiplied = matrix * vectorToMatrix;
	return Vector3(multiplied(0, 0), multiplied(1, 0), multiplied(2, 0));
}

Matrix Mathe::Translate(Matrix& m, double x, double y, double z)
{
	Matrix translation = Matrix(4, 4);
	translation(0, 3) = x;
	translation(1, 3) = y;
	translation(2, 3) = z;
	return m * translation;
}

//pitch, yaw, roll
Matrix Mathe::Rotate(double x, double y, double z)
{
	Matrix xMatrix = Matrix(4, 4);
	xMatrix(1, 1) = cos(x * PI / 180.0);
	xMatrix(1, 2) = -sin(x * PI / 180.0);
	xMatrix(2, 1) = sin(x * PI / 180.0);
	xMatrix(2, 2) = cos(x * PI / 180.0);

	Matrix yMatrix = Matrix(4, 4);
	yMatrix(0, 0) = cos(y * PI / 180.0);
	yMatrix(0, 2) = sin(y * PI / 180.0);
	yMatrix(2, 0) = -sin(y * PI / 180.0);
	yMatrix(2, 2) = cos(y * PI / 180.0);

	Matrix zMatrix = Matrix(4, 4);
	zMatrix(0, 0) = cos(z * PI / 180.0);
	zMatrix(1, 0) = -sin(z * PI / 180.0);
	zMatrix(0, 1) = sin(z * PI / 180.0);
	zMatrix(1, 0) = cos(z * PI / 180.0);

	Matrix rot = xMatrix * yMatrix;
	rot = rot * zMatrix;
	return rot;
}

Matrix Mathe::Scale(Matrix& m, double x, double y, double z)
{
	Matrix scale = Matrix(4, 4);
	scale(0, 0) = x;
	scale(1, 1) = y;
	scale(2, 2) = z;

	return m * scale;
}*/
