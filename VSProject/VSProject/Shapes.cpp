#include "Shapes.h"

using namespace Shapes;

std::vector<Vertex> ShapeVertices::GetCubeVertices(const Colour& _colour)
{
	// 4 * 6 = 24 vertices for quads
	std::vector<Vertex> vertices;

	//front
	Colour faceColour = GetColour(Colours::red, _colour);
	Vector3 normal = Vector3(0, 0, 1);
	vertices.push_back(Vertex(-1.0, 1.0, 1.0,	faceColour, normal));
	vertices.push_back(Vertex(-1.0, -1.0, 1.0,	faceColour, normal));
	vertices.push_back(Vertex(1.0, -1.0, 1.0,	faceColour, normal));
	vertices.push_back(Vertex(1.0, 1.0, 1.0,	faceColour, normal));
	
	//back
	faceColour = GetColour(Colours::green, _colour);
	normal = Vector3(0, 0, -1);
	vertices.push_back(Vertex(1.0, 1.0, -1.0,	faceColour, normal));
	vertices.push_back(Vertex(1.0, -1.0, -1.0,	faceColour, normal));
	vertices.push_back(Vertex(-1.0, -1.0, -1.0, faceColour, normal));
	vertices.push_back(Vertex(-1.0, 1.0, -1.0,	faceColour, normal));

	//right
	faceColour = GetColour(Colours::blue, _colour);
	normal = Vector3(1, 0, 0);
	vertices.push_back(Vertex(1.0, 1.0, 1.0,	faceColour, normal));
	vertices.push_back(Vertex(1.0, -1.0, 1.0,	faceColour, normal));
	vertices.push_back(Vertex(1.0, -1.0, -1.0,	faceColour, normal));
	vertices.push_back(Vertex(1.0, 1.0, -1.0,	faceColour, normal));

	//left
	faceColour = GetColour(Colours::yellow, _colour);
	normal = Vector3(-1, 0, 0);
	vertices.push_back(Vertex(-1.0, 1.0, -1.0,	faceColour, normal));
	vertices.push_back(Vertex(-1.0, -1.0, -1.0, faceColour, normal));
	vertices.push_back(Vertex(-1.0, -1.0, 1.0,	faceColour, normal));
	vertices.push_back(Vertex(-1.0, 1.0, 1.0,	faceColour, normal));

	//top
	faceColour = GetColour(Colours::cyan, _colour);
	normal = Vector3(0, 1, 0);
	vertices.push_back(Vertex(-1.0, 1.0, -1.0,	faceColour, normal));
	vertices.push_back(Vertex(-1.0, 1.0, 1.0,	faceColour, normal));
	vertices.push_back(Vertex(1.0, 1.0, 1.0,	faceColour, normal));
	vertices.push_back(Vertex(1.0, 1.0, -1.0,	faceColour, normal));

	//bottom
	faceColour = GetColour(Colours::magenta, _colour);
	normal = Vector3(0, -1, 0);
	vertices.push_back(Vertex(-1.0, -1.0, 1.0,	faceColour, normal));
	vertices.push_back(Vertex(-1.0, -1.0, -1.0, faceColour, normal));
	vertices.push_back(Vertex(1.0, -1.0, -1.0,	faceColour, normal));
	vertices.push_back(Vertex(1.0, -1.0, 1.0,	faceColour, normal));

	return vertices;
	vertices.clear();
}

