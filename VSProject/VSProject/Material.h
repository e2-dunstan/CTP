#pragma once
#include <fstream>
#include <sstream>


enum class BounceCombineType
{
	MAXIMUM, MULTIPLY, MINIMUM, AVERAGE
};

enum class Material : int
{
	ICE = 0, 
	GLASS = 1, 
	CONCRETE = 2, 
	WOOD = 3,
	METAL = 4,
	SMOOTH_METAL = 5, 
	RUBBER = 6
};

namespace Materials
{
	//See "FrictionCoefficients - readable.csv" for clearer table
	const float coefficientsButEfficient[56]
	{
		0.1f,	0.02f,	0.1f,	0.03f,	0.5f,	0.5f,	0.05f,	0.05f,	0.03f,	0.03f,	0.03f,	0.03f,	0.6f,	0.6f,
						0.95f,	0.4f,	0.6f,	0.6f,	0.6f,	0.6f,	0.6f,	0.6f,	0.25f,	0.53f,	0.6f,	0.6f,
										0.8f,	0.1f,	0.75f,	0.5f,	1.0f,	1.0f,	0.68f,	0.68f,	1.0f,	0.8f,
														0.4f,	0.4f,	0.4f,	0.4f,	0.2f,	0.4f,	0.6f,	0.6f,
																		0.6f,	0.4f,	0.1f,	0.05f,	0.68f,	0.68f,
																						0.7f,	0.42f,	0.68f,	0.68f,
																										1.16f,	1.16f
	};
	/*const float coefficients[7][14]
	{
		{0.1f,	0.02f,	0.1f,	0.03f,	0.5f,	0.5f,	0.05f,	0.05f,	0.03f,	0.03f,	0.03f,	0.03f,	0.6f,	0.6f},
		{0.0f,	0.0f,	0.95f,	0.4f,	0.6f,	0.6f,	0.6f,	0.6f,	0.6f,	0.6f,	0.25f,	0.53f,	0.6f,	0.6f},
		{0.0f,	0.0f,	0.0f,	0.0f,	1.0f,	1.0f,	0.5f,	0.4f,	1.0f,	1.0f,	0.68f,	0.68f,	1.0f,	0.8f},
		{0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.4f,	0.4f,	0.4f,	0.4f,	0.2f,	0.4f,	0.6f,	0.6f},
		{0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.6f,	0.4f,	0.1f,	0.05f,	0.68f,	0.68f},
		{0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.7f,	0.42f,	0.68f,	0.68f},
		{0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	1.16f,	1.16f}
	};*/

	/*struct Texture
	{
		Texture(const char* path) : filepath(path) {}
		~Texture() = default;

		const char* filepath;
		unsigned char* data;
		unsigned int width, height;
		GLuint textureID;
	};
	const std::unique_ptr<Texture[]> textures(new Texture[6]
	{
		Texture("/Textures/concrete.bmp"),
		Texture("/Textures/ice.bmp"),
		Texture("/Textures/wood.bmp"),
		Texture("/Textures/rubber.bmp"),
		Texture("/Textures/metal1.bmp"),
		Texture("/Textures/metal2.bmp")
	});*/

	static float GetCombinedStaticFriction(const Material m1, const Material m2)
	{
		int row = (int)m1, col = (int)m2;
		if (row > col)
		{
			row = (int)m2;
			col = (int)m1;
		}

		int index = row * (13 - row) + 2 * col;

		//for (int r = 0; r < 7; r++)
		//{
		//	for (int c = r; c < 7; c++)
		//	{
		//		//index = ((r * 14) + (c * 2)) - (((r - 1) * r) + (r * 2));
		//		//index *= 2;
		//		index = r * (13 - r) + 2 * c;
		//		std::cout << "Row: " << r << ", Col: " << c << ", index: " << index << std::endl;
		//	}
		//}

		return coefficientsButEfficient[index];
		//return coefficients[col][row * 2];
	}
	static float GetCombinedDynamicFriction(const Material m1, const Material m2)
	{
		int row = (int)m1, col = (int)m2;
		if (row < col)
		{
			row = (int)m2;
			col = (int)m1;
		}

		int index = row * (13 - row) + 2 * col;

		return coefficientsButEfficient[index + 1];
		//return coefficients[col][(row * 2) + 1];
	}

	static float GetBounciness(const Material mat)
	{
		switch (mat)
		{
		case Material::WOOD:
			return 0.05f;
		case Material::RUBBER:
			return 0.6f;
		case Material::SMOOTH_METAL:
		case Material::METAL:
		case Material::GLASS:
		case Material::ICE:
		case Material::CONCRETE:
		default:
			return 0.01f;
		}
	}

	static BounceCombineType GetBounceCombineType(Material m1, Material m2)
	{
		return BounceCombineType::AVERAGE;

		if ((int)m1 > (int)m2)
			std::swap(m1, m2);

		if ((m1 == Material::RUBBER && m2 == Material::CONCRETE)
			|| (m1 == Material::SMOOTH_METAL && m2 == Material::RUBBER))
			return BounceCombineType::MAXIMUM;
		else
			return BounceCombineType::AVERAGE;
	}

	/*static void LoadTextures()
	{
		std::cout << "\n\tLoading textures... ";

		// -- BMP FILE LOADER -- //

		for (uint16_t i = 0; i < 6; i++)
		{
			unsigned char header[54];
			unsigned int dataPos;
			unsigned int imageSize; //width * height * 3

			FILE* file;// = fopen_s(&file, textures[i].filepath, "rb");
			fopen_s(&file, textures[i].filepath, "rb");
			if (!file)
			{
				std::cout << "\tERROR: Image cannot be opened: " << textures[i].filepath << std::endl;
				continue;
			}
			if (fread(header, 1, 54, file) != 54 || header[0] != 'B' || header[1] != 'M')
			{
				std::cout << "\tERROR: Image is not a BMP file: " << textures[i].filepath << std::endl;
				continue;
			}

			dataPos = *(int*)&(header[0x0A]);
			imageSize = *(int*)&(header[0x22]);
			textures[i].width = *(int*)&(header[0x12]);
			textures[i].height = *(int*)&(header[0x16]);
			if (imageSize == 0) imageSize = textures[i].width * textures[i].height * 3;
			if (dataPos == 0) dataPos = 54;

			textures[i].data = new unsigned char[imageSize];
			fread(textures[i].data, 1, imageSize, file);
			fclose(file);

			GLuint textureID;
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textures[i].width, textures[i].height, 0, GL_BGR, GL_UNSIGNED_BYTE, textures[i].data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			textures[i].textureID = textureID;
		}
		std::cout << "\tDone!" << std::endl;
	}

	static void BindTexture(const int matInt)
	{
		//0 = concrete, 1 = ice, 2 = wood, 3 = rubber, 4 = smooth metal, 5 = rough metal
		unsigned int textureIndex;
		switch (matInt)
		{
		case 0: //ice
			textureIndex = 1;
			break;
		case 3: //wood
			textureIndex = 2;
			break;
		case 4: //metal
			textureIndex = 5;
			break;
		case 5: //smooth metal
			textureIndex = 4;
			break;
		case 6: //rubber
			textureIndex = 3;
			break;
		case 1: //glass
		case 2: //concrete
		default:
			textureIndex = 0;
			break;
		}
	
		glGenTextures(1, &textures[textureIndex].textureID);
		glBindTexture(GL_TEXTURE_2D, textures[textureIndex].textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textures[textureIndex].width, textures[textureIndex].height, 0, GL_BGR, GL_UNSIGNED_BYTE, textures[textureIndex].data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	
	}*/
};