#pragma once
#include "Utilities.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Camera.h"
#include "Material.h"

#include <iostream>


HittableList RandomScene()
{
    HittableList world;

    world.Add(std::make_shared<Sphere>(
        Vector3(0, -1000, 0), 1000, std::make_shared<Lambertian>(Vector3(0.5, 0.5, 0.5))));

    int i = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = RandomDouble();
            Vector3 center(a + 0.9 * RandomDouble(), 0.2, b + 0.9 * RandomDouble());
            if ((center - Vector3(4, 0.2, 0)).Magnitude() > 0.9) {
                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = RandomVectorNormalised() * RandomVectorNormalised();
                    world.Add(
                        std::make_shared<Sphere>(center, 0.2, std::make_shared<Lambertian>(albedo)));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Vector3(RandomDouble(0.5, 1), RandomDouble(0.5, 1), RandomDouble(0.5, 1));
                    auto fuzz = RandomDouble(0, 0.5);
                    world.Add(
                        std::make_shared<Sphere>(center, 0.2, std::make_shared<Metal>(albedo, fuzz)));
                }
                else {
                    // glass
                    world.Add(std::make_shared<Sphere>(center, 0.2, std::make_shared<Dielectric>(1.5)));
                }
            }
        }
    }

    world.Add(std::make_shared<Sphere>(Vector3(0, 1, 0), 1.0, std::make_shared<Dielectric>(1.5)));

    world.Add(
        std::make_shared<Sphere>(Vector3(-4, 1, 0), 1.0, std::make_shared<Lambertian>(Vector3(0.4, 0.2, 0.1))));

    world.Add(
        std::make_shared<Sphere>(Vector3(4, 1, 0), 1.0, std::make_shared<Metal>(Vector3(0.7, 0.6, 0.5), 0.0)));

    return world;
}

Vector3 RayColour(const Ray& r, const Hittable& world, int depth)
{
    if (depth <= 0) return Vector3();

    HitRecord rec;
    if (world.Hit(r, 0.001, infinity, rec))
    {
        Ray scattered;
        Vector3 attenuation;
        if (rec.matPtr->Scatter(r, rec, attenuation, scattered))
            return attenuation * RayColour(scattered, world, depth - 1);
        else return Vector3();
    }
    else
    {
        Vector3 unitDir = r.Direction().Normalise();
        auto t = 0.5 * (unitDir.y + 1.0);
        return Vector3(1, 1, 1) * (1.0 - t) + Vector3(0.5, 0.7, 1.0) * t;
    }
}

int main()
{
	const int imgWidth = 400;
	const int imgHeight = 200;
    const int samplesPerPixel = 300;
    const int maxDepth = 50;
    const auto aspectRatio = double(imgWidth) / imgHeight;

	std::cout << "P3\n" << imgWidth << ' ' << imgHeight << "\n255\n";

    auto world = RandomScene();

    Vector3 lookFrom(13, 2, 3);
    Vector3 lookAt(0, 0, 0);
    Vector3 viewUp(0, 1, 0);
    auto distanceToFocus = 10;
    auto aperture = 0.1;
    Camera cam(lookFrom, lookAt, viewUp, 20, aspectRatio, aperture, distanceToFocus);


    for (int j = imgHeight - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < imgWidth; ++i)
        {
            Vector3 colour(0, 0, 0);

            for (int s = 0; s < samplesPerPixel; ++s)
            {
                auto u = (i + RandomDouble()) / imgWidth;
                auto v = (j + RandomDouble()) / imgHeight;
                Ray r = cam.GetRay(u, v);
                colour += RayColour(r, world, maxDepth);
            }
            // Divide the color total by the number of samples and gamma-correct
            // for a gamma value of 2.0.
            auto scale = 1.0 / samplesPerPixel;
            auto r = sqrt(scale * colour.x);
            auto g = sqrt(scale * colour.y);
            auto b = sqrt(scale * colour.z);
            r = static_cast<int>(256 * Clamp(r, 0.0, 0.999));
            g = static_cast<int>(256 * Clamp(g, 0.0, 0.999));
            b = static_cast<int>(256 * Clamp(b, 0.0, 0.999));

            std::cout << r << ' ' << g << ' ' << b << '\n';
        }
    }
    std::cerr << "\nDone.\n";

    return 0;
}