std::vector<Tri> Shapes::ShapeVertices::GetCubeTris(const Colour& _colour)
{
	// 7 * 6 = 42 vertices for tris
	// 2 * 6 = 12 tris with shared vertices
	/*const float uv_left[6] =
	{
		0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f
	};
	const float uv_right[6] =
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};*/

	const Vector3 vertices[8] =
	{
		Vector3(-1.0, 1.0, 1.0),	//0
		Vector3(-1.0, 1.0, -1.0),	//1
		Vector3(-1.0, -1.0, -1.0),	//2
		Vector3(-1.0, -1.0, 1.0),	//3
		Vector3(1.0, 1.0, 1.0),		//4
		Vector3(1.0, 1.0, -1.0),	//5
		Vector3(1.0, -1.0, -1.0),	//6
		Vector3(1.0, -1.0, 1.0),	//7
	};

	std::vector<Tri> tris;

	//front
	Colour faceColour = GetColour(Colours::white, _colour);
	Vector3 normal = Vector3(0, 0, 1);
	tris.push_back(Tri(vertices[0], vertices[3], vertices[7], normal, faceColour));//, uv_left));
	tris.push_back(Tri(vertices[0], vertices[7], vertices[4], normal, faceColour));//, uv_right));

	//back
	faceColour = GetColour(Colours::white, _colour);
	normal = Vector3(0, 0, -1);
	tris.push_back(Tri(vertices[5], vertices[6], vertices[2], normal, faceColour));//uv_left));
	tris.push_back(Tri(vertices[5], vertices[2], vertices[1], normal, faceColour));//uv_right));

	//right
	faceColour = GetColour(Colours::white, _colour);
	normal = Vector3(1, 0, 0);
	tris.push_back(Tri(vertices[4], vertices[7], vertices[6], normal, faceColour));// uv_left));
	tris.push_back(Tri(vertices[4], vertices[6], vertices[5], normal, faceColour));// uv_right));

	//left
	faceColour = GetColour(Colours::white, _colour);
	normal = Vector3(-1, 0, 0);
	tris.push_back(Tri(vertices[1], vertices[2], vertices[3], normal, faceColour));// uv_left));
	tris.push_back(Tri(vertices[1], vertices[3], vertices[0], normal, faceColour));// uv_right));

	//top
	faceColour = GetColour(Colours::white, _colour);
	normal = Vector3(0, 1, 0);
	tris.push_back(Tri(vertices[1], vertices[0], vertices[4], normal, faceColour));// uv_left));
	tris.push_back(Tri(vertices[1], vertices[4], vertices[5], normal, faceColour));// uv_right));

	//bottom
	faceColour = GetColour(Colours::white, _colour);
	normal = Vector3(0, -1, 0);
	tris.push_back(Tri(vertices[3], vertices[2], vertices[6], normal, faceColour));//uv_left));
	tris.push_back(Tri(vertices[3], vertices[6], vertices[7], normal, faceColour));//uv_right));

	return tris;
}

std::vector<Tri> ShapeVertices::GetPlaneTris(const Colour& _colour)
{
	const float uv_left[6] =
	{
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f
	};
	const float uv_right[6] =
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};

	std::vector<Tri> tris;
	tris.reserve(2);

	tris.push_back(Tri(Vector3(-1.0, 0.0, -1.0), Vector3(-1.0, 0.0, 1.0), Vector3(1.0, 0.0, 1.0),
						Vector3(0, 1, 0), _colour));// , uv_left));
	tris.push_back(Tri(Vector3(1.0, 0.0, 1.0), Vector3(1.0, 0.0, -1.0), Vector3(-1.0, 0.0, -1.0),
						Vector3(0, 1, 0), _colour));// , uv_right));

	return tris;
}

