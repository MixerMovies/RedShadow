#pragma once

#include <vector>

#include "Texture.h"

class MaterialInfo
{
	enum Illum
	{
		COLOR_ON_AMBIENT_OFF = 0,
		COLOR_ON_AMBIENT_ON = 1,
		HIGHLIGHT_ON = 2,
		REFLECTION_ON_RAY_TRACE_ON = 3,
		GLASS_ON_RAY_TRACE_ON = 4,
		FRESNEL_ON_RAY_TRACE_ON = 5,
		REFRACTION_ON_FRESNEL_OFF_RAY_TRACE_ON = 6,
		REFRACTION_ON_FRESNEL_ON_RAY_TRACE_ON = 7,
		REFLECTION_ON_RAY_TRACE_OFF = 8,
		GLASS_ON_RAY_TRACE_OFF = 9,
		INVISIBLE_SURFACES_SHADOW_CAST = 10
	};

public:
	MaterialInfo();
	static void loadMaterialFile(std::vector<MaterialInfo*>& materials, std::string fileName, std::string dirName);
	std::string name;
	Texture* texture;
	Texture* bumpMap;
	float ambient[3] = { 0, 0, 0 };
	float diffuse[3] = { 0, 0, 0 };
	float specular[3] = { 0, 0, 0 };
	float emission[3] = { 0, 0, 0 };
	float shininess = 10.0f;
	float alpha = 1.0f;
	float optical_density = 1.0f;
	bool hasTexture;
	unsigned int illum = 2;
};
