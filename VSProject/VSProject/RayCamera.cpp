#include "RayCamera.h"
#include <fstream>
#include <thread>
#include <mutex>


void RayCamera::AddPrimitive(std::vector<Tri>& tris, Matrix4& trans, float sphereRadius, const Vector3& sphereOrigin)
{
	triPrimitives.push_back(RayCameraPrimitive(tris, trans, sphereRadius, sphereOrigin));
}

void RayCamera::CastRays(const Vector3& camPos, const uint16_t width, const uint16_t height)
{
	Global::shouldUpdate = false;
	cPosTemp = camPos;
	SetModelViewMatrix();

	const unsigned int arrSize = width * height * 3;
	sf::Uint8* pixels = new sf::Uint8[arrSize];
	Vector3* pixelsVec = new Vector3[width * height];

	std::cout << "Initialising pixel array...";
	memset(pixelsVec, 0, width * height * sizeof(Vector3));
	memset(pixels, 0, width * height * 3 * sizeof(sf::Uint8)); //sizeof return 1
	std::cout << " done." << std::endl;

	// ---------
	// Begin threads
	// ---------
	/*uint16_t supportedThreads = std::thread::hardware_concurrency();
	std::cerr << supportedThreads << " physical supported threads (not definite)" << std::endl;
	
	std::vector<std::thread> threads;
	const uint16_t threadedSize = height / supportedThreads;
	for (uint16_t thread = 0; thread < supportedThreads; thread++)
	{
		threads.push_back(std::thread(CastRaysInPixelRange,
			thread * threadedSize, 0, (thread + 1) * threadedSize, width,
			width, height, pixelsVec, thread));
		//CastRaysInPixelRange(thread * threadedSize, 0, (thread + 1) * threadedSize, width,
		//	width, height, pixelsVec, thread);
	}
	unsigned int pixelIndex = 0;
	if (store)
	{
		for (uint16_t col = 0; col < width; col++)
			for (uint16_t row = 0; row < height; row++)
			{
				int actualIndex = row * width + col;
				pixels[pixelIndex++] = (sf::Uint8)(pixelsVec[actualIndex].x);
				pixels[pixelIndex++] = (sf::Uint8)(pixelsVec[actualIndex].y);
				pixels[pixelIndex++] = (sf::Uint8)(pixelsVec[actualIndex].z);
			}
	}*/


	// ---------
	// Loop through all pixels
	// ---------
	uint16_t samples = 32 * (picCount + 1);
	Vector3 newColour = Vector3();
	unsigned int pixelIndex = 0;

	Ray ray = Ray(Vector3(), Vector3());

	for (uint16_t col = 0; col < width; col++)
	{
		if (col % 100 == 0)
			std::cerr << "Progress... " << 100 * col / width << "%" << " Latest colour: " << 
			(int)newColour.x << ", " << (int)newColour.y << ", " << (int)newColour.z << std::endl;
	
		for (uint16_t row = 0; row < height; row++)
		{
			int actualIndex = row * width + col;
	
			for (uint16_t s = 0; s < samples; s++)
			{
				//integrating over area of pixel
				ray = GetRayAt(col + RandomFloat(0.0f, 1.0f), row + RandomFloat(0.0f, 1.0f), width, height, cPosTemp);
				Vector3 pathThroughput = Vector3(1, 1, 1);
	
				newColour = ComputeRayHit(pathThroughput, ray.direction, ray.origin, 0, ray);
	
				//if (debugRaysCounter >= 3) return;
	
				pixelsVec[actualIndex] += newColour;
			}
	
			newColour = Vector3(
				powf(std::min(pixelsVec[actualIndex].x / (float)samples, 1.0f), 1.0f / 2.2f),
				powf(std::min(pixelsVec[actualIndex].y / (float)samples, 1.0f), 1.0f / 2.2f),
				powf(std::min(pixelsVec[actualIndex].z / (float)samples, 1.0f), 1.0f / 2.2f)
			);
			newColour *= 255.0f;
	
			pixels[pixelIndex++] = (sf::Uint8)(newColour.x);
			pixels[pixelIndex++] = (sf::Uint8)(newColour.y);
			pixels[pixelIndex++] = (sf::Uint8)(newColour.z);
		}
	}

	SavePixelsToFile(pixels, arrSize, width, height);

	delete[] pixels;
	delete[] pixelsVec;
	
	triPrimitives.clear();
}

