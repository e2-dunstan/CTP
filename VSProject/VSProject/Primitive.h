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
	virtual ~Primitive() = default;

	PrimitiveType type = PrimitiveType::BOX;
	GLenum drawType = GL_QUADS;

	virtual void Start() = 0;
	void Update();
	void Draw();

	virtual void CalculateInertiaTensor() = 0;

	//Physics data.
	RigidBody rigidbody = RigidBody();
	Vector3 startingVelocity = Vector3();

	//Transform data.
	Vector3 translation = Vector3();
	//Vector3 rotation = Vector3();
	Quaternion orientation = Quaternion();
	Matrix4 transform = Matrix4();
	virtual void UpdateTransform() = 0;
	bool updateTransform = false;
	bool transformChanged = true;

	void GetOrientation(Quaternion* _orientation) const;
	Quaternion GetOrientation() const;
	Matrix3 GetOrientation(Matrix4 _matrix);
	Matrix3 GetOrientation(double _matrix[16]);

	void SetOrientation(const Quaternion& _orientation);
	void SetOrientation(const double r, const double i, const double j, const double k);

	std::vector<Vertex> vertices = std::vector<Vertex>();

	bool enableCollision = true;
	bool colliding = false;

	BoundingVolume boundingVolume;
	//std::unique_ptr<CollisionVolume> collisionVolume = std::make_unique<CollisionVolume>();

	bool initialised = false;

protected:
	Vector3 previousPosition = Vector3();
	bool debugCollision = false;
	bool freeze = false;
};

class Box : public Primitive
{
public:
	Box() = default;
	Box(std::vector<Vertex> v)
	{ 
		vertices = v;
		//collisionVolume = std::make_unique<BoxCV>(boxCollisionVolume);
		initialised = true;
	}
	~Box() = default;

	void Start() override;
	void CalculateInertiaTensor() override;
	void UpdateTransform() override;

	Vector3 scale = Vector3();
	BoxCV collisionVolume;
};

class Sphere : public Primitive
{
public:
	Sphere() = default;
	Sphere(std::vector<Vertex> v, float r)
	{
		vertices = v;
		radius = r;
		//collisionVolume = std::make_unique<SphereCV>(sphereCollisionVolume);
		initialised = true;
	}
	~Sphere() = default;

	void Start() override;
	void CalculateInertiaTensor() override;
	void UpdateTransform() override;

	float radius = 0;
	SphereCV collisionVolume;
};

class Plane : public Primitive
{
public:
	Plane() = default;
	Plane(std::vector<Vertex> v)
	{
		vertices = v;
		//collisionVolume = std::make_unique<PlaneCV>(planeCollisionVolume);
		initialised = true;
	}
	~Plane() = default;


	void Start() override { drawType = GL_QUADS; }
	void CalculateInertiaTensor() override {}
	void UpdateTransform() override;

	Vector3 scale = Vector3();
	PlaneCV collisionVolume;
};

/*struct OldPrimitive
{
	OldPrimitive() = default;
	OldPrimitive(std::vector<Vertex> v, float r = 0) : vertices(v), radius(r) { initialised = true; }
	~OldPrimitive() = default;

	enum class PrimitiveType
	{
		BOX, SPHERE, PLANE, CAPSULE, CYLINDER, COMPLEX
	};
	PrimitiveType type = PrimitiveType::BOX;

	void Start();
	void Update();
	void Draw();

	void CalculateInertiaTensor();

	//Physics data.
	RigidBody rigidbody = RigidBody();
	
	//Transform data.
	Vector3 translation = Vector3();
	//Vector3 rotation = Vector3();
	Quaternion orientation = Quaternion();
	Vector3 scale = Vector3();
	Matrix4 transform = Matrix4();
	void UpdateTransform();
	bool updateTransform = false;
	bool transformChanged = true;

	void GetOrientation(Quaternion* _orientation) const;
	Quaternion GetOrientation() const;
	Matrix3 GetOrientation(Matrix4 _matrix);
	Matrix3 GetOrientation(double _matrix[16]);

	void SetOrientation(const Quaternion& _orientation);
	void SetOrientation(const double r, const double i, const double j, const double k);

	std::vector<Vertex> vertices = std::vector<Vertex>();

	bool enableCollision = true;
	bool colliding = false;

	BoundingVolume boundingVolume;
	CollisionVolume collisionVolume;

	float radius = 0; //if sphere, capsule or cylinder
	Vector3 upDir = Vector3(0, 1, 0); //if capsule or cylinder
	Matrix3 upDirMat = Matrix3(); //if cylinder

	void Tween(float speed, const Vector3& direction, float distance);
	void SetTweenOrigin();

	Vector3 tweenOrigin = Vector3();
	Vector3 tweenMax = Vector3();
	bool tweenMaxSet = false;
	bool moveTowards = true;

	bool initialised = false;

private:
	Vector3 previousPosition = Vector3();
	GLenum GetDrawType(Primitive::PrimitiveType objectType);

	bool debugCollision = false;

	bool freeze = false;
};*/