#pragma once

#include <string>
#include <vector>
#include <list>

#include <windows.h>
#include <gl/GL.h>
#include "Texture.h"

using namespace std;

class Vec3f
{
public:
	union
	{
		struct
		{
			float x, y, z;
		};
		float v[3];
	};
	Vec3f();
	Vec3f(Vec3f &other);
	Vec3f(float x, float y, float z);
	float& operator [](int);
};

class Vec2f
{
public:
	union
	{
		struct
		{
			float x, y;
		};
		float v[2];
	};
	Vec2f();
	Vec2f(float x, float y);
	Vec2f(Vec2f &other);
	float& operator [](int);
};


class ObjModel
{
private:
	class Vertex
	{
	public:
		int position;
		int normal;
		int texcoord;
	};

	class Face
	{
	public:
		list<Vertex> vertices;
	};
	class MaterialInfo
	{
	public:
		MaterialInfo();
		std::string name;
		bool hasTexture;
		Texture *texture;
		GLfloat ambient[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
		GLfloat diffuse[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
		GLfloat specular[4] = { 0, 0, 0, 1.0f };
		GLfloat shininess[1] = { 0 };
		GLfloat emission[4] = { 0, 0, 0, 1 };
	};

	class ObjGroup
	{
	public:
		std::string name;
		int materialIndex;
		list<Face> faces;
	};


	std::vector<Vec3f>	vertices;
	std::vector<Vec3f>	normals;
	std::vector<Vec2f>	texcoords;
	std::vector<ObjGroup*> groups;
	std::vector<MaterialInfo*> materials;

	void loadMaterialFile(std::string fileName, std::string dirName);
public:
	ObjModel(std::string filename);
	~ObjModel(void);

	void draw();
};
