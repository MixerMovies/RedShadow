#include "MaterialInfo.h"

#include <fstream>
#include <iostream>

#include "Util.h"
#include "FileLoader.h"

std::vector<Shader*> MaterialInfo::shaders = std::vector<Shader*>();

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
	Shader* shader1 = new Shader("Shaders/Standard/texture.vs", "Shaders/Standard/texture.fs", "Shaders/Geometry/standard.gs");
	shaders.push_back(shader1);
	Shader* shader2 = new Shader("Shaders/Standard/texture.vs", "Shaders/Standard/texture.fs", "Shaders/Geometry/standard.gs");
	shaders.push_back(shader2);
	Shader* shader3 = new Shader("Shaders/Standard/texture.vs", "Shaders/Standard/texture.fs", "Shaders/Geometry/standard.gs");
	shaders.push_back(shader3);
	Shader* shader4 = new Shader("Shaders/Standard/texture.vs", "Shaders/Standard/texture.fs", "Shaders/Geometry/standard.gs");
	shaders.push_back(shader4);
	Shader* shader5 = new Shader("Shaders/Standard/texture.vs", "Shaders/Standard/texture.fs", "Shaders/Geometry/standard.gs");
	shaders.push_back(shader5);
	Shader* shader6 = new Shader("Shaders/Standard/texture.vs", "Shaders/Standard/texture.fs", "Shaders/Geometry/standard.gs");
	shaders.push_back(shader6);
	Shader* shader7 = new Shader("Shaders/Standard/texture.vs", "Shaders/Standard/texture.fs", "Shaders/Geometry/standard.gs");
	shaders.push_back(shader7);
	Shader* shader8 = new Shader("Shaders/Standard/texture.vs", "Shaders/Standard/texture.fs", "Shaders/Geometry/standard.gs");
	shaders.push_back(shader8);
	Shader* shader9 = new Shader("Shaders/Standard/texture.vs", "Shaders/Standard/texture.fs", "Shaders/Geometry/standard.gs");
	shaders.push_back(shader9);
	Shader* shader10 = new Shader("Shaders/Standard/texture.vs", "Shaders/Standard/texture.fs", "Shaders/Geometry/standard.gs");
	shaders.push_back(shader10);
	Shader* shader11 = new Shader("Shaders/Standard/texture.vs", "Shaders/Standard/texture.fs", "Shaders/Geometry/standard.gs");
	shaders.push_back(shader11);
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
		currentMaterial->currentShader = MaterialInfo::getShaderByIllum(currentMaterial->illum);
	}

	materials.push_back(currentMaterial);
}