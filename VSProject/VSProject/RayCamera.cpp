#include <SFML/Graphics.hpp>
#include "RayCamera.h"
#include <fstream>

RayCamera::RayCamera()
{
	//Will probably be a lot more primitives later on
	triPrimitives.reserve(200);

	//Set Z up matrices
	Quaternion rotation = Mathe::VectorToQuaternion(Vector3(0, 90, 0));
	Z_up.Identity();
	Mathe::Rotate(Z_up, rotation);
	Z_up_inverse = Z_up;
	Z_up_inverse.Inverse();
}

void RayCamera::AddPrimitive(std::vector<Tri>& tris, Matrix4* trans)
{
	triPrimitives.push_back(RayCameraPrimitive(tris, trans));

	if (triPrimitives.size() > 200)
		std::cout << "WARNING: Tri vector reserve hit in RayCamera" << std::endl;
}

void RayCamera::CastRays(const Vector3& camPos, const uint16_t width, const uint16_t height)
{	
	const unsigned int arrSize = width * height * 3;
	unsigned char* pixels = new unsigned char[arrSize];
	if (store)
	{
		std::cout << "Initialising pixel array...";
		for (unsigned int i = 0; i < arrSize; i++)
		{
			pixels[i] = (unsigned char)(0);
		}
		std::cout << " done." << std::endl;
	}
	unsigned int pixelIndex = 0;

	Global::shouldUpdate = false;
	cPosTemp = camPos;

	SetModelViewMatrix();

	// ---------
	// Loop through all pixels
	// ---------
	for (unsigned int col = 0; col < width; col++)
	{
		if (col % 10 == 0)
			std::cerr << "Progress... " << 100 * col / width << "%" << ", pixel index: " << pixelIndex << "/" << arrSize << std::endl;

		for (unsigned int row = 0; row < height; row++)
		{
			ray = GetRayAt(col, row, width, height, cPosTemp);

			Vector3 pathThroughput = Vector3(1, 1, 1);
			unsigned int samples = 1;
			Vector3 newColour = Vector3();

			newColour = ComputeRayHit(pathThroughput, ray.direction, ray.origin, newColour, samples);

			//newColour.DebugOutput();

			newColour = Vector3(
				powf(std::min((float)newColour.x / (float)samples, 1.0f), 1.0f / 2.2f),
				powf(std::min((float)newColour.y / (float)samples, 1.0f), 1.0f / 2.2f),
				powf(std::min((float)newColour.z / (float)samples, 1.0f), 1.0f / 2.2f)
				);

			//if (Mathe::IsVectorNAN(newColour) || newColour.x > 1 || newColour.y > 1 || newColour.z > 1)
			//	std::cout << std::endl;

			/*
			closestDistance = 1000.0f;

			for (unsigned int p = 0; p < triPrimitives.size(); p++)
			{
				for (unsigned int t = 0; t < triPrimitives[p].tris.size(); t++)
				{
					if (RayCast::TestTriangle(triPrimitives[p].tris[t], triPrimitives[p].transform, ray)
						&& ray.intersection1 < closestDistance)
					{
						closestDistance = ray.intersection1;
						closestTriangle = &triPrimitives[p].tris[t];

						newColour = triPrimitives[p].tris[t].normal;
						Matrix3 normalTransform = triPrimitives[p].transform.ToMatrix3();
						Mathe::Transform(newColour, normalTransform);
						newColour.Normalise();
						triPrimitives[p].tris[t].colour = newColour;
						

					}
				}
			}*/
			
			if (store)
			{
				pixels[pixelIndex++] = (unsigned char)(newColour.x * 255);
				pixels[pixelIndex++] = (unsigned char)(newColour.y * 255);
				pixels[pixelIndex++] = (unsigned char)(newColour.z * 255);
			}
		}
	}

	if (store) SavePixelsToFile(pixels, arrSize, width, height);
}

void RayCamera::DrawLatestRay()
{
	if (!draw) return;

	Ray r1 = GetRayAt(0, 0, 1280, 720, cPosTemp);
	r1.Draw(Colours::blue);
	//r1.direction.DebugOutput();
	Ray r2 = GetRayAt(1280, 720, 1280, 720, cPosTemp);
	r2.Draw(Colours::green);
	//r2.direction.DebugOutput();
	Ray r3 = GetRayAt(0, 720, 1280, 720, cPosTemp);
	r3.Draw(Colours::green);
	Ray r4 = GetRayAt(1280, 0, 1280, 720, cPosTemp);
	r4.Draw(Colours::blue);
	Ray r5 = GetRayAt(1280 / 2, 720 / 2, 1280, 720, cPosTemp);
	r5.Draw(Colours::red);
	//r5.direction.DebugOutput();

	glPushMatrix();
	glTranslated(cPosTemp.x, cPosTemp.y, cPosTemp.z);
	glutWireSphere(0.5, 4, 4);
	glPopMatrix();
	glFlush();
}

