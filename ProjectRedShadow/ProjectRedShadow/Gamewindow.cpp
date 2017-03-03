#include "Gamewindow.h"
#include "Space.h"
#include "ObjModel.h"
#include "SoundTest.h"

SoundTest test = SoundTest();

Gamewindow::Gamewindow()
{
	Space::Instance()->building = new ObjModel("models/Library/library.obj");
	Space::Instance()->music = test.LoadSound("Sound/OdeToJoy(Remix).wav");
	Space::Instance()->music->Play();
}


Gamewindow::~Gamewindow()
{
}

void Gamewindow::Setup(int windowWidth, int windowHeight)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glViewport(0, 0, windowWidth, windowHeight);
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
	glTranslatef(Space::Instance()->player.position[0], Space::Instance()->player.position[1], Space::Instance()->player.position[2]);
}

void Gamewindow::Display()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	Space::Instance()->building->draw();
	glFlush();
	glutSwapBuffers();
}