//std::vector<Tri> Shapes::ShapeVertices::GetSphereTris(float radius, const Colour& _colour, const uint16_t subdivisionCount)
//{
//
//
//		/*
//		//Ico/geosphere
//		//http://www.songho.ca/opengl/gl_sphere.html
//
//		// -- Compute vertices -- //
//
//		const float H_ANGLE = Mathe::PI / 180.0f * 72.0f;
//		const float V_ANGLE = atanf(1.0f / 2.0f);
//
//		uint16_t i1, i2;
//		float z, xy; //coords
//		float hAngle1 = -Mathe::PI / 2.0f - H_ANGLE / 2.0f;
//		float hAngle2 = -Mathe::PI / 2.0f;
//
//		std::vector<float> vertices(12 * 3);
//
//		vertices[0] = 0;
//		vertices[1] = 0;
//		vertices[2] = radius;
//		for (uint16_t i = 1; i <= 5; i++)
//		{
//			i1 = i * 3;
//			i2 = (i + 5) * 3;
//			z = radius * sinf(V_ANGLE); //elevation
//			xy = radius * cosf(V_ANGLE); //length on xy plane
//
//			vertices[i1] = xy * cosf(hAngle1);      // x
//			vertices[i2] = xy * cosf(hAngle2);
//			vertices[i1 + 1] = xy * sinf(hAngle1);  // y
//			vertices[i2 + 1] = xy * sinf(hAngle2);
//			vertices[i1 + 2] = z;                   // z
//			vertices[i2 + 2] = -z;
//
//			hAngle1 += H_ANGLE;
//			hAngle2 += H_ANGLE;
//		}
//		i1 = 11 * 3;
//		vertices[i1] = 0;
//		vertices[i1 + 1] = 0;
//		vertices[i1 + 2] = -radius;
//
//		// -- Build triangles -- //
//
//		const float* v0, * v1, * v2, * v3, * v4, * v11;          // vertex positions
//		Vector3 normal;                                         // face normal
//		unsigned int index = 0;
//
//		std::vector<uint16_t> indices;
//		std::vector<Tri> tris;
//
//		// compute and add 20 tiangles of icosahedron first
//		v0 = &vertices[0];       // 1st vertex
//		v11 = &vertices[11 * 3]; // 12th vertex
//		for (int i = 1; i <= 5; ++i)
//		{
//			// 4 vertices in the 2nd row
//			v1 = &vertices[i * 3];
//			if (i < 5)
//				v2 = &vertices[(i + 1) * 3];
//			else
//				v2 = &vertices[3];
//
//			v3 = &vertices[(i + 5) * 3];
//			if ((i + 5) < 10)
//				v4 = &vertices[(i + 6) * 3];
//			else
//				v4 = &vertices[6 * 3];
//
//			ComputeFaceNormal(v0, v1, v2, normal);
//			tris.push_back(Tri(Vector3(v0[0], v0[1], v0[2]), Vector3(v1[0], v1[1], v1[2]), Vector3(v2[0], v2[1], v2[2]), normal, _colour));
//			indices.push_back(index);
//			indices.push_back(index + 1);
//			indices.push_back(index + 2);
//
//			ComputeFaceNormal(v1, v3, v2, normal);
//			tris.push_back(Tri(Vector3(v1[0], v1[1], v1[2]), Vector3(v3[0], v3[1], v3[2]), Vector3(v2[0], v2[1], v2[2]), normal, _colour));
//			indices.push_back(index + 3);
//			indices.push_back(index + 4);
//			indices.push_back(index + 5);
//
//			ComputeFaceNormal(v2, v3, v4, normal);
//			tris.push_back(Tri(Vector3(v2[0], v2[1], v2[2]), Vector3(v3[0], v3[1], v3[2]), Vector3(v4[0], v4[1], v4[2]), normal, _colour));
//			indices.push_back(index + 6);
//			indices.push_back(index + 7);
//			indices.push_back(index + 8);
//
//			ComputeFaceNormal(v3, v11, v4, normal);
//			tris.push_back(Tri(Vector3(v3[0], v3[1], v3[2]), Vector3(v11[0], v11[1], v11[2]), Vector3(v4[0], v4[1], v4[2]), normal, _colour));
//			indices.push_back(index + 9);
//			indices.push_back(index + 10);
//			indices.push_back(index + 11);
//
//			// next index
//			index += 12;
//		}
//
//		// subdivide icosahedron
//		unsigned int indexCount;
//		//const float* v1, * v2, * v3;          // ptr to original vertices of a triangle
//		float newV1[3], newV2[3], newV3[3];  // new vertex positions
//		//Vector3 normal;                    // new face normal
//		index = 0;             // new index value
//
//
//		// iteration of num subdivisions
//		for (uint16_t i = 1; i <= subdivisionCount; ++i)
//		{
//			index = 0;
//			indexCount = indices.size();
//			for (uint16_t j = 0; j < indexCount; j += 3)
//			{
//				// get 3 vertices of a triangle
//				v1 = &vertices[indices[j] * 3];
//				v2 = &vertices[indices[j + 1] * 3];
//				v3 = &vertices[indices[j + 2] * 3];
//
//				// get 3 new vertices by spliting half on each edge
//				ComputeHalfVertex(v1, v2, radius, newV1);
//				ComputeHalfVertex(v2, v3, radius, newV2);
//				ComputeHalfVertex(v1, v3, radius, newV3);
//
//				Vector3 v[3] = { Vector3(v1[0], v1[1], v1[2]), Vector3(v2[0], v2[1], v2[2]), Vector3(v3[0], v3[1], v3[2]) };
//				Vector3 newV[3] = { Vector3(newV1[0], newV1[1], newV1[2]), Vector3(newV2[0], newV2[1], newV2[2]),Vector3(newV3[0], newV3[1], newV3[2]) };
//
//				// add 4 new triangles
//				ComputeFaceNormal(v1, newV1, newV3, normal);
//				tris.push_back(Tri(v[0], newV[0], newV[2], normal, _colour));
//				indices.push_back(index);
//				indices.push_back(index + 1);
//				indices.push_back(index + 2);
//
//				ComputeFaceNormal(newV1, v2, newV2, normal);
//				tris.push_back(Tri(newV[0], v[1], newV[1], normal, _colour));
//				indices.push_back(index + 3);
//				indices.push_back(index + 4);
//				indices.push_back(index + 5);
//
//				ComputeFaceNormal(newV1, newV2, newV3, normal);
//				tris.push_back(Tri(newV[0], newV[1], newV[2], normal, _colour));
//				indices.push_back(index + 6);
//				indices.push_back(index + 7);
//				indices.push_back(index + 8);
//
//				ComputeFaceNormal(newV3, newV2, v3, normal);
//				tris.push_back(Tri(newV[2], newV[1], v[2], normal, _colour));
//				indices.push_back(index + 9);
//				indices.push_back(index + 10);
//				indices.push_back(index + 11);
//
//				// next index
//				index += 12;
//			}
//		}
//		return tris;*/
//}

