#include "mathe.h"

#define PI 3.14159265

void Mathe::Transform(Vector3& vector, Matrix4& matrix)
{
	if (IsVectorNAN(vector)) return;

	Matrix4 vectorToMatrix;
	vectorToMatrix(0, 0) = vector.x;
	vectorToMatrix(1, 0) = vector.y;
	vectorToMatrix(2, 0) = vector.z;
	vectorToMatrix(3, 0) = 1.0;
	//double vectorToMatrix[4] =
	//{ vector.x, vector.y, vector.z, 1.0 };

	Matrix4 multiplied = matrix * vectorToMatrix;

	vector = Vector3(multiplied(0, 0), multiplied(1, 0), multiplied(2, 0));
}

void Mathe::Transform(Vector3& vector, Matrix3& matrix)
{
	if (IsVectorNAN(vector)) return;

	Matrix3 vectorToMatrix;
	vectorToMatrix(0, 0) = vector.x;
	vectorToMatrix(1, 0) = vector.y;
	vectorToMatrix(2, 0) = vector.z;
	//double vectorToMatrix[3] =
	//{ vector.x, vector.y, vector.z};

	Matrix3 multiplied = matrix * vectorToMatrix;

	vector = Vector3(multiplied(0, 0), multiplied(1, 0), multiplied(2, 0));
}

void Mathe::TransformTranspose(Vector3& vector, Matrix4& matrix)
{
	if (IsVectorNAN(vector)) return;

	Matrix4 vectorToMatrix;
	vectorToMatrix(0, 0) = vector.x;
	vectorToMatrix(1, 0) = vector.y;
	vectorToMatrix(2, 0) = vector.z;
	vectorToMatrix(3, 0) = 1.0;
	//double vectorToMatrix[4] =
	//{ vector.x, vector.y, vector.z, 1.0 };

	Matrix4 transpose = matrix.GetTranspose();
	Matrix4 multiplied = transpose * vectorToMatrix;

	vector = Vector3(multiplied(0, 0), multiplied(1, 0), multiplied(2, 0));
}

void Mathe::TransformTranspose(Vector3& vector, Matrix3& matrix)
{
	if (IsVectorNAN(vector)) return;

	Matrix3 vectorToMatrix;
	vectorToMatrix(0, 0) = vector.x;
	vectorToMatrix(1, 0) = vector.y;
	vectorToMatrix(2, 0) = vector.z;
	//double vectorToMatrix[3] =
	//{ vector.x, vector.y, vector.z };

	Matrix3 transpose = matrix.GetTranspose();
	Matrix3 multiplied = transpose * vectorToMatrix;

	vector = Vector3(multiplied(0, 0), multiplied(1, 0), multiplied(2, 0));
}

