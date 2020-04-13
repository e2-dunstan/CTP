#pragma once
#include <cstdlib>
#include <limits>
#include <memory>
#include <functional>
#include <random>

#include "Ray.h"
#include "Vector.h"

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double DegreesToRadians(double degrees) { return degrees * pi / 180; }

inline double GetMinimum(double a, double b) { return a <= b ? a : b; }
inline double GetMaximum(double a, double b) { return a >= b ? a : b; }

inline double RandomDouble()
{
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	static std::function<double()> randGenerator = std::bind(distribution, generator);
	return randGenerator();
}
inline double RandomDouble(const double min, const double max) { return min + (max - min) * RandomDouble(); }

inline Vector3 RandomVectorNormalised()
{
	auto a = RandomDouble(0, 2 * pi);
	auto z = RandomDouble(-1, 1);
	auto r = sqrt(1 - z * z);
	return Vector3(r * cos(a), r * sin(a), z);
}

inline Vector3 RandomInSphere()
{
	while (true)
	{
		Vector3 p = RandomVectorNormalised();
		if (p.SquaredMagnitude() >= 1) continue;
		else return p;
	}
}

inline Vector3 RandomInHemiSphere(const Vector3& normal)
{
	Vector3 inSphere = RandomInSphere();
	if (inSphere.ScalarProduct(normal) > 0.0) return inSphere;
	else return inSphere * -1.0;
}

inline Vector3 RandomInDisk()
{
	while (true)
	{
		Vector3 p(RandomDouble(-1, 1), RandomDouble(-1, 1), 0);
		if (p.SquaredMagnitude() >= 1) continue;
		else return p;
	}
}

inline Vector3 Reflect(const Vector3& v, const Vector3& n) { return v - n * 2.0 * v.ScalarProduct(n); }

inline Vector3 Refract(const Vector3& uv, const Vector3& n, double etaI_over_etaT)
{
	auto cosTheta = (uv * -1.0).ScalarProduct(n);
	Vector3 rOutParallel = (uv + n * cosTheta) * etaI_over_etaT;
	Vector3 rOutPerp = n * -sqrt(1.0 - rOutParallel.SquaredMagnitude());
	return rOutParallel + rOutPerp;
}

inline double Clamp(double x, double min, double max) { return x < min ? min : (x > max ? max : x); }