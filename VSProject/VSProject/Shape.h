#pragma once
#include "Utilities.h"


struct Vertex
{
	Vector3 position = Vector3();

	Vertex(double _x, double _y, double _z)
		: position(_x, _y, _z) {}
};
struct Object
{
	//Shape::Type type;

	std::vector<Vertex> vertices;

	Vector3 position;
	Vector3 scale = Vector3(1, 1, 1);
	Vector3 rotation = Vector3();
};


class Shape
{
public:
	Shape() = default;
	~Shape() = default;

	enum Type{ TRIANGLE, CUBE };

	Object CreateObject(Type _type, std::vector<Vertex> _vertices);

	void Draw(std::vector<Vertex> _vertices);

private:

protected:

};
