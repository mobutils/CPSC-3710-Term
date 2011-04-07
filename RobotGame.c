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

int deltaX=0;
int deltaY=0; 
int deltaZ=0;

GLfloat lightDiffuse[] = {.9, .9, .9, 5.0};
GLfloat lightPosition[] = {10.0, 50.0, 10.0, 0.5};  /* Infinite light location. */

void display(void)
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	
	gluLookAt(10.0+deltaX,2.5+deltaY,10.0+deltaZ,0.0+deltaX,0.0,0.0+deltaZ, 0.0,1.0,0.0);
	
	
	//Draw ground
	glBegin(GL_QUADS);
		glColor4f(0.0f,1.0f,0.0f,0.0f); //Green
		glVertex3f(0.0f, 0.0f, 0.0f);
				glColor4f(0.0f,0.0f,1.0f,0.0f); //B
		glVertex3f(0.0f, 0.0f, 100.0f);
				glColor4f(1.0f,0.0f,0.0f,0.0f); //Green
		glVertex3f(100.0f,0.0f,100.0f);
				glColor4f(0.0f,1.0f,0.0f,0.0f); //Green
		glVertex3f(100.0f,0.0f,0.0f);
	glEnd();
	
	//Start drawing buildings from origin
	glTranslatef(5.0f,2.5f,5.0f);
	
	int j;
	for(j=0; j < 10; j++)
	{
		if(j != 0)
			glTranslatef(-100.f,0.0f,10.0f);
	
		int i;
		for(i=0; i < 10; i++)
		{
			glColor4f(1.0f,1.0f,1.0f,0.5f);
			glutSolidCube(5.0f);
			glTranslatef(10.0f,0.0f,0.0f);
		}	
	}
	//Draw Buildings
	
	
	
	glutSwapBuffers();
	
	
}

void resize(int width, int height)
{
	//Prevent divide by zero errors
	if(height <= 0)
		height = 1;
		
	glViewport(0,0, (GLsizei)width, (GLsizei)height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(60.0f,(float)width/(float)height,1.0f,100.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	Window_Width = width;
	Window_Height = height;
}

void init(int width, int height)
{
	glClearColor(0.0f,0.0f,0.7f,1.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	
	glShadeModel(GL_SMOOTH);
	
	//Set up the sun.
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	
	glEnable(GL_DEPTH_TEST);

}

int keyPressControl(unsigned char key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_UP:
			printf ("KEY: UP press detected\n");
			deltaZ--;
		break;
		case GLUT_KEY_DOWN:
			printf ("KEY: DOWN press detected\n");
			deltaZ++;
		break;
		case GLUT_KEY_RIGHT:
			printf ("KEY: RIGHT press detected\n");
			deltaX++;
		break;
		case GLUT_KEY_LEFT:
			printf ("KEY: LEFT press detected\n");
			deltaX--;
		break;
		
		
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(Window_Width, Window_Height);
	Window_ID = glutCreateWindow(PROGRAM_TITLE);
	glutInitWindowPosition(0,0);
	
	glutDisplayFunc(&display);
	glutIdleFunc(&display);
	glutSpecialFunc(&keyPressControl);
	glutReshapeFunc(&resize);
	
	init(Window_Width, Window_Height);
	
	init(Window_Width,Window_Height);
	glutMainLoop();
	return 1;
}
