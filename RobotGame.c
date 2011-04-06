#define PROGRAM_TITLE "CPSC 3710 TERM"
#define DISPLAY_INFO "Robot World"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

//Global Variables
int Window_ID;
int Window_Width = 600;
int Window_Height = 400;

int main(int &argc, char **argv)
{
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT DEPTH);
	glutInitWindowSize(Window_Width, Window_Height);
	Window_ID = glutCreateWindow(PROGRAM_TITLE);
	glutInit(Window_Width,Window_Height);
	
}