std::vector<Tri> ShapeVertices::GetSphereVertices(float radius, const Colour& _colour, uint16_t sectorCount, uint16_t stackCount)
{
	/*const float uv_left[6] =
	{
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f
	};
	const float uv_right[6] =
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};*/

	Colour sphereColour = Colour(1, 1, 1);
	if (_colour != Colours::white) sphereColour = _colour;

	std::vector<Tri> vertices;

	std::vector<Vector3> positions;
	std::vector<Vector3> normals;
	//std::vector<unsigned> indices;

	Vector3 pos;
	float xy;
	float lengthInverse = 1.0f / radius;
	Vector3 normal = Vector3(lengthInverse, lengthInverse, lengthInverse);

	float sectorStep = 2.0f * (float)Mathe::PI / sectorCount;
	float stackStep = (float)Mathe::PI / stackCount;
	float sectorAngle;
	float stackAngle;

	for (uint16_t i = 0; i <= stackCount; ++i)
	{
		stackAngle = Mathe::PI / 2.0f - ((float)i * stackStep);
		xy = radius * cosf(stackAngle);
		pos.z = (double)radius * (double)sin(stackAngle);

		for (uint16_t j = 0; j <= sectorCount; ++j)
		{
			sectorAngle = j * sectorStep;

			pos.x = (double)xy * (double)cos(sectorAngle);
			pos.y = (double)xy * (double)sin(sectorAngle);

			normal = pos * lengthInverse;

			positions.push_back(pos);
			normals.push_back(normal);
			//vertices.push_back(Vertex(pos.x, pos.y, pos.z, Colour(cylinderColour), normal));
		}
	}

	uint16_t k1;
	uint16_t k2;
	for (uint16_t i = 0; i < stackCount; i++)
	{
		k1 = i * (sectorCount + 1);
		k2 = k1 + sectorCount + 1;

		for (uint16_t j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			if (i != 0)
			{
				Vector3 n = (normals[k1] + normals[k2] + normals[k1 + 1]) / 3.0;
				vertices.push_back(Tri(positions[k1], positions[k2], positions[k1 + 1], n, sphereColour));// , uv_left));
				//indices.push_back(k1);
				//indices.push_back(k2);
				//indices.push_back(k1 + 1);
			}
			if (i != (stackCount - 1))
			{
				Vector3 n = (normals[k1 + 1] + normals[k2] + normals[k2 + 1]) / 3.0;
				vertices.push_back(Tri(positions[k1 + 1], positions[k2], positions[k2 + 1], n, sphereColour));// , uv_right));
				//indices.push_back(k1 + 1);
				//indices.push_back(k2);
				//indices.push_back(k2 + 1);
			}
		}
	}

	//for (uint16_t in = 0; in < indices.size() - 3; in += 3)
	//{
	//	vertices.push_back(Vertex(positions[indices[in]].x, positions[indices[in]].y, positions[indices[in]].z, sphereColour, normals[indices[in]]));
	//}
	return vertices;
}

