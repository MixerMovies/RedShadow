#include "GL\glew.h"
#include "GL\freeglut.h"
#include "Gamewindow.h"
#include "Space.h"

GLint gameWindowInt;
Gamewindow* gamewindow;
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
	glEnable(GL_BLEND);
	glewInit();
	glEnableVertexAttribArray(0);							// positie
	glEnableVertexAttribArray(1);							// texcoord
	glEnableVertexAttribArray(2);							// normal
	glEnableVertexAttribArray(3);							// tangent
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
	gamewindow->Setup();
	gamewindow->Display();
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
	case '[':
		gamewindow->PreviousShader();
		break;
	case ']':
		gamewindow->NextShader();
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

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	gamewindow = new Gamewindow();
	
	Space::Instance();

	glutMainLoop();
}