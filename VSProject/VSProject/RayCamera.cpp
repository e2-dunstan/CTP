#include "RayCamera.h"

RayCamera::RayCamera()
{
	//Will probably be a lot more tris later on
	triPrimitives.reserve(100);
}

void RayCamera::AddPrimitive(std::vector<Tri>& tris, Matrix4* trans)
{
	triPrimitives.push_back(RayCameraPrimitive(tris, trans));

	if (triPrimitives.size() > 100)
		std::cout << "WARNING: Tri vector reserve hit in RayCamera" << std::endl;
}

void RayCamera::CastRays(const Vector3& camPos, const uint16_t width, const uint16_t height, const uint16_t iterationStep)
{	
	Tri* closestTriangle = nullptr;
	float closestDistance = 1000.0f;

	Global::shouldUpdate = false;
	cPosTemp = camPos;

	SetModelViewMatrix();

	for (uint16_t w = 0; w < width; w += iterationStep)
	{
		std::cerr << "Progress... " << 100.0 * w / (double)width << "%" << std::endl;
		for (uint16_t h = 0; h < height; h += iterationStep)
		{
			ray = GetRayAt(w, h, width, height, cPosTemp);
			for (uint16_t p = 0; p < triPrimitives.size(); p++)
			{
				for (uint16_t t = 0; t <  triPrimitives[p].tris.size(); t++)
				{
					if (RayCast::TestTriangle(triPrimitives[p].tris[t], triPrimitives[p].transform, ray))
					{
						Vector3 newColour = triPrimitives[p].tris[t].normal;
						Matrix3 normalTransform = triPrimitives[p].transform.ToMatrix3();
						Mathe::Transform(newColour, normalTransform);
						triPrimitives[p].tris[t].colour = newColour.Normalise();

						if (ray.intersection1 < closestDistance)
						{
							closestDistance = ray.intersection1;
							closestTriangle = &triPrimitives[p].tris[t];
						}
					}
				}
			}
		}
	}
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

Ray RayCamera::GetRayAt(int pX, int pY, const float width, const float height, const Vector3& cameraPos)
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
	double mat[16];
	for (uint16_t i = 0; i < 16; i++)
		mat[i] = double(model[i]);

	modelViewMatrix = Matrix4(mat);
}