std::vector<Vertex> Shapes::ShapeVertices::GetCapsuleVertices(float radius, float straight, const Colour& _colour, int sectorCount, int stackCount)
{
	Colour capsuleColour = Colour(1, 1, 1);
	if (_colour != Colours::white) capsuleColour = _colour;

	if (stackCount % 2 == 0)
	{
		std::cout << "WARNING: stack count for capsules must be odd! Adding 1 to stack count." << std::endl;
		stackCount += 1;
	}

	std::vector<Vertex> vertices;
	vertices.clear();

	std::vector<Vector3> positions;
	std::vector<Vector3> normals;
	std::vector<unsigned> indices;

	Vector3 pos;
	float xy;
	float lengthInverse = 1.0f / radius;
	Vector3 normal = Vector3(lengthInverse, lengthInverse, lengthInverse);

	float PI = 3.141592653589f;
	float sectorStep = 2 * PI / sectorCount;
	float stackStep = PI / stackCount;
	float sectorAngle;
	float stackAngle;

	bool topSection = false;

	for (int i = 0; i <= stackCount; ++i)
	{
		topSection = i > stackCount / 2;
		float capsuleOffset = topSection ? -straight / 2 : straight / 2;

		stackAngle = PI / 2 - (i * stackStep);
		xy = radius * cosf(stackAngle);
		pos.z = (double)radius * (double)sin(stackAngle) + capsuleOffset;

		for (int j = 0; j <= sectorCount; ++j)
		{
			sectorAngle = j * sectorStep;

			pos.x = (double)xy * (double)cos(sectorAngle);
			pos.y = (double)xy * (double)sin(sectorAngle);

			normal = pos * lengthInverse;

			positions.push_back(pos);
			normals.push_back(normal);
			//vertices.push_back(Vertex(pos.x, pos.y, pos.z, Colour(cylinderColour), normal));
		}
	}

	unsigned index1;
	unsigned index2;
	for (unsigned i = 0; i < (unsigned)stackCount; i++)
	{
		index1 = i * (sectorCount + 1);
		index2 = index1 + sectorCount + 1;

		for (unsigned j = 0; j < (unsigned)sectorCount; ++j, ++index1, ++index2)
		{
			if (i != 0)
			{
				indices.push_back(index1);
				indices.push_back(index2);
				indices.push_back(index1 + 1);
			}
			if (i != (stackCount - 1))
			{
				indices.push_back(index1 + 1);
				indices.push_back(index2);
				indices.push_back(index2 + 1);
			}
		}
	}

	for (unsigned in = 0; in < indices.size(); in++)
	{
		unsigned index = indices[in];
		Vector3 indexPos = positions[index];
		Vector3 indexNor = normals[index];
		vertices.push_back(Vertex(indexPos.x, indexPos.y, indexPos.z, capsuleColour, indexNor));
	}
	return vertices;
}

