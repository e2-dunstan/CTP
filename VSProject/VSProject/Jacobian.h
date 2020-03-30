#pragma once

// This is essentially a generic Matrix class, but with specific functions for Jacobians

//for non penetration, C (constraint) = penetration

struct Rigidbody;

struct SparseRow
{
	Vector3* b1_pos = nullptr;
	Vector3* b1_rot = nullptr;
	Vector3* b2_pos = nullptr;
	Vector3* b2_rot = nullptr;
};

struct MapRow
{
	//Rigidbody* rb1 = nullptr;
	//RigidBody* rb2 = nullptr;
};

struct Jacobian
{
public:
	//Jacobian() = default;
	Jacobian(uint16_t numConstraints)
	{
		if (numConstraints != constraintCount)
		{
			constraintCount = numConstraints;
			sparse = new SparseRow[constraintCount];
			map = new MapRow[constraintCount];
		}
	}
	//Jacobian(Vector3 point, Vector3 targetVel, Vector3 orig) //friction
	//{
	//	j = new Vector3[2];
	//	j[0] = targetVel;
	//	j[1] = (point - orig).VectorProduct(targetVel);
	//}
	//Jacobian(Vector3 normal, Vector3 point, Vector3 b1Orig, Vector3 b2Orig) //penetration
	//{
	//	j = new Vector3[4];
	//	j[0] = normal * -1.0;
	//	j[1] = (point - b1Orig).VectorProduct(normal) * -1.0;
	//	j[2] = normal;
	//	j[3] = (point - b2Orig).VectorProduct(normal);
	//}
	~Jacobian()
	{
		sparse = nullptr;
		delete sparse;
		map = nullptr;
		delete map;
	}

	//double* Multiply(const Vector3& vec)
	//{
	//	double ret[4] =
	//	{
	//		j[0].x * vec.x + j[0].y * vec.y + j[0].z * vec.z,
	//		j[1].x * vec.x + j[1].y * vec.y + j[1].z * vec.z,
	//		j[2].x * vec.x + j[2].y * vec.y + j[2].z * vec.z,
	//		j[3].x * vec.x + j[3].y * vec.y + j[3].z * vec.z
	//	};
	//	return ret;
	//}

	void AssignSparse(uint16_t index, Vector3* b1Pos, Vector3* b1Rot, Vector3* b2Pos, Vector3* b2Rot)
	{
		sparse[index].b1_pos = b1Pos;
		sparse[index].b1_rot = b1Rot;
		sparse[index].b2_pos = b2Pos;
		sparse[index].b2_rot = b2Rot;
	}
	//ideally should just be an index
	//void AssignMap(uint16_t index, RigidBody* rb1, RigidBody* rb2)
	//{
	//	map[index].rb1 = rb1;
	//	map[index].rb2 = rb2;
	//}

	//assign 5 by default
	uint16_t constraintCount = 5;

	SparseRow* sparse = new SparseRow[constraintCount];
	MapRow* map = new MapRow[constraintCount];
};