#pragma once
#include "Utilities.h"
#include "Shapes.h"

enum ObjectType
{
	CUBE
};

struct Object
{
	ObjectType type = CUBE;

	std::vector<Vertex> vertices;

	Vector3 translation = Vector3();
	Vector3 rotation = Vector3();
	Vector3 scale = Vector3();
	//Matrix transform = Matrix(4, 4);

	Object(std::vector<Vertex> _v)
		: vertices(_v) {}
};


class Objects
{
public:

	Objects() = default;
	~Objects() = default;

	void Create(ObjectType type, Vector3 scale, Vector3 translation, Vector3 rotation);
	void Animate();
	void Draw();

	Shapes* shapes = new Shapes();

private:

	Mathe* mathe = new Mathe();

	std::vector<Object> objects;

protected:

};
