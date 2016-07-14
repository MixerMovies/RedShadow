#include "GL\freeglut.h"

GLint gameWindowInt;

void Idle();
void Display();

void Idle()
{
}

void Display()
{
}

int main(int argc, char *argv[])
{
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInit(&argc, argv);

	glutInitWindowSize(1920, 1080);
	gameWindowInt = glutCreateWindow("Tower City");
	glutIdleFunc(Idle);
	glutDisplayFunc(Display);

	glutMainLoop();
}