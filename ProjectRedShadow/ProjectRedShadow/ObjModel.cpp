#include <gl/glew.h>
#include "ObjModel.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <GL/freeglut_std.h>
#include "gtc/type_ptr.hpp"

#include "Util.h"
#include "FileLoader.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

glm::vec4 calcTangentVector(
	glm::vec3 pos1,		glm::vec3 pos2,		glm::vec3 pos3, 
	glm::vec2 texCoord1,	glm::vec2 texCoord2,	glm::vec2 texCoord3, glm::vec3 normal)
{
	// Given the 3 vertices (position and texture coordinates) of a triangle
	// calculate and return the triangle's tangent vector.

	// Create 2 vectors in object space.
	//
	// edge1 is the vector from vertex positions pos1 to pos2.
	// edge2 is the vector from vertex positions pos1 to pos3.
	glm::vec3 edge1(pos2-pos1);//Vector3 edge1(pos2[0] - pos1[0], pos2[1] - pos1[1], pos2[2] - pos1[2]);
	glm::vec3 edge2(pos3-pos1);//Vector3 edge2(pos3[0] - pos1[0], pos3[1] - pos1[1], pos3[2] - pos1[2]);

	edge1 = glm::normalize(edge1);
	edge2 = glm::normalize(edge2);

	// Create 2 vectors in tangent (texture) space that point in the same
	// direction as edge1 and edge2 (in object space).
	//
	// texEdge1 is the vector from texture coordinates texCoord1 to texCoord2.
	// texEdge2 is the vector from texture coordinates texCoord1 to texCoord3.
	glm::vec2 texEdge1(texCoord2 - texCoord1);
	glm::vec2 texEdge2(texCoord3 - texCoord1);

	texEdge1 = glm::normalize(texEdge1);
	texEdge2 = glm::normalize(texEdge2);

	// These 2 sets of vectors form the following system of equations:
	//
	//  edge1 = (texEdge1.x * tangent) + (texEdge1.y * bitangent)
	//  edge2 = (texEdge2.x * tangent) + (texEdge2.y * bitangent)
	//
	// Using matrix notation this system looks like:
	//
	//  [ edge1 ]     [ texEdge1.x  texEdge1.y ]  [ tangent   ]
	//  [       ]  =  [                        ]  [           ]
	//  [ edge2 ]     [ texEdge2.x  texEdge2.y ]  [ bitangent ]
	//
	// The solution is:
	//
	//  [ tangent   ]        1     [ texEdge2.y  -texEdge1.y ]  [ edge1 ]
	//  [           ]  =  -------  [                         ]  [       ]
	//  [ bitangent ]      det A   [-texEdge2.x   texEdge1.x ]  [ edge2 ]
	//
	//  where:
	//        [ texEdge1.x  texEdge1.y ]
	//    A = [                        ]
	//        [ texEdge2.x  texEdge2.y ]
	//
	//    det A = (texEdge1.x * texEdge2.y) - (texEdge1.y * texEdge2.x)
	//
	// From this solution the tangent space basis vectors are:
	//
	//    tangent = (1 / det A) * ( texEdge2.y * edge1 - texEdge1.y * edge2)
	//  bitangent = (1 / det A) * (-texEdge2.x * edge1 + texEdge1.x * edge2)
	//     normal = cross(tangent, bitangent)

	glm::vec3 t;
	glm::vec3 b;
	

	float det = (texEdge1[0] * texEdge2[1]) - (texEdge1[1] * texEdge2[0]);

	if (det == 0)
	{
		t = glm::vec3(1.0f, 0.0f, 0.0f);
		b = glm::vec3(0.0f, 1.0f, 0.0f);
	}
	else
	{
		det = 1.0f / det;

		t[0] = (texEdge2[1] * edge1[0] - texEdge1[1] * edge2[0]) * det;
		t[1] = (texEdge2[1] * edge1[1] - texEdge1[1] * edge2[0]) * det;
		t[2] = (texEdge2[1] * edge1[2] - texEdge1[1] * edge2[0]) * det;

		b[0] = (-texEdge2[0] * edge1[0] + texEdge1[0] * edge2[0]) * det;
		b[1] = (-texEdge2[0] * edge1[1] + texEdge1[0] * edge2[1]) * det;
		b[2] = (-texEdge2[0] * edge1[2] + texEdge1[0] * edge2[2]) * det;

		t = normalize(t);
		b = normalize(b);
	}

	// Calculate the handedness of the local tangent space.
	// The bitangent vector is the cross product between the triangle face
	// normal vector and the calculated tangent vector. The resulting bitangent
	// vector should be the same as the bitangent vector calculated from the
	// set of linear equations above. If they point in different directions
	// then we need to invert the cross product calculated bitangent vector. We
	// store this scalar multiplier in the tangent vector's 'w' component so
	// that the correct bitangent vector can be generated in the normal mapping
	// shader's vertex shader.

	glm::vec3 bitangent = glm::cross(normal, t);
	float handedness = (glm::dot(bitangent, b) < 0.0f) ? -1.0f : 1.0f;

	return glm::vec4(t[0], t[1], t[2], handedness);
}

