#pragma once
#include "Utilities.h"

struct Object
{
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

	void CreateObject(std::vector<Vertex> _vertices);
	void Animate();
	void Draw();

	Colour red = Colour(1, 0, 0);
	Colour green = Colour(0, 1, 0);
	Colour blue = Colour(0, 0, 1);

	Mathe* mathe = new Mathe();

private:

	std::vector<Object> objects;

protected:

};