std::vector<Vertex> Shapes::ShapeVertices::GetCylinderVertices(float radius, float straight, const Colour& _colour, int sectorCount)
{
	Colour cylinderColour = Colour(1, 1, 1);
	if (_colour != Colours::white) cylinderColour = _colour;

	std::vector<Vertex> vertices;
	vertices.clear();
	float PI = 3.141592653589f;
	float halfLength = straight / 2;

	//Top
	for (int i = 0; i < sectorCount; i++)
	{
		float theta = ((float)i / sectorCount) * 2 * PI;
		float nextTheta = ((float)(i + 1) / sectorCount) * 2 * PI;

		vertices.push_back(Vertex(0, halfLength, 0, cylinderColour, Vector3(0, 1, 0)));
		vertices.push_back(Vertex((double)radius * (double)sin(theta), halfLength, (double)radius * (double)cos(theta),
			cylinderColour, Vector3(cos(theta), 0.0, sin(theta))));
		if (i != sectorCount - 1) vertices.push_back(Vertex(0, halfLength, 0, cylinderColour, Vector3(0, 1, 0)));
		vertices.push_back(Vertex((double)radius * (double)sin(nextTheta), halfLength, (double)radius * (double)cos(nextTheta),
			cylinderColour, Vector3(cos(nextTheta), 0.0, sin(nextTheta))));
	}
	//Sides
	for (int i = 0; i < sectorCount; i++)
	{
		float theta = ((float)i / sectorCount) * 2 * PI;
		float nextTheta = ((float)(i + 1) / sectorCount) * 2 * PI;

		vertices.push_back(Vertex((double)radius * (double)sin(theta), -halfLength, (double)radius * (double)cos(theta),
			cylinderColour, Vector3(-cos(theta), 0.0, -sin(theta))));		
		vertices.push_back(Vertex((double)radius * (double)sin(theta), halfLength, (double)radius * (double)cos(theta),
			cylinderColour, Vector3(cos(theta), 0.0, sin(theta))));
		vertices.push_back(Vertex((double)radius * (double)sin(nextTheta), -halfLength, (double)radius * (double)cos(nextTheta),
				cylinderColour, Vector3(-cos(nextTheta), 0.0, -sin(nextTheta))));
		vertices.push_back(Vertex((double)radius * (double)sin(theta), halfLength, (double)radius * (double)cos(theta),
			cylinderColour, Vector3(cos(theta), 0.0, sin(theta))));
	}
	//Bottom
	for (int i = 0; i < sectorCount; i++)
	{
		float theta = ((float)i / sectorCount) * 2 * PI;
		float nextTheta = ((float)(i + 1) / sectorCount) * 2 * PI;

		vertices.push_back(Vertex((double)radius * (double)sin(theta), -halfLength, (double)radius * (double)cos(theta),
			cylinderColour, Vector3(cos(theta), 0.0, sin(theta))));
		if (i != 0) vertices.push_back(Vertex(0, -halfLength, 0, cylinderColour, Vector3(0, -1, 0)));
		vertices.push_back(Vertex((double)radius * (double)sin(nextTheta), -halfLength, (double)radius * (double)cos(nextTheta),
			cylinderColour, Vector3(cos(nextTheta), 0.0, sin(nextTheta))));
		vertices.push_back(Vertex(0, -halfLength, 0, cylinderColour, Vector3(0, -1, 0)));
	}
	return vertices;
}

Colour ShapeVertices::GetColour(const Colour& _rainbow, const Colour& _colour)
{
	if (_rainbow != Colours::white) return _rainbow;
	else return _colour;
}

/*void Shapes::ShapeVertices::ComputeFaceNormal(const Vector3& v1, const Vector3& v2, const Vector3& v3, Vector3& normal)
{
	normal = Vector3();

	// find 2 edge vectors: v1-v2, v1-v3
	float ex1 = v2[0] - v1[0];
	float ey1 = v2[1] - v1[1];
	float ez1 = v2[2] - v1[2];
	float ex2 = v3[0] - v1[0];
	float ey2 = v3[1] - v1[1];
	float ez2 = v3[2] - v1[2];

	// cross product: e1 x e2
	float nx, ny, nz;
	nx = ey1 * ez2 - ez1 * ey2;
	ny = ez1 * ex2 - ex1 * ez2;
	nz = ex1 * ey2 - ey1 * ex2;

	// normalize only if the length is > 0
	float length = sqrtf(nx * nx + ny * ny + nz * nz);
	if (length > 0.0001f)
	{
		// normalize
		float lengthInv = 1.0f / length;
		normal = Vector3(nx * lengthInv, ny * lengthInv, nz * lengthInv);
	}
}

void Shapes::ShapeVertices::ComputeHalfVertex(const float v1[3], const float v2[3], float length, float newV[3])
{
	newV[0] = v1[0] + v2[0];
	newV[1] = v1[1] + v2[1];
	newV[2] = v1[2] + v2[2];
	float scale = 1.0f / sqrtf(newV[0] * newV[0] + newV[1] * newV[1] + newV[2] * newV[2]);
	newV[0] *= scale;
	newV[1] *= scale;
	newV[2] *= scale;
}*/
