#pragma once

#include <GL\glew.h>
#include <GL\freeglut.h>
#include <stdio.h>
#include <vector>

#include "mathe.h"
using namespace Mathe;

struct Colour
{
	GLfloat r = 0.0f;
	GLfloat g = 0.0f;
	GLfloat b = 0.0f;
	GLfloat a = 1.0f;

	//Colour constructor.
	Colour(GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a = 1.0f)
		: r(_r), g(_g), b(_b), a(_a) {}
	
	bool operator==(const Colour& _c) const { return (r == _c.r && g == _c.g && b == _c.b && a == _c.a); }
	bool operator!=(const Colour& _c) const { return !(r == _c.r && g == _c.g && b == _c.b && a == _c.a); }
	bool operator==(const Colour& _c) { return (r == _c.r && g == _c.g && b == _c.b && a == _c.a); }
	bool operator!=(const Colour& _c) { return !(r == _c.r && g == _c.g && b == _c.b && a == _c.a); }
};
struct Vertex
{
	Vector3 position = Vector3();
	Vector3 normal = Vector3();
	Colour colour;

	//Vertex constructor.
	Vertex(double _x, double _y, double _z, Colour _c, Vector3 _n)
		: position(_x, _y, _z), colour(_c.r, _c.g, _c.b), normal(_n) {}
};

namespace Colours
{
	const Colour white = Colour(1.0, 1.0, 1.0);
	const Colour black = Colour(0.0, 0.0, 0.0);
	const Colour red = Colour(1.0, 0.0, 0.0);
	const Colour green = Colour(0.0, 1.0, 0.0);
	const Colour blue = Colour(0.0, 0.0, 1.0);
	const Colour yellow = Colour(1.0, 1.0, 0.0);
	const Colour cyan = Colour(0.0, 1.0, 1.0);
	const Colour magenta = Colour(1.0, 0.0, 1.0);
};
