#pragma once


enum class Material
{
	ICE, GLASS, CONCRETE, WOOD, METAL, SMOOTH_METAL, RUBBER
};

namespace Materials
{
	static float GetCombinedStaticFriction(const Material& m1, const Material& m2)
	{
		if (m1 == Material::WOOD && m2 == Material::WOOD)																		return 0.4f;
		if ((m1 == Material::WOOD && m2 == Material::CONCRETE) || (m2 == Material::WOOD && m1 == Material::CONCRETE))			return 0.5f;
		if ((m1 == Material::WOOD && m2 == Material::ICE) || (m2 == Material::WOOD && m1 == Material::ICE))						return 0.2f;
		if ((m1 == Material::GLASS && m2 == Material::ICE) || (m2 == Material::GLASS && m1 == Material::ICE))					return 0.1f;
		if (m1 == Material::GLASS && m2 == Material::GLASS)																		return 0.95f;
		if (m1 == Material::METAL && m2 == Material::METAL)																		return 0.6f;
		if ((m1 == Material::SMOOTH_METAL && m2 == Material::METAL) || (m2 == Material::SMOOTH_METAL && m1 == Material::METAL)) return 0.1f;
		if ((m1 == Material::RUBBER && m2 == Material::CONCRETE) || (m2 == Material::RUBBER && m1 == Material::CONCRETE))		return 1.0f;
	}
	static float GetCombinedDynamicFriction(const Material& m1, const Material& m2)
	{
		if (m1 == Material::WOOD && m2 == Material::WOOD)																		return 0.4f;
		if ((m1 == Material::WOOD && m2 == Material::CONCRETE) || (m2 == Material::WOOD && m1 == Material::CONCRETE))			return 0.4f;
		if ((m1 == Material::WOOD && m2 == Material::ICE) || (m2 == Material::WOOD && m1 == Material::ICE))						return 0.1f;
		if ((m1 == Material::GLASS && m2 == Material::ICE) || (m2 == Material::GLASS && m1 == Material::ICE))					return 0.03f;
		if (m1 == Material::GLASS && m2 == Material::GLASS)																		return 0.4f;
		if (m1 == Material::METAL && m2 == Material::METAL)																		return 0.4f;
		if ((m1 == Material::SMOOTH_METAL && m2 == Material::METAL) || (m2 == Material::SMOOTH_METAL && m1 == Material::METAL)) return 0.05f;
		if ((m1 == Material::RUBBER && m2 == Material::CONCRETE) || (m2 == Material::RUBBER && m1 == Material::CONCRETE))		return 0.8f;
	}
};