ObjModel::ObjModel(std::string fileName)
{
	std::string dirName = fileName;
	if(dirName.rfind("/") != std::string::npos)
		dirName = dirName.substr(0, dirName.rfind("/"));
	if(dirName.rfind("\\") != std::string::npos)
		dirName = dirName.substr(0, dirName.rfind("\\"));
	if(fileName == dirName)
		dirName = "";


	std::ifstream pFile(fileName.c_str());

	if (!pFile.is_open())
	{
		std::cout << "Could not open file " << fileName << std::endl;
		return;
	}

	std::vector<float>	vertices;
	std::vector<float>	normals;
	std::vector<float>	texcoords;

	std::vector<float> finalVertices;


	ObjGroup* currentGroup = new ObjGroup();
	currentGroup->end = -1;
	currentGroup->start = 0;
	currentGroup->materialIndex = -1;


	while(!pFile.eof())
	{
		std::string line;
		std::getline(pFile, line);
		
		line = Util::replace(line, "\t", " ");
		while(line.find("  ") != std::string::npos)
			line = Util::replace(line, "  ", " ");
		if(line == "")
			continue;
		if(line[0] == ' ')
			line = line.substr(1);
		if(line == "")
			continue;
		if(line[line.length()-1] == ' ')
			line = line.substr(0, line.length()-1);
		if(line == "")
			continue;
		if(line[0] == '#')
			continue;

		std::vector<std::string> params = Util::split(line, " ");
		params[0] = Util::toLower(params[0]);

		if(params[0] == "v")
		{
			vertices.push_back((float)atof(params[1].c_str()));
			vertices.push_back((float)atof(params[2].c_str()));
			vertices.push_back((float)atof(params[3].c_str()));
		}
		else if(params[0] == "vn")
		{
			normals.push_back((float)atof(params[1].c_str()));
			normals.push_back((float)atof(params[2].c_str()));
			normals.push_back((float)atof(params[3].c_str()));
		}
		else if(params[0] == "vt")
		{
			texcoords.push_back((float)atof(params[1].c_str()));
			texcoords.push_back((float)atof(params[2].c_str()));
		}
		else if(params[0] == "f")
		{
			std::vector<std::string> indices1 = Util::split(params[1], "/");
			std::vector<std::string> indices2 = Util::split(params[2], "/");
			std::vector<std::string> indices3 = Util::split(params[3], "/");
			
			glm::vec3 p1(vertices[(atoi(indices1[0].c_str())-1)*3+0],vertices[(atoi(indices1[0].c_str())-1)*3+1],vertices[(atoi(indices1[0].c_str())-1)*3+2]);
			glm::vec3 p2(vertices[(atoi(indices2[0].c_str())-1)*3+0],vertices[(atoi(indices2[0].c_str())-1)*3+1],vertices[(atoi(indices2[0].c_str())-1)*3+2]);
			glm::vec3 p3(vertices[(atoi(indices3[0].c_str())-1)*3+0],vertices[(atoi(indices3[0].c_str())-1)*3+1],vertices[(atoi(indices3[0].c_str())-1)*3+2]);
			glm::vec2 t1(0, 0);
			glm::vec2 t2(0, 0);
			glm::vec2 t3(0, 0);
			if (texcoords.size() > 0 && indices1[1] != "")
			{
				glm::vec2 t1 = glm::vec2(texcoords[(atoi(indices1[1].c_str()) - 1) * 2 + 0], texcoords[(atoi(indices1[1].c_str()) - 1) * 2 + 1]);
				glm::vec2 t2 = glm::vec2(texcoords[(atoi(indices2[1].c_str()) - 1) * 2 + 0], texcoords[(atoi(indices2[1].c_str()) - 1) * 2 + 1]);
				glm::vec2 t3 = glm::vec2(texcoords[(atoi(indices3[1].c_str()) - 1) * 2 + 0], texcoords[(atoi(indices3[1].c_str()) - 1) * 2 + 1]);
			}

			glm::vec3 n(normals[(atoi(indices1[2].c_str())-1)*3+0],normals[(atoi(indices1[2].c_str())-1)*3+1],normals[(atoi(indices1[2].c_str())-1)*3+2]);

			glm::vec4 tangent(calcTangentVector(p1,p2,p3,t1,t2,t3,n));

			for(size_t ii = 4; ii <= params.size(); ii++)
			{

				for(size_t i = ii-3; i < ii; i++)
				{
					std::vector<std::string> indices = Util::split(params[i == (ii - 3) ? 1 : i], "/");
					glm::vec3 p;
					glm::vec2 t;
					glm::vec3 n;

					float px = 0, py = 0, pz = 0, tx = 0, ty = 0, nx = 0, ny = 0, nz = 0;
					if(indices.size() >= 1)
					{
						p[0] = vertices[(atoi(indices[0].c_str())-1) * 3+0];
						p[1] = vertices[(atoi(indices[0].c_str())-1) * 3+1];
						p[2] = vertices[(atoi(indices[0].c_str())-1) * 3+2];
					}
					if(indices.size() == 2) //texture 
					{
						t[0] = texcoords[(atoi(indices[1].c_str())-1) * 2+0];
						t[1] = texcoords[(atoi(indices[1].c_str())-1) * 2+1];
					}
					if(indices.size() == 3)
					{
						if( indices[1] != "")
						{
							t[0] = texcoords[(atoi(indices[1].c_str())-1) * 2+0];
							t[1] = texcoords[(atoi(indices[1].c_str())-1) * 2+1];
						}
						n[0] = normals[(atoi(indices[2].c_str())-1) * 3+0];
						n[1] = normals[(atoi(indices[2].c_str())-1) * 3+1];
						n[2] = normals[(atoi(indices[2].c_str())-1) * 3+2];
					}
					finalVertices.push_back(p[0]);
					finalVertices.push_back(p[1]);
					finalVertices.push_back(p[2]);
					finalVertices.push_back(n[0]);
					finalVertices.push_back(n[1]);
					finalVertices.push_back(n[2]);
					finalVertices.push_back(t[0]);
					finalVertices.push_back(t[1]);

					finalVertices.push_back(tangent[0]);
					finalVertices.push_back(tangent[1]);
					finalVertices.push_back(tangent[2]);
					finalVertices.push_back(tangent[3]);
					

					currentGroup->end = finalVertices.size() / 12;
				}
			}
		}
        else if(params[0] == "mtllib")
        {
            MaterialInfo::loadMaterialFile(materials, dirName + "/" + params[1], dirName);
        }
		else if(params[0] == "usemtl")
		{
			if(currentGroup->end != -1)
				groups.push_back(currentGroup);
			currentGroup = new ObjGroup();
			currentGroup->start = finalVertices.size()/12;
			currentGroup->materialIndex = -1;

			for(size_t i = 0; i < materials.size(); i++)
			{
				MaterialInfo* info = materials[i];
				if(info->name == params[1])
				{
					currentGroup->materialIndex = i;
					break;
				}
			}
			if(currentGroup->materialIndex == -1)
				std::cout<<"Could not find material name "<<params[1]<<std::endl;
		}
		//else
		//	std::cout<<line<<std::endl;

	}

	groups.push_back(currentGroup);
	size = finalVertices.size();
	std::cout << size << " " << fileName << std::endl;

    glGenVertexArrays(1, &_vertexArray);
    glBindVertexArray(_vertexArray);
        
    GLuint _vertexBuffer;
    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, finalVertices.size()*sizeof(GLfloat), &finalVertices[0], GL_STATIC_DRAW);
        
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 48, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 48, BUFFER_OFFSET(12));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 48, BUFFER_OFFSET(24));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 48, BUFFER_OFFSET(32));
        
    glBindVertexArray(0);        
}

