#pragma once

#include <GL\glew.h>
#include <GL\freeglut.h>
#include <stdio.h>
#include <vector>

#include "mathe.h"

struct Colour
{
	GLfloat r = 0.0f;
	GLfloat g = 0.0f;
	GLfloat b = 0.0f;
	GLfloat a = 1.0f;

	Colour(GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a = 1.0f)
		: r(_r), g(_g), b(_b), a(_a) {}
};
struct Vertex
{
	Vector3 position = Vector3();
	Vector3 normal = Vector3();
	Colour colour;

	Vertex(double _x, double _y, double _z, Colour _c, Vector3 _n)
		: position(_x, _y, _z), colour(_c.r, _c.g, _c.b), normal(_n) {}
};

class ColourPresets
{
public:
	ColourPresets() = default;
	~ColourPresets() = default;

	Colour red = Colour(1.0, 0.0, 0.0);
	Colour green = Colour(0.0, 1.0, 0.0);
	Colour blue = Colour(0.0, 0.0, 1.0);
	Colour yellow = Colour(1.0, 1.0, 0.0);
	Colour cyan = Colour(0.0, 1.0, 1.0);
	Colour magenta = Colour(1.0, 0.0, 1.0);
};
