#include "MaterialInfo.h"

#include <fstream>
#include <iostream>

#include "Util.h"
#include "FileLoader.h"

std::vector<MaterialInfo::IllumShaderOptions*> MaterialInfo::shaders = std::vector<IllumShaderOptions*>();

MaterialInfo::MaterialInfo()
{
	if (MaterialInfo::shaders.empty())
		initShaders();

	texture = NULL;
	bumpMap = NULL;
	hasTexture = false;
}

void MaterialInfo::initShaders()
{
	IllumShaderOptions* illum0 = new IllumShaderOptions(new Shader("Shaders/Illum/0.vs", "Shaders/Illum/0.fs", "Shaders/Geometry/standard.gs"), nullptr);
	shaders.push_back(illum0);
	IllumShaderOptions* illum1 = new IllumShaderOptions(new Shader("Shaders/Illum/1.vs", "Shaders/Illum/1.fs", "Shaders/Geometry/standard.gs"), nullptr);
	shaders.push_back(illum1);
	IllumShaderOptions* illum2 = new IllumShaderOptions(new Shader("Shaders/Illum/2.vs", "Shaders/Illum/2.fs", "Shaders/Geometry/standard.gs"), 
		new Shader("Shaders/Illum/2.vs", "Shaders/Illum/2-bumpmap.fs", "Shaders/Geometry/standard.gs"));
	shaders.push_back(illum2);
	IllumShaderOptions* illum3 = new IllumShaderOptions(new Shader("Shaders/Illum/2.vs", "Shaders/Illum/2.fs", "Shaders/Geometry/standard.gs"), nullptr);
	shaders.push_back(illum3);
	IllumShaderOptions* illum4 = new IllumShaderOptions(new Shader("Shaders/Illum/2.vs", "Shaders/Illum/2.fs", "Shaders/Geometry/standard.gs"), nullptr);
	shaders.push_back(illum4);
	IllumShaderOptions* illum5 = new IllumShaderOptions(new Shader("Shaders/Illum/2.vs", "Shaders/Illum/2.fs", "Shaders/Geometry/standard.gs"), nullptr);
	shaders.push_back(illum5);
	IllumShaderOptions* illum6 = new IllumShaderOptions(new Shader("Shaders/Illum/2.vs", "Shaders/Illum/2.fs", "Shaders/Geometry/standard.gs"), nullptr);
	shaders.push_back(illum6);
	IllumShaderOptions* illum7 = new IllumShaderOptions(new Shader("Shaders/Illum/2.vs", "Shaders/Illum/2.fs", "Shaders/Geometry/standard.gs"), nullptr);
	shaders.push_back(illum7);
	IllumShaderOptions* illum8 = new IllumShaderOptions(new Shader("Shaders/Illum/8.vs", "Shaders/Illum/8.fs", "Shaders/Geometry/standard.gs"), nullptr);
	shaders.push_back(illum8);
	IllumShaderOptions* illum9 = new IllumShaderOptions(new Shader("Shaders/Illum/9.vs", "Shaders/Illum/9.fs", "Shaders/Geometry/standard.gs"), nullptr);
	shaders.push_back(illum9);
	IllumShaderOptions* illum10 = new IllumShaderOptions(new Shader("Shaders/Illum/2.vs", "Shaders/Illum/2.fs", "Shaders/Geometry/standard.gs"), nullptr);
	shaders.push_back(illum10);
}

void MaterialInfo::loadMaterialFile(std::vector<MaterialInfo*>& materials, std::string fileName, std::string dirName, Shader* shader)
{
	std::ifstream pFile(fileName.c_str());

	if (!pFile.is_open())
	{
		std::cout << "Could not open file " << fileName << std::endl;
		return;
	}

	MaterialInfo* currentMaterial = NULL;

	while (!pFile.eof())
	{
		std::string line;
		std::getline(pFile, line);

		line = Util::replace(line, "\t", " ");
		while (line.find("  ") != std::string::npos)
			line = Util::replace(line, "  ", " ");
		if (line == "")
			continue;
		if (line[0] == ' ')
			line = line.substr(1);
		if (line == "")
			continue;
		if (line[line.length() - 1] == ' ')
			line = line.substr(0, line.length() - 1);
		if (line == "")
			continue;
		if (line[0] == '#')
			continue;

		std::vector<std::string> params = Util::split(line, " ");
		params[0] = Util::toLower(params[0]);

		if (params[0] == "newmtl")
		{
			if (currentMaterial != NULL)
			{
				finishCurrentMaterial(currentMaterial, shader, materials);
			}
			currentMaterial = new MaterialInfo();
			currentMaterial->name = params[1];
		}
		else if (params[0] == "map_kd")
		{
			currentMaterial->texture = new Texture(dirName + "/" + params[1]);
			currentMaterial->hasTexture = true;
		}
		else if (params[0] == "map_bump")
		{
			currentMaterial->bumpMap = new Texture(dirName + "/" + params[1]);
		}
		else if (params[0] == "ka")
		{
			currentMaterial->ambient[0] = std::stof(params[1]);
			currentMaterial->ambient[1] = std::stof(params[2]);
			currentMaterial->ambient[2] = std::stof(params[3]);
		}
		else if (params[0] == "kd")
		{
			currentMaterial->diffuse[0] = std::stof(params[1]);
			currentMaterial->diffuse[1] = std::stof(params[2]);
			currentMaterial->diffuse[2] = std::stof(params[3]);
		}
		else if (params[0] == "ks")
		{
			currentMaterial->specular[0] = std::stof(params[1]);
			currentMaterial->specular[1] = std::stof(params[2]);
			currentMaterial->specular[2] = std::stof(params[3]);
		}
		else if (params[0] == "ke")
		{
			currentMaterial->emission[0] = std::stof(params[1]);
			currentMaterial->emission[1] = std::stof(params[2]);
			currentMaterial->emission[2] = std::stof(params[3]);
		}
		else if (params[0] == "ns")
		{
			currentMaterial->shininess = std::stof(params[1]);
		}
		else if (params[0] == "d")
		{
			currentMaterial->alpha = std::stof(params[1]);
		}
		else if (params[0] == "illum")
		{
			currentMaterial->illum = static_cast<Illum>(std::stoi(params[1]));
			std::cout << currentMaterial->illum << std::endl;
		}
		else if (params[0] == "ni")
		{
			currentMaterial->optical_density = std::stof(params[1]);
		}
		else
			std::cout << "Didn't parse " << params[0] << " in material file " << currentMaterial->name << " from model " << fileName << std::endl;
	}

	if (currentMaterial != NULL)
	{
		finishCurrentMaterial(currentMaterial, shader, materials);
	}
}

void MaterialInfo::finishCurrentMaterial(MaterialInfo* currentMaterial, Shader* shader, std::vector<MaterialInfo*>& materials)
{
	Texture* emptyTexture = new Texture(FileLoader::getMainPath() + "\\Textures\\White.png");

	if (currentMaterial->texture == NULL)
	{
		currentMaterial->texture = emptyTexture;
		currentMaterial->hasTexture = true;
	}

	if (currentMaterial->currentShader != nullptr)
		currentMaterial->currentShader = shader;
	else
	{
		currentMaterial->currentShader = MaterialInfo::getShaderByIllum(currentMaterial->illum, currentMaterial->bumpMap != nullptr);
	}

	materials.push_back(currentMaterial);
}

Shader* MaterialInfo::getShaderByIllum(Illum illum, bool hasBumpMap)
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
}