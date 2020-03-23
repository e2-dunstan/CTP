#pragma once

// This is essentially a generic Matrix class, but with specific functions for Jacobians

//for non penetration, C (constraint) = penetration

struct Jacobian
{
public:
	Jacobian() = default;
	Jacobian(Vector3 point, Vector3 targetVel, Vector3 orig) //friction
	{
		j = new Vector3[2];
		j[0] = targetVel;
		j[1] = (point - orig).VectorProduct(targetVel);
	}
	Jacobian(Vector3 normal, Vector3 point, Vector3 b1Orig, Vector3 b2Orig) //penetration
	{
		j = new Vector3[4];
		j[0] = normal * -1.0;
		j[1] = (point - b1Orig).VectorProduct(normal) * -1.0;
		j[2] = normal;
		j[3] = (point - b2Orig).VectorProduct(normal);
	}
	~Jacobian()
	{
		j = nullptr;
		delete j;
	}

	double* Multiply(const Vector3& vec)
	{
		double ret[4] =
		{
			j[0].x * vec.x + j[0].y * vec.y + j[0].z * vec.z,
			j[1].x * vec.x + j[1].y * vec.y + j[1].z * vec.z,
			j[2].x * vec.x + j[2].y * vec.y + j[2].z * vec.z,
			j[3].x * vec.x + j[3].y * vec.y + j[3].z * vec.z
		};

		return ret;
	}

	Vector3* j = new Vector3[4]; //max size it will be
};