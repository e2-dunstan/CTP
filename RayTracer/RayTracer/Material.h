#pragma once
#include "Utilities.h"
#include "Hittable.h"

class Material
{
public:
	Material() = default;
	~Material() = default;

	virtual bool Scatter(const Ray& rayIn, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const = 0;
};

//Diffuse
class Lambertian : public Material
{
public:
	Lambertian(const Vector3& a) : albedo(a) {}

	bool Scatter(const Ray& rayIn, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const override
	{
		Vector3 scatterDir = rec.normal + RandomVectorNormalised();
		scattered = Ray(rec.point, scatterDir);
		attenuation = albedo;
		return true;
	}

	Vector3 albedo;
};

class Metal : public Material
{
public:
	Metal(const Vector3& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}
	~Metal() = default;

	bool Scatter(const Ray& rayIn, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const override
	{
		Vector3 reflected = Reflect(rayIn.Direction().Normalise(), rec.normal);
		scattered = Ray(rec.point, reflected + RandomInSphere() * fuzz);
		attenuation = albedo;
		return scattered.Direction().ScalarProduct(rec.normal) > 0;
	}

	Vector3 albedo;
	double fuzz;
};

//Glass, air, water, diamond, etc.
//Sphere with negative radius makes a bubble
class Dielectric : public Material
{
public:
	Dielectric(double ri) : refractiveIndex(ri) {}
	~Dielectric() = default;

	bool Scatter(const Ray& rayIn, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const override;
		
	double Schlick(double cosine, double ri);

	double refractiveIndex;
};