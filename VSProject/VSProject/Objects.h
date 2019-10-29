#pragma once
#include "RigidBody.h"

class Objects
{
public:

	Objects() = default;
	~Objects() = default;

	void Create(RigidBody::Type type, Vector3 scale, Vector3 translation, Vector3 rotation);
	void Animate();
	void Draw();

	std::unique_ptr<Shapes> shapes = std::make_unique<Shapes>();

private:

	void UpdateTransforms(RigidBody& rb);
	void CreateBoundingVolume(RigidBody& obj);

	std::unique_ptr<Mathe> mathe = std::make_unique<Mathe>();

	std::vector<RigidBody> rbs;

	GLenum GetDrawType(RigidBody::Type objectType);

	bool drawBoundingVolumes = true;

protected:

};
