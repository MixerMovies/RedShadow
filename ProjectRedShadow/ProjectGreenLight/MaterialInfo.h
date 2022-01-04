#pragma once

#include <vector>
#include <iostream>

#include "Texture.h"
#include "Shader.h"

class MaterialInfo
{
private:
	struct IllumShaderOptions
	{
		IllumShaderOptions(Shader* Default, Shader* BumpMap) 
		{
			DefaultShader = Default;
			BumpMapShader = BumpMap;
		};
		Shader* DefaultShader;
		Shader* BumpMapShader;
	};

	static std::vector<IllumShaderOptions*> shaders;
	static void initShaders();

	Shader* currentShader = nullptr;

public:

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
	Illum illum = HIGHLIGHT_ON;

	MaterialInfo();
	Shader* getShader() { return currentShader; };
	void setShader(Shader* shader) { currentShader = shader; };

	static void loadMaterialFile(std::vector<MaterialInfo*>& materials, std::string fileName, std::string dirName, Shader* shader = nullptr);
	static void finishCurrentMaterial(MaterialInfo* currentMaterial, Shader* shader, std::vector<MaterialInfo*>& materials);
	static Shader* getShaderByIllum(Illum illum, bool hasBumpMap = false) 
	{ 
		if (hasBumpMap)
		{
			if (shaders[illum]->BumpMapShader != nullptr)
				return shaders[illum]->BumpMapShader;
			else
			{
				std::cout << illum << " doesn't have a bump map shader" << std::endl;
				return shaders[illum]->DefaultShader;
			}
		}
		return shaders[illum]->DefaultShader;
	};
};
