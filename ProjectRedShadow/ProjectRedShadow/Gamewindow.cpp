#include "Gamewindow.h"
#include "City.h"
#include "ObjModel.h"
#include "SoundTest.h"

SoundTest test = SoundTest();

Gamewindow::Gamewindow()
{
	City::Instance()->building = new ObjModel("models/Library/library.obj");
	City::Instance()->music = test.LoadSound("Sound/OdeToJoy(Remix).wav");
	City::Instance()->music->Play();
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
	glRotatef(City::Instance()->player.rotation[0], 1, 0, 0);
	glRotatef(City::Instance()->player.rotation[1], 0, 1, 0);
	glRotatef(City::Instance()->player.rotation[2], 0, 0, 1);
	glTranslatef(City::Instance()->player.position[0], City::Instance()->player.position[1], City::Instance()->player.position[2]);
}

void Gamewindow::Display()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	City::Instance()->building->draw();
	glFlush();
	glutSwapBuffers();
}
