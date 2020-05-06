#pragma once
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <stdio.h>
#include <vector>
#include <ctime>

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
		r = (GLfloat)(_v.x + 1.0f) / (GLfloat)2.0f; 
		g = (GLfloat)(_v.y + 1.0f) / (GLfloat)2.0f; 
		b = (GLfloat)(_v.z + 1.0f) / (GLfloat)2.0f;
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
	Vertex(float _x, float _y, float _z, Colour _c, Vector3 _n)
		: position(_x, _y, _z), colour(_c.r, _c.g, _c.b), normal(_n) {}
	~Vertex() = default;
};
struct Tri
{
	Tri() = default;
	Tri(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 n, Colour c)//, const float uv[6])
	{
		positions[0] = p1;
		positions[1] = p2;
		positions[2] = p3;
		normal = n;
		colour = c;
		//for(uint16_t i = 0; i < 6; i++)
		//	uvs[i] = uv[i];
	}
	~Tri() = default;

	Vector3 positions[3];
	//float uvs[6];
	Vector3 normal = Vector3();
	Colour colour = Colour(1, 1, 1);
};

namespace Colours
{
	const Colour white = Colour(1.0f, 1.0f, 1.0f);
	const Colour offWhite = Colour(0.95f, 0.95f, 0.95f);
	const Colour black = Colour(0.0f, 0.0f, 0.0f);
	const Colour nearBlack = Colour(0.05f, 0.05f, 0.05f);
	const Colour red = Colour(1.0f, 0.0f, 0.0f);
	const Colour deepRed = Colour(0.85f, 0.1f, 0.1f);
	const Colour green = Colour(0.0f, 1.0f, 0.0f);
	const Colour grass = Colour(0.64f, 0.88f, 0.71f);
	const Colour blue = Colour(0.0f, 0.0f, 1.0f);
	const Colour yellow = Colour(1.0f, 1.0f, 0.0f);
	const Colour cyan = Colour(0.0f, 1.0f, 1.0f);
	const Colour lightCyan = Colour(0.65f, 1.0f, 1.0f);
	const Colour lightestCyan = Colour(0.9f, 1.0f, 1.0f);
	const Colour magenta = Colour(1.0f, 0.0f, 1.0f);
	const Colour pink = Colour(1.0f, 0.5f, 0.7f);
	const Colour grey = Colour(0.7f, 0.7f, 0.7f);
	const Colour darkGrey = Colour(0.4f, 0.4f, 0.4f);
	const Colour brown = Colour(0.7f, 0.4f, 0.28f);
};

static inline Vector3 CosineSampleHemisphere(const float u1, const float u2, float& pdf)
{
	//pdf = probability density function

	const float r = sqrt(u1);
	const float theta = 2 * Mathe::PI * u2;

	const float x = r * cos(theta);
	const float y = r * sin(theta);
	const float z = sqrt(1.0f - u1);
	pdf = z / Mathe::PI;

	return Vector3(x, y, z);
}

static inline float RandomFloat(const float min, const float max)
{
	float random = (float)(std::rand() + std::rand()) / (float)(RAND_MAX * 2.0f);

	float diff = max - min;
	random *= diff;

	return min + random;
}