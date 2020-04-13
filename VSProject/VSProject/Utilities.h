#pragma once
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <stdio.h>
#include <vector>

#include "Global.h"
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
	
	void operator=(const Vector3& _v)
	{
		r = abs(_v.x); 
		g = abs(_v.y); 
		b = abs(_v.z);
	}

	bool operator==(const Colour& _c) const { return (r == _c.r && g == _c.g && b == _c.b && a == _c.a); }
	bool operator!=(const Colour& _c) const { return !(r == _c.r && g == _c.g && b == _c.b && a == _c.a); }
	bool operator==(const Colour& _c) { return (r == _c.r && g == _c.g && b == _c.b && a == _c.a); }
	bool operator!=(const Colour& _c) { return !(r == _c.r && g == _c.g && b == _c.b && a == _c.a); }
};
struct Vertex
{
	Vector3 position = Vector3();
	Vector3 normal = Vector3();
	Colour colour = Colour(1, 1, 1);

	//Vertex constructor.
	Vertex() = default;
	Vertex(double _x, double _y, double _z, Colour _c, Vector3 _n)
		: position(_x, _y, _z), colour(_c.r, _c.g, _c.b), normal(_n) {}
	~Vertex() = default;
};
struct Tri
{
	Tri() = default;
	Tri(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 n, Colour c)
	{
		positions[0] = p1;
		positions[1] = p2;
		positions[2] = p3;
		normal = n;
		colour = c;
	}
	~Tri() = default;

	Vector3 positions[3];
	Vector3 normal = Vector3();
	Colour colour = Colour(1, 1, 1);
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

/*static inline Vector3 ScreenToWorldSpace(int x, int y, int width, int height, Vector3 camPos, Quaternion camRot, bool output = false)
{
	//Top left = 0, 0
	//Bottom right = width, height

	Vector3 screenPos((double)x / (double)width, (double)y / (double)height, 0);
	Matrix4 camMat;
	Mathe::Rotate(camMat, camRot);
	Mathe::Translate(camMat, camPos.x, camPos.y, camPos.z);
	Mathe::Transform(screenPos, camMat);

	if (output) screenPos.DebugOutput();

	return screenPos;
}*/