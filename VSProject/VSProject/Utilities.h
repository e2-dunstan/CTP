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
	Colour(const Vector3& _v) { *this = _v; }
	
	void operator=(const Vector3& _v)
	{
		r = (_v.x + 1.0) / 2.0; 
		g = (_v.y + 1.0) / 2.0; 
		b = (_v.z + 1.0) / 2.0;
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

static inline Vector3 RandomInHemisphere(const float u1, const float u2)
{
	float z = u1;
	float r = sqrtf(std::max(0.0f, 1.0f - z * z));
	float phi = 2.0f * Mathe::PI * u2;
	float x = r * cosf(phi);
	float y = r * sinf(phi);
	return Vector3(x, y, z);
}

static inline float RandomFloat(const float min, const float max)
{
	float random = (float)rand() / (float)RAND_MAX;
	float diff = max - min;
	random *= diff;
	return min + random;
}