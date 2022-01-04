#include "Skybox.h"

#include <vector>

Skybox::Skybox(int x, int y, int z, int size)
{
	Skybox::size = size;
	location = glm::vec3(x, y, z);
	rotation = glm::vec3(0, 0, 0);
	name = "Skybox";
	scale = glm::vec3(size, size, size);
    CreateSkybox();
	//objModel = new ObjModel("models/Projects/Skyboxes/std-skybox.obj");
}

void Skybox::CreateSkybox()
{
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    std::vector<uint16_t> indices = std::vector<uint16_t>();
    for (int i = 1; i < 109; i++)
    {
        indices.push_back(i / 3);
    }

    std::vector<float> vertices = std::vector<float>();
    vertices.insert(vertices.end(), skyboxVertices, skyboxVertices + 108);

    std::vector<std::string> textures;
    textures.push_back("Textures/skybox/right.jpg");
    textures.push_back("Textures/skybox/left.jpg");
    textures.push_back("Textures/skybox/top.jpg");
    textures.push_back("Textures/skybox/bottom.jpg");
    textures.push_back("Textures/skybox/front.jpg");
    textures.push_back("Textures/skybox/back.jpg");

    objModel = new ObjModel(vertices, vertices, vertices, indices, new Texture(textures), new Shader( "Shaders/Standard/cubemap.vs", "Shaders/Standard/cubemap.fs", "Shaders/Geometry/cubemap.gs"));
}

void Skybox::Draw()
{

}

Skybox::~Skybox()
{
}
