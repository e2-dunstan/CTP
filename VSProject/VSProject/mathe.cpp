#include "mathe.h"

#define PI 3.14159265

void Mathe::Transform(Vector3& vector, Matrix& matrix)
{
	if (IsVectorNAN(vector)) return;

	Matrix vectorToMatrix = Matrix(4, 1);
	vectorToMatrix(0, 0) = vector.x;
	vectorToMatrix(1, 0) = vector.y;
	vectorToMatrix(2, 0) = vector.z;
	vectorToMatrix(3, 0) = 1;
	Matrix multiplied = matrix * vectorToMatrix;

	vector = Vector3(multiplied(0, 0), multiplied(1, 0), multiplied(2, 0));
}

void Mathe::TransformTranspose(Vector3& vector, Matrix& matrix)
{
	Matrix vectorToMatrix = Matrix(4, 1);
	vectorToMatrix(0, 0) = vector.x;
	vectorToMatrix(1, 0) = vector.y;
	vectorToMatrix(2, 0) = vector.z;
	vectorToMatrix(3, 0) = 1;
	Matrix multiplied = matrix.Transpose() * vectorToMatrix;

	vector = Vector3(multiplied(0, 0), multiplied(1, 0), multiplied(2, 0));
}

Vector3 Mathe::MatrixInverse(Matrix& m, Vector3& v)
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
	xMatrix(1, 1) = cos(x == 0 ? 0 : ToRadians(x));
	xMatrix(1, 2) = -sin(x == 0 ? 0 : ToRadians(x));
	xMatrix(2, 1) = sin(x == 0 ? 0 : ToRadians(x));
	xMatrix(2, 2) = cos(x == 0 ? 0 : ToRadians(x));

	Matrix yMatrix = Matrix(4, 4);
	yMatrix(0, 0) = cos(y == 0 ? 0 : ToRadians(y));
	yMatrix(0, 2) = sin(y == 0 ? 0 : ToRadians(y));
	yMatrix(2, 0) = -sin(y == 0 ? 0 : ToRadians(y));
	yMatrix(2, 2) = cos(y == 0 ? 0 : ToRadians(y));

	Matrix zMatrix = Matrix(4, 4);
	zMatrix(0, 0) = cos(z == 0 ? 0 : ToRadians(z));
	zMatrix(1, 0) = -sin(z == 0 ? 0 : ToRadians(z));
	zMatrix(0, 1) = sin(z == 0 ? 0 : ToRadians(z));
	zMatrix(1, 1) = cos(z == 0 ? 0 : ToRadians(z));

	Matrix rot = xMatrix * yMatrix * zMatrix;
	m = m * rot;
}

void Mathe::Rotate(Matrix& m, const Quaternion& q)
{
	m(0, 0) = 1 - (2 * q.j * q.j) - (2 * q.k * q.k);
	m(0, 1) = (2 * q.i * q.j) - (2 * q.r * q.k);
	m(0, 2) = (2 * q.i * q.k) + (2 * q.r * q.j);

	m(1, 0) = (2 * q.i * q.j) + (2 * q.r * q.k);
	m(1, 1) = 1 - (2 * q.i * q.i) - (2 * q.k * q.k);
	m(1, 2) = (2 * q.j * q.k) - (2 * q.r * q.i);

	m(2, 0) = (2 * q.i * q.k) - (2 * q.r * q.j);
	m(2, 1) = (2 * q.j * q.k) + (2 * q.r * q.i);
	m(2, 2) = 1 - (2 * q.i * q.i) - (2 * q.j * q.j);
}

void Mathe::Scale(Matrix& m, double x, double y, double z)
{
	Matrix scale = Matrix(4, 4);

	scale(0, 0) = x;
	scale(1, 1) = y;
	scale(2, 2) = z;

	m = m * scale;
}

Vector3 Mathe::GetAxis(unsigned i, Matrix& mat)
{
	//return Vector3(mat.matrix[i], mat.matrix[i + 4], mat.matrix[i + 8]);
	return Vector3(mat(0, i), mat(1, i), mat(2, i));
}

Quaternion Mathe::VectorToQuaternion(const Vector3& v)
{
	double x = ToRadians(v.x);
	double y = ToRadians(v.y);
	double z = ToRadians(v.z);

	// yaw = z, pitch = y, roll = x
	double cy = cos(z * 0.5);
	double cp = cos(y * 0.5);
	double cr = cos(x * 0.5);
	double sy = sin(z * 0.5);
	double sp = sin(y * 0.5);
	double sr = sin(x * 0.5);

	Quaternion q;    
	q.r = cy * cp * cr + sy * sp * sr;
	q.i = cy * cp * sr - sy * sp * cr;
	q.j = sy * cp * sr + cy * sp * cr;
	q.k = sy * cp * cr - cy * sp * sr;

	return q;
}

void Mathe::AddScaledVector(Quaternion& q, const Vector3& v, double scale)
{
	Quaternion newq(0, v.x * scale, v.y * scale, v.z * scale);
	newq *= q;
	q.r += newq.r * (double)0.5;
	q.i += newq.i * (double)0.5;
	q.j += newq.j * (double)0.5;
	q.k += newq.k * (double)0.5;
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

	tensorWorld.Identity();

	tensorWorld.matrix[0] =
		t4 * rot.Get(0)
		+ t9 * rot.Get(1)
		+ t14 * rot.Get(2);
	tensorWorld.matrix[1] = 
		t4 * rot.Get(4)
		+ t9 * rot.Get(5)
		+ t14 * rot.Get(6);
	tensorWorld.matrix[2] = 
		t4 * rot.Get(8)
		+ t9 * rot.Get(9)
		+ t14 * rot.Get(10);

	tensorWorld.matrix[4] = 
		t28 * rot.Get(0)
		+ t33 * rot.Get(1)
		+ t38 * rot.Get(2);
	tensorWorld.matrix[5] =
		t28 * rot.Get(4)
		+ t33 * rot.Get(5)
		+ t38 * rot.Get(6);
	tensorWorld.matrix[6] =
		t28 * rot.Get(8)
		+ t33 * rot.Get(9)
		+ t38 * rot.Get(10);

	tensorWorld.matrix[8] =
		t52 * rot.Get(0)
		+ t57 * rot.Get(1)
		+ t62 * rot.Get(2);
	tensorWorld.matrix[9] =
		t52 * rot.Get(4)
		+ t57 * rot.Get(5)
		+ t62 * rot.Get(6);
	tensorWorld.matrix[10] =
		t52 * rot.Get(8)
		+ t57 * rot.Get(9)
		+ t62 * rot.Get(10);
}

std::array<float, 2> Mathe::SolveQuadraticFormula(float a, float b, float c, bool twoRealRoots)
{
	std::array<float, 2> x = { 0.0f, 0.0f };

	x[0] = (-b + sqrt(((b * b) - (4 * a * c)))) / (2 * a);

	if (twoRealRoots)
	{
		x[1] = (-b - sqrt(((b * b) - (4 * a * c)))) / (2 * a);
	}

	return x;
}

double Mathe::ToRadians(const double deg)
{
	return deg * PI / 180.0;
}

double Mathe::ToDegrees(const double rad)
{
	return rad * 180.0 / PI;
}

bool Mathe::IsVectorNAN(const Vector3& v)
{
	if (isnan(v.x) || isnan(v.y) || isnan(v.z)) return true;
	else return false;
}

