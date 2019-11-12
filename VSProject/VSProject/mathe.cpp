#include "mathe.h"

#define PI 3.14159265

//https://www.slideshare.net/DelwarHossain8/3d-transformation-computer-graphics

void Mathe::Transform(Vector3& vector, Matrix& matrix)
{
	Matrix vectorToMatrix = Matrix(4, 1);
	vectorToMatrix(0, 0) = vector.x;
	vectorToMatrix(1, 0) = vector.y;
	vectorToMatrix(2, 0) = vector.z;
	vectorToMatrix(3, 0) = 1;
	Matrix multiplied = matrix * vectorToMatrix;

	vector = Vector3(multiplied(0, 0), multiplied(1, 0), multiplied(2, 0));
}

void Mathe::Translate(Matrix& m, double x, double y, double z)
{
	Matrix translation = Matrix(4, 4);
	translation(0, 3) = x;
	translation(1, 3) = y;
	translation(2, 3) = z;
	
	m = m * translation;
}

//pitch, yaw, roll
void Mathe::Rotate(Matrix& m, double x, double y, double z)
{
	Matrix xMatrix = Matrix(4, 4);
	xMatrix(1, 1) = cos(x == 0 ? 0 : x * PI / 180.0);
	xMatrix(1, 2) = -sin(x == 0 ? 0 : x * PI / 180.0);
	xMatrix(2, 1) = sin(x == 0 ? 0 : x * PI / 180.0);
	xMatrix(2, 2) = cos(x == 0 ? 0 : x * PI / 180.0);

	Matrix yMatrix = Matrix(4, 4);
	yMatrix(0, 0) = cos(y == 0 ? 0 : y * PI / 180.0);
	yMatrix(0, 2) = sin(y == 0 ? 0 : y * PI / 180.0);
	yMatrix(2, 0) = -sin(y == 0 ? 0 : y * PI / 180.0);
	yMatrix(2, 2) = cos(y == 0 ? 0 : y * PI / 180.0);

	Matrix zMatrix = Matrix(4, 4);
	zMatrix(0, 0) = cos(z == 0 ? 0 : z * PI / 180.0);
	zMatrix(1, 0) = -sin(z == 0 ? 0 : z * PI / 180.0);
	zMatrix(0, 1) = sin(z == 0 ? 0 : z * PI / 180.0);
	zMatrix(1, 1) = cos(z == 0 ? 0 : z * PI / 180.0);

	Matrix rot = xMatrix * yMatrix * zMatrix;
	m = m * rot;
}

void Mathe::Scale(Matrix& m, double x, double y, double z)
{
	Matrix scale = Matrix(4, 4);
	scale(0, 0) = x;
	scale(1, 1) = y;
	scale(2, 2) = z;

	m = m * scale;
}

Vector3 Mathe::GetAxis(unsigned i, Matrix mat)
{
	return Vector3(mat.Get(i, 0), mat.Get(i, 1), mat.Get(i, 2));
}