void RayCamera::DrawLatestRay()
{
	//debugRays[0].Draw(Colours::magenta);
	//debugRays[1].Draw(Colours::magenta);
	//debugRays[2].Draw(Colours::magenta);
	if (!draw)
		return;

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
	r5.direction.DebugOutput();

	glPushMatrix();
	glTranslated(cPosTemp.x, cPosTemp.y, cPosTemp.z);
	glutWireSphere(0.5, 4, 4);
	glPopMatrix();
	glFlush();
}

bool RayCamera::ShouldDrawNextFrame()
{
	if (timeElapsed < timeBetweenCaptures)
	{
		if (!skipFrame)
			timeElapsed += Global::deltaTime;
		else skipFrame = false;
		return false;
	}
	return true;
}


/*void RayCamera::CastRaysInPixelRange(const uint16_t startRow, const uint16_t startCol, 
	const uint16_t endRow, const uint16_t endCol,
	const uint16_t screenWidth, const uint16_t screenHeight, 
	Vector3* thisPixelVec, const uint16_t threadCount, Ray& ray)
{
	uint16_t samples = 5;
	Vector3 newColour = Vector3();
	int actualIndex = 0; 

	//Vector3* pixelsVec = new Vector3[(endRow - startRow) * (endCol - startCol)];
	//if (store) memset(pixelsVec, 0, (endRow - startRow) * (endCol - startCol) * sizeof(Vector3));

	for (uint16_t col = startCol; col < endCol; col++)
	{
		if (col % 10 == 0)
			std::cerr << "Thread " << threadCount << " progress... " << 100 * (col - startCol) / (endCol - startCol) << "%"
			<< " Latest colour: " << (int)newColour.x << ", " << (int)newColour.y << ", " << (int)newColour.z << std::endl;

		for (uint16_t row = startRow; row < endRow; row++)
		{
			actualIndex = row * screenWidth + col;
			for (uint16_t s = 0; s < samples; s++)
			{
				//integrating over area of pixel
				ray = GetRayAt(col + RandomFloat(0.0f, 1.0f), row + RandomFloat(0.0f, 1.0f), screenWidth, screenHeight, cPosTemp);
				Vector3 pathThroughput = Vector3(1, 1, 1);

				newColour = ComputeRayHit(pathThroughput, ray.direction, ray.origin, 0, ray);

				thisPixelVec[actualIndex] += newColour;
			}

			newColour = Vector3(
				powf(std::min(thisPixelVec[actualIndex].x / (float)samples, 1.0f), 1.0f / 2.2f),
				powf(std::min(thisPixelVec[actualIndex].y / (float)samples, 1.0f), 1.0f / 2.2f),
				powf(std::min(thisPixelVec[actualIndex].z / (float)samples, 1.0f), 1.0f / 2.2f)
			);
			//newColour *= 255.0f;
			thisPixelVec[actualIndex] = newColour * 255.0f;
			//if (store)
			//{
			//	thisPixels[pixelIndex++] = (sf::Uint8)(newColour.x);
			//	thisPixels[pixelIndex++] = (sf::Uint8)(newColour.y);
			//	thisPixels[pixelIndex++] = (sf::Uint8)(newColour.z);
			//}
		}
	}
	//delete[] pixelsVec;
}*/

Ray RayCamera::GetRayAt(const int pX, const int pY, const float width, const float height, const Vector3& cameraPos)
{
	//https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays

	float aspectRatio = width / height; //assuming w > h

	float cameraX = (2.0f * (((float)pX + 0.5f) / (float)width) - 1.0f) * tanf((float)Mathe::ToRadians(80 / 2)) * aspectRatio;
	float cameraY = (1.0f - 2.0f * ((float)pY + 0.5f) / (float)height) * tanf((float)Mathe::ToRadians(80 / 2));

	Vector3 rayDir = Vector3(cameraX, cameraY, -1);

	Mathe::Transform(rayDir, modelViewMatrix);

	rayDir = rayDir.Normalise();

	return Ray(cameraPos, rayDir);
}

void RayCamera::SetModelViewMatrix()
{
	GLfloat model[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, model);

	//Convert to double array
	float matrix[16];
	for (uint16_t i = 0; i < 16; i++)
		matrix[i] = model[i];

	modelViewMatrix = Matrix4(matrix);
}

