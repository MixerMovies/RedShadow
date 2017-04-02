#include "Gamewindow.h"
#include "Space.h"
#include "ObjModel.h"
#include "SoundTest.h"

class Vertex
{
public:
	glm::vec3 position;
	glm::vec4 color;
	Vertex(const glm::vec3 &position, const glm::vec4 &color) : position(position), color(color) {}
};

SoundTest test = SoundTest();

Gamewindow::Gamewindow()
{
	Shader* shader = new Shader("Shaders/texture.vs", "Shaders/texture.fs");
	Space::Instance()->Spaceship = new ObjModel("models/ship/shipA_OBJ.obj");
	Space::Instance()->music = test.LoadSound("Sound/OdeToJoy(Remix).wav");
	//Space::Instance()->music->Play();
	shader->link();
	shaders.push_back(shader);
	Shader* shader2 = new Shader("Shaders/simple.vs", "Shaders/simple.fs");
	shader2->link();
	shaders.push_back(shader2);
	Shader* shader3 = new Shader("Shaders/specular.vs", "Shaders/specular.fs");
	shader3->link();
	shaders.push_back(shader3);
}

Gamewindow::~Gamewindow()
{
}

void Gamewindow::Setup(int windowWidth, int windowHeight)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 mvp = glm::perspective(80.0f, screenSize.x / (float)screenSize.y, 0.01f, 100.0f);		//begin met een perspective matrix
	mvp *= glm::lookAt(glm::vec3(0, 0, 50), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));					//vermenigvuldig met een lookat
	mvp = glm::translate(mvp, glm::vec3(0, 0, -1));													//of verplaats de camera gewoon naar achter
	mvp = glm::rotate(mvp, rotation, glm::vec3(0, 1, 0));											//roteer het object een beetje
	glUniformMatrix4fv(shaders[currentshader]->modelViewUniform, 1, 0, glm::value_ptr(mvp));
	glUniformMatrix4fv(shaders[currentshader]->)

	/*glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, (float)windowWidth / windowHeight, 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 0,
		0, 0, 1,
		0, 1, 0);
	glRotatef(Space::Instance()->player.rotation[0], 1, 0, 0);
	glRotatef(Space::Instance()->player.rotation[1], 0, 1, 0);
	glRotatef(Space::Instance()->player.rotation[2], 0, 0, 1);
	glTranslatef(Space::Instance()->player.position[0], Space::Instance()->player.position[1], Space::Instance()->player.position[2]);*/
}

void Gamewindow::Display()
{
	shaders[currentshader]->use();

	Space::Instance()->Spaceship->draw();

	//Space::Instance()->building->draw();
	glutSwapBuffers();
}

void Gamewindow::NextShader()
{
	if (currentshader < shaders.size() - 1)
	{
		currentshader++;
	}
	else
	{
		currentshader = 0;
	}
}

void Gamewindow::PreviousShader()
{
	if (currentshader == 0)
	{
		currentshader = shaders.size() - 1;
	}
	else
	{
		currentshader--;
	}
}