ObjModel::ObjModel(std::vector<float> vertices, std::vector<float> normals, std::vector<float> textureCoordinats, std::vector<uint16_t> indices, Texture* texture, Shader* shader)
{
	std::vector<float> finalVertices = std::vector<float>();

	for (unsigned int i = 0; i < vertices.size() / 3; i++)
	{
		finalVertices.push_back(vertices[i * 3]);
		finalVertices.push_back(vertices[i * 3 + 1]);
		finalVertices.push_back(vertices[i * 3 + 2]);
		finalVertices.push_back(normals[i * 3]);
		finalVertices.push_back(normals[i * 3 + 1]);
		finalVertices.push_back(normals[i * 3 + 2]);
		finalVertices.push_back(textureCoordinats[i * 2]);
		finalVertices.push_back(textureCoordinats[i * 2 + 1]);
	}

	ObjGroup* currentGroup = new ObjGroup();
	currentGroup->end = -1;
	currentGroup->start = 0;
	currentGroup->materialIndex = -1;
	currentGroup->end = finalVertices.size() / 8;
	currentGroup->materialIndex = 0;
	groups.push_back(currentGroup);

	MaterialInfo* material = new MaterialInfo();
	
	material->texture = texture;
	material->hasTexture = true;
	material->setShader(shader);
	materials.push_back(material);

	glGenVertexArrays(1, &_vertexArray);
	glBindVertexArray(_vertexArray);

	GLuint _vertexBuffer;
	glGenBuffers(1, &_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, finalVertices.size() * sizeof(GLfloat), &finalVertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 32, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 32, BUFFER_OFFSET(12));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 32, BUFFER_OFFSET(24));
	//glEnableVertexAttribArray(3);
	//glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 48, BUFFER_OFFSET(32));

	// Create and populate the index buffer
	GLuint _indexBuffer;
	glGenBuffers(1, &_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	size = finalVertices.size();
}

