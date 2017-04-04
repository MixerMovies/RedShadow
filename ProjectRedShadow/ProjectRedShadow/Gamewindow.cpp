#include "Gamewindow.h"
#include "Space.h"
#include "ObjModel.h"
#include "SoundTest.h"

SoundTest test = SoundTest();

Gamewindow::Gamewindow()
{
	Space::Instance()->Spaceship = new ObjModel("models/ship/shipA_OBJ.obj");
	//Space::Instance()->music = test.LoadSound("Sound/OdeToJoy(Remix).wav");
	//Space::Instance()->music->Play();
	Shader* shader = new Shader("Shaders/texture.vs", "Shaders/texture.fs");
	shader->link();
	shader->use();
	shaders.push_back(shader);
	Shader* shader2 = new Shader("Shaders/simple.vs", "Shaders/simple.fs");
	shader2->link();
	shader2->use();
	shaders.push_back(shader2);
	Shader* shader3 = new Shader("Shaders/specular.vs", "Shaders/specular.fs");
	shader3->link();
	shader3->use();
	shaders.push_back(shader3);
	Shader* shader4 = new Shader("Shaders/textureanim.vs", "Shaders/textureanim.fs");
	shader4->link();
	shader4->use();
	shaders.push_back(shader4);

	glEnableVertexAttribArray(0);							// positie
	glEnableVertexAttribArray(1);							// texcoord
	glEnableVertexAttribArray(2);							// normal
	glEnableVertexAttribArray(3);							// tangent

	if (glDebugMessageCallback)
	{
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		glEnable(GL_DEBUG_OUTPUT);
	}
}

Gamewindow::~Gamewindow()
{
}

void Gamewindow::Setup()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection = glm::perspective(70.0f, screenSize.x / (float)screenSize.y, 0.01f, 200.0f);		//begin met een perspective matrix
	glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 50), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));					//vermenigvuldig met een lookat
	glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(0, 0, -1));													//of verplaats de camera gewoon naar achter
	model = glm::rotate(model, rotation, glm::vec3(0, 1, 0));											//roteer het object een beetje
	
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(view * model)));										//roteer het object een beetje

	shaders[currentshader]->use();

	glUniformMatrix4fv(shaders[currentshader]->getUniformLocation("modelMatrix"), 1, 0, glm::value_ptr(model));
	glUniformMatrix4fv(shaders[currentshader]->getUniformLocation("viewMatrix"), 1, 0, glm::value_ptr(view));
	glUniformMatrix4fv(shaders[currentshader]->getUniformLocation("projectionMatrix"), 1, 0, glm::value_ptr(projection));
	glUniformMatrix3fv(shaders[currentshader]->getUniformLocation("normalMatrix"), 1, 0, glm::value_ptr(normalMatrix));

	glUniform1f(shaders[currentshader]->getUniformLocation("time"), glutGet(GLUT_ELAPSED_TIME) / 1000.0f);
	glUniform1i(shaders[currentshader]->getUniformLocation("s_texture"), 0);
	/*glRotatef(Space::Instance()->player.rotation[0], 1, 0, 0);
	glRotatef(Space::Instance()->player.rotation[1], 0, 1, 0);
	glRotatef(Space::Instance()->player.rotation[2], 0, 0, 1);
	glTranslatef(Space::Instance()->player.position[0], Space::Instance()->player.position[1], Space::Instance()->player.position[2]);*/
}

void Gamewindow::Display()
{

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