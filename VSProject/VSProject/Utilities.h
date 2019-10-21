#pragma once

#include <GL\glew.h>
#include <GL\freeglut.h>
#include <stdio.h>
#include <vector>

#include "mathe.h"

struct Colour
{
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float a = 1.0f;

	Colour(float _r, float _g, float _b, float _a = 1.0f)
		: r(_r), g(_g), b(_b), a(_a) {}
};
struct Vertex
{
	Vector3 position = Vector3();
	Colour colour;

	Vertex(double _x, double _y, double _z, Colour _c)
		: position(_x, _y, _z), colour(_c.r, _c.g, _c.b) {}
};
