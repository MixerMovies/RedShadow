#include "GL\glew.h"
#include "GL\freeglut.h"
#include "Gamewindow.h"
#include "Space.h"
#include <iostream>

GLint gameWindowInt;
Gamewindow* gamewindow;
Space* space;
float lastTime;
bool wireframeEnabled = false;

void Init();
void Idle();
void Display();
void KeyEvent(unsigned char, int, int);
void KeyEventUp(unsigned char, int, int);
void SpecialKeyEvent(int, int, int);

#ifdef WIN32
void GLAPIENTRY onDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
#else
void onDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam)
#endif
{
	std::cout << message << std::endl;
}

void Init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glewInit();

	if (glDebugMessageCallback)
	{
		glDebugMessageCallback(&onDebug, NULL);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		glEnable(GL_DEBUG_OUTPUT);
	}
}

void Idle()
{
	float time = glutGet(GLUT_ELAPSED_TIME);
	float elapsed = time - lastTime;


	gamewindow->rotation += elapsed / 1000.0f;

	space->player.move(elapsed);
	glutPostRedisplay();
	lastTime = time;
}

void Display()
{
	gamewindow->Setup();
	gamewindow->Display();
}

void KeyEvent(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
		space->player.turnLeft(true);
		break;
	case 'd':
		space->player.turnRight(true);
		break;
	case'w':
		space->player.goForward();
		break;
	case 's':
		space->player.goBackward();
		break;
	case '[':
		gamewindow->PreviousShader();
		break;
	case ']':
		gamewindow->NextShader();
		break;
	case '.':
		space->PreviousModel();
		break;
	case '/':
		space->NextModel();
		break;
	case 'p':
		gamewindow->postProcessingEnabled = !gamewindow->postProcessingEnabled;
		break;
	case 'l':
		gamewindow->PreviousPostShader();
		break;
	case ';':
		gamewindow->NextPostShader();
		break;
	case 't':
		if (wireframeEnabled)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		wireframeEnabled = !wireframeEnabled;
		break;
	case 27:
		exit(0);
		break;
	}
}

void KeyEventUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
		space->player.turnLeft(false);
		break;
	case 'd':
		space->player.turnRight(false);
		break;
	case 'w':
		space->player.stop();
		break;
	case 's':
		space->player.stop();
		break;
	}
}

void SpecialKeyEvent(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F11:
		glutFullScreenToggle();
		break;
	}
}

void reshape(int newWidth, int newHeight)
{
	gamewindow->screenSize.x = newWidth;
	gamewindow->screenSize.y = newHeight;
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInit(&argc, argv);

	glutInitWindowSize(1920, 1080);
	gameWindowInt = glutCreateWindow("Tower City");
	glutIdleFunc(Idle);
	glutDisplayFunc(Display);
	glutKeyboardFunc(KeyEvent);
	glutKeyboardUpFunc(KeyEventUp);
	glutSpecialFunc(SpecialKeyEvent);
	glutReshapeFunc(reshape);
	Init();

	space = new Space();
	gamewindow = new Gamewindow(space);

	glutMainLoop();
}