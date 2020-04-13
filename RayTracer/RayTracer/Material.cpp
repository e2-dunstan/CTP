#include "Material.h"

bool Dielectric::Scatter(const Ray& rayIn, const HitRecord& rec, Vector3& attenuation, Ray& scattered) const
{
	attenuation = Vector3(1, 1, 1);
	double etaI_over_etaT = rec.frontFace ? (1.0 / refractiveIndex) : refractiveIndex;
	Vector3 dir = rayIn.Direction().Normalise();

	double cosTheta = GetMinimum((dir * -1.0).ScalarProduct(rec.normal), 1.0);
	double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

	if (etaI_over_etaT * sinTheta > 1.0)
	{
		Vector3 reflected = Reflect(dir, rec.normal);
		scattered = Ray(rec.point, reflected);
		return true;
	}

	//double reflectProbability = Schlick(cosTheta, etaI_over_etaT);
	//Schlick:
	auto r0 = (1.0 - etaI_over_etaT) / (1.0 + etaI_over_etaT);
	r0 = r0 * r0;
	double reflectProbability = r0 + (1.0 - r0) * pow(1.0 - cosTheta, 5);


	if (RandomDouble() < reflectProbability)
	{
		Vector3 reflected = Reflect(dir, rec.normal);
		scattered = Ray(rec.point, reflected);
		return true;
	}
	
	Vector3 refracted = Refract(dir, rec.normal, etaI_over_etaT);
	scattered = Ray(rec.point, refracted);
	return true;
}

double Dielectric::Schlick(double cosine, double ri)
{
	auto r0 = (1.0 - refractiveIndex) / (1.0 + refractiveIndex);
	r0 = r0 * r0;
	return r0 + (1.0 - r0) * pow(1.0 - cosine, 5);
}
