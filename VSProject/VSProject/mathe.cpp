#include "mathe.h"

#define PI 3.14159265

//https://www.slideshare.net/DelwarHossain8/3d-transformation-computer-graphics

void Mathe::Transform(Vector3& vector, Matrix& matrix4x4)
{
	Matrix vectorToMatrix = Matrix(4, 1);
	vectorToMatrix(0, 0) = vector.x;
	vectorToMatrix(1, 0) = vector.y;
	vectorToMatrix(2, 0) = vector.z;
	vectorToMatrix(3, 0) = 1;
	Matrix multiplied = matrix4x4 * vectorToMatrix;

	vector = Vector3(multiplied(0, 0), multiplied(1, 0), multiplied(2, 0));
}

Vector3 Mathe::matrixInverse(Matrix& m, Vector3& v)
{
	Matrix transp = m;
	Vector3 vec = v;
	vec.x -= transp(0, 3);
	vec.y -= transp(1, 3);
	vec.z -= transp(2, 3);

	Vector3 ret = Vector3(
		(vec.x * transp(0, 0)) + 
		(vec.y * transp(1, 0)) + 
		(vec.z * transp(2, 0)),
				
		(vec.x * transp(0, 1)) + 
		(vec.y * transp(1, 1)) + 
		(vec.z * transp(2, 1)),
				
		(vec.x * transp(0, 2)) + 
		(vec.y * transp(1, 2)) +
		(vec.z * transp(2, 2))
	);
	return ret;
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
	return Vector3(mat(i, 0), mat(i, 1), mat(i, 2));
}

void Mathe::TransformInverseInertiaTensor(Matrix& tensorWorld, const Matrix& tensorLocal, const Matrix& rot)
{
	//treating rot as a Mat4 and tensor mats as Mat3

	double t4 = rot.Get(0) * tensorLocal.Get(0)
		+ rot.Get(1) * tensorLocal.Get(4)
		+ rot.Get(2) * tensorLocal.Get(8);
	double t9 = rot.Get(0) * tensorLocal.Get(1)
		+ rot.Get(1) * tensorLocal.Get(5)
		+ rot.Get(2) * tensorLocal.Get(9);
	double t14 = rot.Get(0) * tensorLocal.Get(2)
		+ rot.Get(1) * tensorLocal.Get(6)
		+ rot.Get(2) * tensorLocal.Get(10);

	double t28 = rot.Get(4) * tensorLocal.Get(0)
		+ rot.Get(5) * tensorLocal.Get(4)
		+ rot.Get(6) * tensorLocal.Get(8);
	double t33 = rot.Get(4) * tensorLocal.Get(1)
		+ rot.Get(5) * tensorLocal.Get(5)
		+ rot.Get(6) * tensorLocal.Get(9);
	double t38 = rot.Get(4) * tensorLocal.Get(2)
		+ rot.Get(5) * tensorLocal.Get(6)
		+ rot.Get(6) * tensorLocal.Get(10);

	double t52 = rot.Get(8) * tensorLocal.Get(0)
		+ rot.Get(9) * tensorLocal.Get(4)
		+ rot.Get(10) * tensorLocal.Get(8);
	double t57 = rot.Get(8) * tensorLocal.Get(1)
		+ rot.Get(9) * tensorLocal.Get(5)
		+ rot.Get(10) * tensorLocal.Get(9);
	double t62 = rot.Get(8) * tensorLocal.Get(2)
		+ rot.Get(9) * tensorLocal.Get(6)
		+ rot.Get(10) * tensorLocal.Get(10);


	tensorWorld.matrix4x4[0] =
		t4 * rot.Get(0)
		+ t9 * rot.Get(1)
		+ t14 * rot.Get(2);
	tensorWorld.matrix4x4[1] = 
		t4 * rot.Get(4)
		+ t9 * rot.Get(5)
		+ t14 * rot.Get(6);
	tensorWorld.matrix4x4[2] = 
		t4 * rot.Get(8)
		+ t9 * rot.Get(9)
		+ t14 * rot.Get(10);
	tensorWorld.matrix4x4[3] = 0;

	tensorWorld.matrix4x4[4] = 
		t28 * rot.Get(0)
		+ t33 * rot.Get(1)
		+ t38 * rot.Get(2);
	tensorWorld.matrix4x4[5] =
		t28 * rot.Get(4)
		+ t33 * rot.Get(5)
		+ t38 * rot.Get(6);
	tensorWorld.matrix4x4[6] =
		t28 * rot.Get(8)
		+ t33 * rot.Get(9)
		+ t38 * rot.Get(10);
	tensorWorld.matrix4x4[7] = 0;

	tensorWorld.matrix4x4[8] =
		t52 * rot.Get(0)
		+ t57 * rot.Get(1)
		+ t62 * rot.Get(2);
	tensorWorld.matrix4x4[9] =
		t52 * rot.Get(4)
		+ t57 * rot.Get(5)
		+ t62 * rot.Get(6);
	tensorWorld.matrix4x4[10] =
		t52 * rot.Get(8)
		+ t57 * rot.Get(9)
		+ t62 * rot.Get(10);
	tensorWorld.matrix4x4[11] = 0;
}
