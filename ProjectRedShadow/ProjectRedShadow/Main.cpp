#include "GL\glew.h"
#include "GL\freeglut.h"
#include "Gamewindow.h"
#include "Space.h"

GLint gameWindowInt;
Gamewindow* gamewindow;
int windowWidth = 1920;
int windowHeight = 1080;
float lastTime;

void Init();
void Idle();
void Display();
void KeyEvent(unsigned char, int, int);
void KeyEventUp(unsigned char, int, int);
void SpecialKeyEvent(int, int, int);

void Init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat position[4] = { 0, -10, 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Idle()
{
	float time = glutGet(GLUT_ELAPSED_TIME);
	float elapsed = time - lastTime;


	gamewindow->rotation += elapsed / 1000.0f;

	Space::Instance()->player.move();
	glutPostRedisplay();
	lastTime = time;
}

void Display()
{
	gamewindow->Setup(windowWidth, windowHeight);
	gamewindow->Display();
}

void Reshape(int WindowWidth, int WindowHeight)
{
	windowWidth = WindowWidth;
	windowHeight = WindowHeight;
}

void KeyEvent(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
		Space::Instance()->player.turnLeft(true);
		break;
	case 'd':
		Space::Instance()->player.turnRight(true);
		break;
	case'w':
		Space::Instance()->player.goForward();
		break;
	case 's':
		Space::Instance()->player.goBackward();
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
		Space::Instance()->player.turnLeft(false);
		break;
	case 'd':
		Space::Instance()->player.turnRight(false);
		break;
	case 'w':
		Space::Instance()->player.stop();
		break;
	case 's':
		Space::Instance()->player.stop();
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

	glewInit();
	glClearColor(0.5f, 0.5f, 0.0f, 1.0f);

	gamewindow = new Gamewindow();
	
	
	Space::Instance();

	glutMainLoop();
}