Ray RayCamera::GetRayAt(const int pX, const int pY, const float width, const float height, const Vector3& cameraPos)
{
	//https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays

	float aspectRatio = width / height; //assuming w > h

	float cameraX = (2.0f * (((float)pX + 0.5f) / (float)width) - 1.0f) * tanf(Mathe::ToRadians(80 / 2)) * aspectRatio; //fov = 80
	float cameraY = (1.0f - 2.0f * ((float)pY + 0.5f) / (float)height) * tanf(Mathe::ToRadians(80 / 2));// *1 / aspectRatio;

	Vector3 rayDir = Vector3(cameraX, cameraY, -1);

	Mathe::Transform(rayDir, modelViewMatrix);

	//rayDir -= cameraPos;
	rayDir = rayDir.Normalise();

	return Ray(cameraPos, rayDir);
}

void RayCamera::SetModelViewMatrix()
{
	GLfloat model[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, model);

	//Convert to double array
	double matrix[16];
	for (uint16_t i = 0; i < 16; i++)
		matrix[i] = double(model[i]);

	modelViewMatrix = Matrix4(matrix);
}

Vector3 RayCamera::ComputeRayHit(Vector3& pathThroughput, const Vector3& normal, const Vector3& point, Vector3& finalColour_pixel, unsigned int& samples)
{
	Matrix4* mat = nullptr;
	Tri* closestTriangle = nullptr;
	float closestDistance = 1000.0f;

	ray = Ray(point, normal);
	for (unsigned int p = 0; p < triPrimitives.size(); p++)
	{
		for (unsigned int t = 0; t < triPrimitives[p].tris.size(); t++)
		{
			if (RayCast::TestTriangle(triPrimitives[p].tris[t], triPrimitives[p].transform, ray)
				&& ray.intersection1 < closestDistance)
			{
				closestTriangle = &triPrimitives[p].tris[t];
				closestDistance = ray.intersection1;
				mat = &triPrimitives[p].transform;
			}
		}
	}

	Vector3 mult = Global::skyColour;
	if (closestTriangle != nullptr)
	{
		for (unsigned int i = 0; i < 2; i++)
		{
			samples++;

			Vector3 randHemisphere = RandomInHemisphere(RandomFloat(0, 1), RandomFloat(0, 1));
			Mathe::Transform(randHemisphere, Z_up_inverse);

			Vector3 n = closestTriangle->normal;
			Matrix3 normalTransform = mat->ToMatrix3();
			Mathe::Transform(n, normalTransform);
			n = n.Normalise();

			randHemisphere += n;
			randHemisphere = randHemisphere.Normalise();

			Vector3 col = Vector3(closestTriangle->colour.r, closestTriangle->colour.g, closestTriangle->colour.b);

			pathThroughput *= (col / Mathe::PI) * abs(n.ScalarProduct(randHemisphere)) / (1.0f / (2.0f * Mathe::PI));

			mult = ComputeRayHit(pathThroughput, randHemisphere, ray.IntersectionPoint(), finalColour_pixel, samples);

			finalColour_pixel += pathThroughput * mult;
		}
	}
	else
		finalColour_pixel += pathThroughput * mult;
	//if (finalColour_pixel != Vector3()) finalColour_pixel.DebugOutput();

	return finalColour_pixel;
}

void RayCamera::SavePixelsToFile(const unsigned char* pixels, const uint16_t arrSize, const uint16_t width, const uint16_t height)
{
	std::cout << "Saving pixel buffer to file... ";
	//sf::RenderWindow window(sf::VideoMode(width, height), "SFML");

	unsigned int pixelIndex = 0;

	sf::Image img;
	img.create(width, height);
	for (unsigned int col = 0; col < width; col++)
	{
		for (unsigned int row = 0; row < height; row++)
		{
			sf::Color c;
			c.r = pixels[pixelIndex++];
			c.g = pixels[pixelIndex++];
			c.b = pixels[pixelIndex++];
			c.a = 255;
			img.setPixel(col, row, c);
		}
	}

	img.saveToFile("PixelBufferSFML.bmp");

	std::cout << "done!" << std::endl;
}