Vector3 RayCamera::ComputeRayHit(Vector3& pathThroughput, const Vector3& normal, const Vector3& point, uint16_t pathLength, Ray& ray)
{
	//Matrix4 mat;
	Tri* closestTriangle = nullptr;
	Ray bestRay;
	float radius = 0;
	Vector3 origin = Vector3();
	bestRay.intersection1 = 1000;

	//if (debugRaysCounter < 3) debugRays[debugRaysCounter++] = ray;
	ray = Ray(point, normal);
	for (uint16_t p = 0; p < triPrimitives.size(); p++)
	{	
		//if (triPrimitives[p].radius != 0)
		//{
		//	if (RayCast::TestSphere(triPrimitives[p].origin, triPrimitives[p].radius, ray)
		//		&& ray.intersection1 <= bestRay.intersection1)
		//	{
		//		closestTriangle = &triPrimitives[p].tris[0]; //doesnt matter for spheres
		//		radius = triPrimitives[p].radius;
		//		//origin = triPrimitives[p].origin;
		//		bestRay = ray;
		//	}
		//}
		//else
		//{
		for (uint16_t t = 0; t < triPrimitives[p].tris.size(); t++)
		{
			//If the normal is facing the same dir as the ray, it shouldn't hit.
			//if ((triPrimitives[p].tris[t].normal->ScalarProduct(ray.direction) > 0 && triPrimitives[p].radius == 0)
			//	|| (triPrimitives[p].tris[t].normal[0].ScalarProduct(ray.direction) > 0
			//		&& triPrimitives[p].tris[t].normal[1].ScalarProduct(ray.direction) > 0
			//		&& triPrimitives[p].tris[t].normal[2].ScalarProduct(ray.direction) > 0
			//		&& triPrimitives[p].radius != 0))
			//	continue;

			if (RayCast::TestTriangle(triPrimitives[p].tris[t], ray, nullptr)
				&& ray.intersection1 <= bestRay.intersection1)
			{
				closestTriangle = &triPrimitives[p].tris[t];
				radius = triPrimitives[p].radius;
				origin = triPrimitives[p].origin;
				bestRay = ray;
				//mat = triPrimitives[p].transform;
			}
		}
		//}
	}

	if (closestTriangle == nullptr)
	{
		return pathThroughput * Vector3(1, 1, 1);
		//return pathThroughput * Global::skyColour;
	}
	else
	{
		Vector3 n = radius == 0 ? closestTriangle->normal : ((bestRay.IntersectionPoint() - origin) / radius);//GetSphereNormal(bestRay.IntersectionPoint(), *closestTriangle);

		Vector3 temp(1, 0, 0);
		if (n.ScalarProduct(temp) > 0.0001f) temp = Vector3(0, 1, 0);
		//Vector product ensures it's the same space as the normal
		Vector3 u = n.VectorProduct(temp).Normalise();
		Vector3 v = n.VectorProduct(u).Normalise();

		//matrix where z is the normal
		float matVals[9] =
		{
			u.x, v.x, n.x,
			u.y, v.y, n.y,
			u.z, v.z, n.z
		};
		Matrix3 normalMat = Matrix3(matVals);

		float pdf;
		//Z up space
		Vector3 randHemisphere = CosineSampleHemisphere(RandomFloat(0.0f, 1.0f), RandomFloat(0.0f, 1.0f), pdf);
		//World space
		Mathe::Transform(randHemisphere, normalMat);
		randHemisphere = randHemisphere.Normalise();

		Vector3 BRDF = Vector3(closestTriangle->colour.r, closestTriangle->colour.g, closestTriangle->colour.b) / Mathe::PI;
		float cosTheta = abs(n.ScalarProduct(randHemisphere));
	
		pathThroughput *= BRDF * cosTheta / pdf;

		return ComputeRayHit(pathThroughput, randHemisphere, bestRay.IntersectionPoint() + (n * 0.0001f), pathLength++, ray);
	}
}

void RayCamera::SavePixelsToFile(const sf::Uint8* pixels, const uint16_t arrSize, const uint16_t width, const uint16_t height)
{
	//std::cout << "Saving pixel buffer to file... ";

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

	img.saveToFile("Captures/Capture" + std::to_string(picCount++) + ".bmp");

	std::cout << "Done! Samples: " << std::to_string(32 * picCount) << std::endl;
	timeElapsed = 0;
	skipFrame = true;
	Global::shouldUpdate = true;
}

Vector3 RayCamera::GetSphereNormal(const Vector3& p, const Tri& t)
{
	float v0 = (p - t.positions[0]).Magnitude();
	float v1 = (p - t.positions[1]).Magnitude();
	float v2 = (p - t.positions[2]).Magnitude();
	float sum = v0 + v1 + v2;

	v0 /= sum;
	v1 /= sum;
	v2 /= sum;

	//Vector3 n = (t.normal[0] * v0) + (t.normal[1] * v1) + (t.normal[2] * v2);

	return Vector3();
}