ObjModel::~ObjModel(void)
{
}

void ObjModel::draw(ShaderMatrices matrices, Shader* shader)
{
    glBindVertexArray(_vertexArray);

	for (ObjGroup* group : groups)
	{
		MaterialInfo* material = materials[group->materialIndex];

		Shader* groupShader = shader;

		if (groupShader == nullptr)
			groupShader = material->getShader();

		groupShader->use();

		glUniform3f(groupShader->getUniformLocation("viewPosition"), matrices.viewPosition[0], matrices.viewPosition[1], matrices.viewPosition[2]);
		glUniform3f(groupShader->getUniformLocation("lightPosition"), matrices.lightPosition.x, matrices.lightPosition.y, matrices.lightPosition.z);
		glUniform4f(groupShader->getUniformLocation("lightColor"), matrices.lightColour.x, matrices.lightColour.y, matrices.lightColour.z, 1);

		glUniform1f(groupShader->getUniformLocation("time"), glutGet(GLUT_ELAPSED_TIME) / 1000.0f);
		glUniform1i(groupShader->getUniformLocation("s_texture"), 0);
		glUniform1i(groupShader->getUniformLocation("bump_map"), 1);
		glUniform1i(groupShader->getUniformLocation("cubemap"), 0);

		glUniformMatrix4fv(groupShader->getUniformLocation("viewMatrix"), 1, 0, glm::value_ptr(matrices.view));
		glUniformMatrix4fv(groupShader->getUniformLocation("projectionMatrix"), 1, 0, glm::value_ptr(matrices.projection));

		glUniformMatrix4fv(groupShader->getUniformLocation("modelMatrix"), 1, 0, glm::value_ptr(matrices.model));
		glUniformMatrix3fv(groupShader->getUniformLocation("normalMatrix"), 1, 0, glm::value_ptr(matrices.normalMatrix));

		glUniform1f(groupShader->getUniformLocation("shininess"), material->shininess);
		glUniform1i(groupShader->getUniformLocation("has_bump_map"), 0);

		glUniform1f(groupShader->getUniformLocation("alpha"), material->alpha);
		glUniform3fv(groupShader->getUniformLocation("ambient"), 1, material->ambient);
		glUniform3fv(groupShader->getUniformLocation("specular"), 1, material->specular);
		glUniform3fv(groupShader->getUniformLocation("diffuse"), 1, material->diffuse);

		if(material->hasTexture)
		{
			glActiveTexture(GL_TEXTURE0);
			if (material->texture->isCubeMap())
			{
				glDepthMask(GL_FALSE);
				//glBindVertexArray(skyboxVAO);
				glBindTexture(GL_TEXTURE_CUBE_MAP, material->texture->textureId);
				glDepthMask(GL_TRUE);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, material->texture->textureId);
			}
		}
		if(material->bumpMap != NULL)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, material->bumpMap->textureId);
		}
		/*else //disabled for now, because it crashes post processing
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, -1);
		}*/
		
		//if (size > 0)
		//	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_SHORT, 0);
		//else
		glDrawArrays(GL_TRIANGLES, group->start, group->end - group->start);

		groupShader = nullptr;
	}
}