Vector3 Mathe::MatrixInverse(Matrix4& m, Vector3& v)
{
	Matrix4 transp = Matrix4(m.matrix);
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

void Mathe::Translate(Matrix4& m, double x, double y, double z)
{
	Matrix4 translation;
	translation(0, 3) = x;
	translation(1, 3) = y;
	translation(2, 3) = z;
	
	m = m * translation;
}

//pitch, yaw, roll
void Mathe::Rotate(Matrix4& m, double x, double y, double z)
{
	Matrix4 xMatrix;
	xMatrix(1, 1) = cos(x == 0 ? 0 : ToRadians(x));
	xMatrix(1, 2) = -sin(x == 0 ? 0 : ToRadians(x));
	xMatrix(2, 1) = sin(x == 0 ? 0 : ToRadians(x));
	xMatrix(2, 2) = cos(x == 0 ? 0 : ToRadians(x));

	Matrix4 yMatrix;
	yMatrix(0, 0) = cos(y == 0 ? 0 : ToRadians(y));
	yMatrix(0, 2) = sin(y == 0 ? 0 : ToRadians(y));
	yMatrix(2, 0) = -sin(y == 0 ? 0 : ToRadians(y));
	yMatrix(2, 2) = cos(y == 0 ? 0 : ToRadians(y));

	Matrix4 zMatrix;
	zMatrix(0, 0) = cos(z == 0 ? 0 : ToRadians(z));
	zMatrix(1, 0) = -sin(z == 0 ? 0 : ToRadians(z));
	zMatrix(0, 1) = sin(z == 0 ? 0 : ToRadians(z));
	zMatrix(1, 1) = cos(z == 0 ? 0 : ToRadians(z));

	Matrix4 rot = xMatrix * yMatrix * zMatrix;
	m = m * rot;
}

void Mathe::Rotate(Matrix4& m, const Quaternion& q)
{
	m.matrix[0] = 1 - (2 * q.j * q.j) - (2 * q.k * q.k);
	m.matrix[1] = (2 * q.i * q.j) - (2 * q.r * q.k);
	m.matrix[2] = (2 * q.i * q.k) + (2 * q.r * q.j);

	m.matrix[4] = (2 * q.i * q.j) + (2 * q.r * q.k);
	m.matrix[5] = 1 - (2 * q.i * q.i) - (2 * q.k * q.k);
	m.matrix[6] = (2 * q.j * q.k) - (2 * q.r * q.i);

	m.matrix[8] = (2 * q.i * q.k) - (2 * q.r * q.j);
	m.matrix[9] = (2 * q.j * q.k) + (2 * q.r * q.i);
	m.matrix[10] = 1 - (2 * q.i * q.i) - (2 * q.j * q.j);

	/*m(0, 0) = 1 - (2 * q.j * q.j) - (2 * q.k * q.k);
	m(0, 1) = (2 * q.i * q.j) - (2 * q.r * q.k);
	m(0, 2) = (2 * q.i * q.k) + (2 * q.r * q.j);

	m(1, 0) = (2 * q.i * q.j) + (2 * q.r * q.k);
	m(1, 1) = 1 - (2 * q.i * q.i) - (2 * q.k * q.k);
	m(1, 2) = (2 * q.j * q.k) - (2 * q.r * q.i);

	m(2, 0) = (2 * q.i * q.k) - (2 * q.r * q.j);
	m(2, 1) = (2 * q.j * q.k) + (2 * q.r * q.i);
	m(2, 2) = 1 - (2 * q.i * q.i) - (2 * q.j * q.j);*/
}

void Mathe::Rotate(Matrix3& m, const Quaternion& q)
{
	m.matrix[0] = 1 - (2 * q.j * q.j) - (2 * q.k * q.k);
	m.matrix[1] = (2 * q.i * q.j) - (2 * q.r * q.k);
	m.matrix[2] = (2 * q.i * q.k) + (2 * q.r * q.j);

	m.matrix[3] = (2 * q.i * q.j) + (2 * q.r * q.k);
	m.matrix[4] = 1 - (2 * q.i * q.i) - (2 * q.k * q.k);
	m.matrix[5] = (2 * q.j * q.k) - (2 * q.r * q.i);

	m.matrix[6] = (2 * q.i * q.k) - (2 * q.r * q.j);
	m.matrix[7] = (2 * q.j * q.k) + (2 * q.r * q.i);
	m.matrix[8] = 1 - (2 * q.i * q.i) - (2 * q.j * q.j);
}

void Mathe::Scale(Matrix4& m, double x, double y, double z)
{
	Matrix4 scale;
	scale(0, 0) = x;
	scale(1, 1) = y;
	scale(2, 2) = z;

	m = m * scale;
}

Vector3 Mathe::GetAxis(uint16_t i, Matrix4& mat)
{
	return Vector3(mat(0, i), mat(1, i), mat(2, i));
}
Vector3 Mathe::GetAxis(uint16_t i, Matrix3& mat)
{
	return Vector3(mat(0, i), mat(1, i), mat(2, i));
}

Quaternion Mathe::VectorToQuaternion(const Vector3& v)
{
	double x = ToRadians(v.x);
	double y = ToRadians(v.y);
	double z = ToRadians(v.z);

	// yaw (y) = z, pitch (p) = y, roll (r) = x
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

	q.Normalise();

	return q;
}

void Mathe::AddScaledVector(Quaternion& q, const Vector3& v, double scale, bool toRadians)
{
	Vector3 rad = v * scale;
	if (toRadians) 
		rad = Vector3(Mathe::ToRadians(v.x), Mathe::ToRadians(v.y), Mathe::ToRadians(v.z));

	Quaternion newq(0, rad.x, rad.y, rad.z);
	newq *= q;
	q.r += newq.r * 0.5;
	q.i += newq.i * 0.5;
	q.j += newq.j * 0.5;
	q.k += newq.k * 0.5;
	//q *= newq;

	//q.Normalise();
}

void Mathe::TransformInverseInertiaTensor(Matrix3& tensorWorld, const Matrix3& tensorLocal, const Matrix3& rot)
{
	//treating rot as a Mat4 and tensor mats as Mat3

	double t4 = rot.matrix[0] * tensorLocal.matrix[0]
		+ rot.matrix[1] * tensorLocal.matrix[3]
		+ rot.matrix[2] * tensorLocal.matrix[6];
	double t9 = rot.matrix[0] * tensorLocal.matrix[1]
		+ rot.matrix[1] * tensorLocal.matrix[4]
		+ rot.matrix[2] * tensorLocal.matrix[7];
	double t14 = rot.matrix[0] * tensorLocal.matrix[2]
		+ rot.matrix[1] * tensorLocal.matrix[5]
		+ rot.matrix[2] * tensorLocal.matrix[8];

	double t28 = rot.matrix[3] * tensorLocal.matrix[0]
		+ rot.matrix[4] * tensorLocal.matrix[3]
		+ rot.matrix[5] * tensorLocal.matrix[6];
	double t33 = rot.matrix[3] * tensorLocal.matrix[1]
		+ rot.matrix[4] * tensorLocal.matrix[4]
		+ rot.matrix[5] * tensorLocal.matrix[7];
	double t38 = rot.matrix[3] * tensorLocal.matrix[2]
		+ rot.matrix[4] * tensorLocal.matrix[5]
		+ rot.matrix[5] * tensorLocal.matrix[8];

	double t52 = rot.matrix[6] * tensorLocal.matrix[0]
		+ rot.matrix[7] * tensorLocal.matrix[3]
		+ rot.matrix[8] * tensorLocal.matrix[6];
	double t57 = rot.matrix[6] * tensorLocal.matrix[1]
		+ rot.matrix[7] * tensorLocal.matrix[4]
		+ rot.matrix[8] * tensorLocal.matrix[7];
	double t62 = rot.matrix[6] * tensorLocal.matrix[2]
		+ rot.matrix[7] * tensorLocal.matrix[5]
		+ rot.matrix[8] * tensorLocal.matrix[8];

	//tensorWorld.Identity();

	tensorWorld.matrix[0] =
		t4 * rot.matrix[0]
		+ t9 * rot.matrix[1]
		+ t14 * rot.matrix[2];
	tensorWorld.matrix[1] = 
		t4 * rot.matrix[3]
		+ t9 * rot.matrix[4]
		+ t14 * rot.matrix[5];
	tensorWorld.matrix[2] = 
		t4 * rot.matrix[6]
		+ t9 * rot.matrix[7]
		+ t14 * rot.matrix[8];

	tensorWorld.matrix[3] = 
		t28 * rot.matrix[0]
		+ t33 * rot.matrix[1]
		+ t38 * rot.matrix[2];
	tensorWorld.matrix[4] =
		t28 * rot.matrix[3]
		+ t33 * rot.matrix[4]
		+ t38 * rot.matrix[5];
	tensorWorld.matrix[5] =
		t28 * rot.matrix[6]
		+ t33 * rot.matrix[7]
		+ t38 * rot.matrix[8];

	tensorWorld.matrix[6] =
		t52 * rot.matrix[0]
		+ t57 * rot.matrix[1]
		+ t62 * rot.matrix[2];
	tensorWorld.matrix[7] =
		t52 * rot.matrix[3]
		+ t57 * rot.matrix[4]
		+ t62 * rot.matrix[5];
	tensorWorld.matrix[8] =
		t52 * rot.matrix[6]
		+ t57 * rot.matrix[7]
		+ t62 * rot.matrix[8];
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

