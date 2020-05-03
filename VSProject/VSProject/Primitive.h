#pragma once
#include "RigidBody.h"
#include "BoundingVolume.h"
//#include "CollisionVolume.h"

enum class PrimitiveType
{
	BOX, SPHERE, PLANE, CAPSULE, CYLINDER, COMPLEX
};

class Primitive
{
public:
	Primitive() = default;
	~Primitive() = default;

	PrimitiveType type = PrimitiveType::BOX;
	const GLenum drawType = GL_TRIANGLES;

	virtual void Start() = 0;
	void Update();
	virtual void Draw() = 0;
	void DrawForVertices(std::vector<Vertex> vertices);
	void DrawForTris(std::vector<Tri> tris);

	virtual void CalculateInertiaTensor() = 0;

	//Physics data.
	RigidBody rigidbody = RigidBody();
	Vector3 startingVelocity = Vector3();

	//Transform data.
	Vector3 translation = Vector3();
	Quaternion orientation = Quaternion();
	Matrix4 transform = Matrix4();
	virtual void UpdateTransform() = 0;
	bool updateTransform = false;
	bool transformChanged = true;

	void GetOrientation(Quaternion* _orientation) const;
	Matrix3 GetOrientation(Matrix4 _matrix);

	void SetOrientation(const Quaternion& _orientation);
	void SetOrientation(const float r, const float i, const float j, const float k);

	bool enableCollision = true;
	bool colliding = false;

	BoundingVolume boundingVolume;

	bool initialised = false;

	bool freeze = false;
	bool isStatic = false;

	std::vector<Tri> tris = std::vector<Tri>();
protected:

	bool debugCollision = false;

	bool drawBoundingVolume = false;
};

class Box : public Primitive
{
public:
	Box() { tris.reserve(12); };
	Box(std::vector<Tri> t)
	{ 
		tris.reserve(12);
		tris = t;
		initialised = true;
	}
	~Box() = default;

	void Start() override;
	void CalculateInertiaTensor() override;
	void UpdateTransform() override;

	void Draw() override { DrawForTris(tris); }

	Vector3 scale = Vector3();
	BoxCV collisionVolume;

	float timeSinceOutput = 0;
};

class Sphere : public Primitive
{
public:
	Sphere() = default;
	Sphere(std::vector<Tri> t, float r)
	{
		tris = t;
		radius = r;
		initialised = true;
	}
	~Sphere() = default;

	void Start() override;
	void CalculateInertiaTensor() override;
	void UpdateTransform() override;

	void Draw() override { DrawForTris(tris); }

	float radius = 0;
	SphereCV collisionVolume;
};

class Plane : public Primitive
{
public:
	Plane() { tris.reserve(2); };
	Plane(std::vector<Tri> t)
	{
		tris.reserve(2);
		tris = t;
		initialised = true;
	}
	~Plane() = default;

	void Start() override {}
	void CalculateInertiaTensor() override {}
	void UpdateTransform() override;

	void Draw() override { DrawForTris(tris); }

	Vector3 scale = Vector3();
	Vector3 normal = Vector3(0, 1, 0);
	PlaneCV collisionVolume;
};