#include "Sphere.h"

bool Sphere::Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const
{
    Vector3 oc = r.Origin() - centre;

    //Quadratic eq
    auto a = r.Direction().SquaredMagnitude();
    auto b = oc.ScalarProduct(r.Direction()); //half b
    auto c = oc.SquaredMagnitude() - (radius * radius);

    auto discriminant = b * b - a * c;

    if (discriminant > 0)
    {
        auto root = sqrt(discriminant);
        auto temp = (-b - root) / a;
        if (temp < tMax && temp > tMin)
        {
            rec.t = temp;
            rec.point = r.At(rec.t);
            Vector3 normal = (rec.point - centre) / radius;
            rec.SetFaceNormal(r, normal);
            rec.matPtr = matPtr;
            return true;
        }
        temp = (-b + root) / a;
        if (temp < tMax && temp > tMin)
        {
            rec.t = temp;
            rec.point = r.At(rec.t);
            Vector3 normal = (rec.point - centre) / radius;
            rec.SetFaceNormal(r, normal);
            rec.matPtr = matPtr;
            return true;
        }
    }
	return false;
}
