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

int boolpause = 0; //C does not support bool apparently (0=false, !0=true)

// camera controls relative to the origin (world)
int deltaX=0;
int deltaY=0; 
int deltaZ=0;

// camera control storage variables relative to the robot (local archive)
int alphaX=-7;
int alphaY=7;
int alphaZ=0;

// view port camera controls (local working)
int betaX=-5;
int betaY=5;
int betaZ=0;

int view = 0;

//Robot Global Variables
int RobX=0.0;
int RobY=0.0;
int RobZ=0.0;
int RobOrient=0;
float antDeg=0.0;
float antSpeed=0.1; //30 degrees made it look like a strobe
float headDeg=0.0;

#define BUILDINGS 10

//Building Map
int Building_Map[BUILDINGS*BUILDINGS];

//Ligthing
GLfloat lightDiffuse[] = {.9, .9, .9, 5.0};
GLfloat lightPosition[] = {10.0, 50.0, 10.0, 0.5};  /* Infinite light location. */

GLUquadricObj *quadratic;
GLuint texture[5];

void initMap()
{
	int i, count;
	
	count = BUILDINGS * BUILDINGS;
	for(i=0; i < count; i++)
	{	
			Building_Map[i] = 1;	
	}
}

void robotOrientation()
{
if(view!=0){
			switch(RobOrient){
				case 0:
					betaX=alphaX;
					betaY=alphaY;
					betaZ=alphaZ;
				break;
				case 90:
					if(view == 2 || view == 4){
						betaX=alphaX;
						betaY=alphaY;
						betaZ=-alphaZ;
					}
					else{
						betaX=-alphaX;
						betaY=alphaY;
						betaZ=alphaZ;
					}
				break;
				case 180:
					betaX=-alphaX;
					betaY=alphaY;
					betaZ=-alphaZ;
				break;
				case 270:
					if(view == 2 || view == 4){
						betaX=-alphaX;
						betaY=alphaY;
						betaZ=alphaZ;
					}
					else{
						betaX=alphaX;
						betaY=alphaY;
						betaZ=-alphaZ;
					}
			}
		}
		else{
			switch(RobOrient){
				case 0:
					betaX=alphaX;
					betaY=alphaY;
					betaZ=alphaZ;
				break;
				case 90:
					betaX=alphaZ;
					betaY=alphaY;
					betaZ=alphaX;//printf("X: %i Y: %i Z: %i \n",RobX,RobY,RobZ);
				break;
				case 180:
					betaX=-alphaX;
					betaY=alphaY;
					betaZ=-alphaZ;
				break;
				case 270:
					betaX=-alphaZ;
					betaY=alphaY;
					betaZ=-alphaX;
			}
					
		}
		if(RobX > 100){ RobX =98; deltaX=98; printf("ERROR CAUGHT (BoundX100)\n");}
		if(RobX < 0)  { RobX = 2; deltaX=2 ; printf("ERROR CAUGHT (BoundX0)\n");}
		if(RobZ > 100){ RobZ =98; deltaZ=98; printf("ERROR CAUGHT (BoundZ100)\n");}
		if(RobZ < 0)  { RobZ = 2; deltaZ=2 ; printf("ERROR CAUGHT (BoundZ0)\n");}
}


void drawBuildings(GLenum mode)
{
		//Start drawing buildings from origin
		glTranslatef(5.0f,2.5f,5.0f);
	
		int count = 1;
		int j;
		for(j=0; j < BUILDINGS; j++)
		{
			if(j != 0)
				glTranslatef(-100.f,0.0f,10.0f);
	
			int i;
			int index;
			for(i=0; i < BUILDINGS; i++)
			{
				if(mode == GL_SELECT)
				{
					glLoadName(count);
					//printf("Loading Object Name: %i", count);
				}
				
				index = (BUILDINGS*j) + i;
				
				glColor4f(1.0f,1.0f,1.0f,0.5f);
				//If previously shot don't draw
				if(Building_Map[index]==1)
					glutSolidCube(5.0f);
					
				glTranslatef(10.0f,0.0f,0.0f);
				count++;
			}	
		}
		//reset position
		glTranslatef(-105.0f,0.0f,-95.0f);
}

void display(void)
{
	if(boolpause==0){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		robotOrientation();
		gluLookAt(deltaX+betaX,deltaY+betaY,deltaZ+betaZ,0.0+deltaX,0.0,0.0+deltaZ, 0.0,1.0,0.0);
	
	
		//Draw ground
		glBegin(GL_QUADS);
			glColor4f(0.0f,1.0f,0.0f,0.0f); //Red corner
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, 100.0f);
			glVertex3f(100.0f,0.0f,100.0f);
			glVertex3f(100.0f,0.0f,0.0f);
		glEnd();
	
	
		//Draw Buildings
		drawBuildings(GL_RENDER);
	
	//*******************************************************************<  START ROBOT

		//Start drawing Robot
	
		//reset position
		//glLoadIdentity();
	
		//move to robot position
		glTranslatef(RobX,RobY,RobZ);
		
		
		// DEBUG: uncomment to verify robot position in global space
		//printf("POS: X: %i Z: %i LOOKAT X: %i Z: %i LOOKFROM X: %i Z: %i\n",RobX,RobZ,deltaX,deltaZ,betaX,betaZ);

		//turn the whole robot
		glRotatef(-RobOrient,0.0f,1.0f,0.0f);
	
		//draw head
		glColor4f(1.0f,1.0f,0.0f,0.0f); //Yellow
		glRotatef(-headDeg,0.0f,1.0f,0.0f);
		glutSolidCube(0.5f);
		glRotatef(90.0,0.0,1.0,0.0);
		glTranslatef(0.125f,0.0f,0.251f);
		glColor4f(1.0,1.0,1.0,0.0);
		gluCylinder(quadratic,0.1f,0.0f,0.1f,8,8);
		//glTranslate(0.15,0.0,0.0);
		glTranslatef(-0.125f,0.0f,-0.251f);
		glRotatef(-90.0,0.0,1.0,0.0);
		glRotatef(headDeg,0.0f,1.0f,0.0f);
		
		glColor4f(1.0f,1.0f,0.0f,0.0f); //Yellow
		//draw antenae //sp?	
		//glTranslatef(0.0,0.0,0.5);
		//gluCylinder(quadratic,0.2f,0.0f,0.5f,8,8);
		//glTranslatef(0.0,0.0,0.5);
		glRotatef(-90.0f,1.0f,0.0f,0.0f);
		antDeg+=antSpeed;
		glRotatef(-antDeg,0.0f,0.0f,1.0f);
		gluCylinder(quadratic,0.1f,0.1f,0.5f,8,8);
		glRotatef(antDeg,0.0f,0.0f,1.0f);
		glRotatef(90.0f,1.0f,0.0f,0.0f);

		//Draw Body
		//glTranslatef(0.0f,-1.0f,0.0f);
		glBegin(GL_QUADS);			// Start Drawing Quads
			// Front Face
			glNormal3f( 0.0f, 0.0f, 1.0f);		// Normal Facing Forward
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -2.5f,  0.5f);	// Bottom Left Of The Texture and Quad
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -2.5f,  0.5f);	// Bottom Right Of The Texture and Quad
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f, -0.5f,  0.5f);	// Top Right Of The Texture and Quad
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f,  0.5f);	// Top Left Of The Texture and Quad
			// Back Face
			glNormal3f( 0.0f, 0.0f,-1.0f);		// Normal Facing Away
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -2.5f, -0.5f);	// Bottom Right Of The Texture and Quad
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);	// Top Right Of The Texture and Quad
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f, -0.5f, -0.5f);	// Top Left Of The Texture and Quad
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -2.5f, -0.5f);	// Bottom Left Of The Texture and Quad
			// Top Face
			glNormal3f( 0.0f, 1.0f, 0.0f);		// Normal Facing Up
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);	// Top Left Of The Texture and Quad
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);	// Bottom Left Of The Texture and Quad
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.5f);	// Bottom Right Of The Texture and Quad
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f, -0.5f, -0.5f);	// Top Right Of The Texture and Quad
			// Bottom Face
			glNormal3f( 0.0f,-1.0f, 0.0f);		// Normal Facing Down
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -2.5f, -0.5f);	// Top Right Of The Texture and Quad
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f, -2.5f, -0.5f);	// Top Left Of The Texture and Quad
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -2.5f,  0.5f);	// Bottom Left Of The Texture and Quad
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -2.5f,  0.5f);	// Bottom Right Of The Texture and Quad
			// Right face
			glNormal3f( 1.0f, 0.0f, 0.0f);		// Normal Facing Right
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -2.5f, -0.5f);	// Bottom Right Of The Texture and Quad
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f, -0.5f, -0.5f);	// Top Right Of The Texture and Quad
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f, -0.5f,  0.5f);	// Top Left Of The Texture and Quad
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -2.5f,  0.5f);	// Bottom Left Of The Texture and Quad
			// Left Face
			glNormal3f(-1.0f, 0.0f, 0.0f);		// Normal Facing Left
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -2.5f, -0.5f);	// Bottom Left Of The Texture and Quad
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -2.5f,  0.5f);	// Bottom Right Of The Texture and Quad
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f,  0.5f);	// Top Right Of The Texture and Quad
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);	// Top Left Of The Texture and Quad
		glEnd();					// Done Drawing Quads

		// Draw Neck
		//glTranslatef(0.0f,-1.0f,0.0f);
		glBegin(GL_QUADS);			// Start Drawing Quads
			// Front Face
			glNormal3f( 0.0f, 0.0f, 1.0f);		// Normal Facing Forward
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.1f, -0.8f,  0.1f);	// Bottom Left Of The Texture and Quad
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.1f, -0.8f,  0.1f);	// Bottom Right Of The Texture and Quad
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.1f, 0.0f,  0.1f);	// Top Right Of The Texture and Quad
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.1f, 0.0f,  0.1f);	// Top Left Of The Texture and Quad
			// Back Face
			glNormal3f( 0.0f, 0.0f,-1.0f);		// Normal Facing Away
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.1f, -0.8f, -0.1f);	// Bottom Right Of The Texture and Quad
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.1f, 0.0f, -0.1f);	// Top Right Of The Texture and Quad
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.1f, 0.0f, -0.1f);	// Top Left Of The Texture and Quad
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.1f, -0.8f, -0.1f);	// Bottom Left Of The Texture and Quad
			// Top Face
			glNormal3f( 0.0f, 1.0f, 0.0f);		// Normal Facing Up
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.1f, 0.0f, -0.1f);	// Top Left Of The Texture and Quad
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.1f, 0.0f,  0.1f);	// Bottom Left Of The Texture and Quad
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.1f, 0.0f,  0.1f);	// Bottom Right Of The Texture and Quad
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.1f, 0.0f, -0.1f);	// Top Right Of The Texture and Quad
			// Bottom Face
			glNormal3f( 0.0f,-1.0f, 0.0f);		// Normal Facing Down
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.1f, -0.8f, -0.1f);	// Top Right Of The Texture and Quad
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.1f, -0.8f, -0.1f);	// Top Left Of The Texture and Quad
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.1f, -0.8f,  0.1f);	// Bottom Left Of The Texture and Quad
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.1f, -0.8f,  0.1f);	// Bottom Right Of The Texture and Quad
			// Right face
			glNormal3f( 1.0f, 0.0f, 0.0f);		// Normal Facing Right
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.1f, -0.8f, -0.1f);	// Bottom Right Of The Texture and Quad
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.1f, 0.0f, -0.1f);	// Top Right Of The Texture and Quad
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.1f, 0.0f,  0.1f);	// Top Left Of The Texture and Quad
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.1f, -0.8f,  0.1f);	// Bottom Left Of The Texture and Quad
			// Left Face
			glNormal3f(-1.0f, 0.0f, 0.0f);		// Normal Facing Left
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.1f, -0.8f, -0.1f);	// Bottom Left Of The Texture and Quad
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.1f, -0.8f,  0.1f);	// Bottom Right Of The Texture and Quad
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.1f, 0.0f,  0.1f);	// Top Right Of The Texture and Quad
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.1f, 0.0f, -0.1f);	// Top Left Of The Texture and Quad
		glEnd();					// Done Drawing Quads
		
		glColor4f(1.0f,0.0f,0.0f,0.0f); //Red
		
		// Draw Box on Front
		glBegin(GL_QUADS);
			glNormal3f( 1.0f, 0.0f, 0.0f);		// Normal Facing Right
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.51f, -2.0f, -0.35f);	// Bottom Right Of The Texture and Quad
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.51f, -1.0f, -0.35f);	// Top Right Of The Texture and Quad
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.51f, -1.0f,  0.35f);	// Top Left Of The Texture and Quad
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.51f, -2.0f,  0.35f);	// Bottom Left Of The Texture and Quad
			// Back Face
		glEnd();

		// Draw Triangles on back
		glColor4f(0.0f,0.0f,1.0f,0.0f); //BLUE
		glBegin(GL_TRIANGLES);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( -0.51f, -1.5f, -0.35f);	// Bottom Right Of The Texture and Quad
			glTexCoord2f(1.0f, 1.0f); glVertex3f( -0.51f, -1.0f, 0.0f);	// Top Right Of The Texture and Quad
			//glTexCoord2f(0.0f, 1.0f); glVertex3f( -0.51f, -1.0f,  0.35f);	// Top Left Of The Texture and Quad
			glTexCoord2f(0.0f, 0.0f); glVertex3f( -0.51f, -1.5f,  0.35f);	// Bottom Left Of The Texture and Quad
			// Back Face
		glEnd();

		glBegin(GL_TRIANGLES);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( -0.51f, -2.0f, -0.35f);	// Bottom Right Of The Texture and Quad
			glTexCoord2f(1.0f, 1.0f); glVertex3f( -0.51f, -1.5f, 0.0f);	// Top Right Of The Texture and Quad
			//glTexCoord2f(0.0f, 1.0f); glVertex3f( -0.51f, -0.0f,  0.35f);	// Top Left Of The Texture and Quad
			glTexCoord2f(0.0f, 0.0f); glVertex3f( -0.51f, -2.0f,  0.35f);	// Bottom Left Of The Texture and Quad
			// Back Face
		glEnd();
		//Done Draw Robot

		//turn the matrix back
		glRotatef(RobOrient,0.0f,1.0f,0.0f);
	//**************************************************************<  END ROBOT
	
		glutSwapBuffers();
	
	}
}






void init(int width, int height)
{
	glClearColor(0.0f,0.0f,0.7f,1.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	
	glShadeModel(GL_SMOOTH);

	//setup quadratic
	quadratic = gluNewQuadric();
	gluQuadricNormals(quadratic, GLU_SMOOTH);
	gluQuadricTexture(quadratic, GL_TRUE);
	
	//Set up the sun.
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	
	glEnable(GL_DEPTH_TEST);

}

void keyPressControl(int key, int x, int y)
{
	if(boolpause == 0){

		switch(key)
		{
		
			case GLUT_KEY_F1:
				printf("KEY: F1 press detected\n");
				headDeg=0.0;
			break;	
			case GLUT_KEY_F2:
				printf("KEY: F2 press detected\n");
				if(headDeg >= -45.0){
					headDeg -= 1.00;
				}
				else{printf("sorry the robot can only turn its head so far... press F1 or F3");}
			break;	
			case GLUT_KEY_F3:
				printf("KEY: F3 press detected\n");
				if(headDeg <= 45){
					headDeg += 1.0;
				}
				else{printf("sorry the robot can only turn its head so far... press F1 or F2");}
			break;
			case GLUT_KEY_F4:
				printf("KEY: F4 press detected\n");
				alphaX=-5;
				alphaY=5;
				alphaZ=0;
				view = 0;
			break;
			case GLUT_KEY_F5:
				printf("KEY: F5 press detected\n");
				alphaX=-5;
				alphaY=10;
				alphaZ=-5;
				view = 1;
			break;
			case GLUT_KEY_F6:
				printf("KEY: F6 press detected\n");
				alphaX=-5;
				alphaY=10;
				alphaZ=5;
				view = 2;
			break;
			case GLUT_KEY_F7:
				printf("KEY: F7 press detected\n");
				alphaX=5;
				alphaY=10;
				alphaZ=5;
				view = 3;
			break;
			case GLUT_KEY_F8:
				printf("KEY: F8 press detected\n");
				alphaX=5;
				alphaY=10;
				alphaZ=-5;
				view = 4;
			break;
		
		}
	}
}

void pressKey(unsigned char key, int x, int y)
{
	if(boolpause == 0){
		switch(key)
		{
			case 'p':
				printf ("KEY: p press detected [PAUSE GAME]\n");
				boolpause=1;
			break;
			case 'z':
				if(RobOrient==0.0){
					printf ("KEY: Z press detected [DIR=FORWARD]\n");
					if(deltaX>=0&&deltaX<=100){
						printf ("RESULT: VALID\n");
						deltaX++;
						RobX+=1.0;
					}
					else{
						printf ("RESULT:BOUND X(%i) Z(%i)\n",deltaX,deltaZ);
					// following code is to correct for error
					// redundant due to error catch in display
						deltaX--;
						RobX-=1.0;
					}
				}
				else if(RobOrient==90.0){
					printf ("KEY: Z press detected [DIR=LEFT]\n");
					if(deltaZ>=0&&deltaZ<=100){
						printf ("RESULT: VALID\n");
						deltaZ++;
						RobZ+=1.0;
					}
					else{
						printf ("RESULT:BOUND X(%i) Z(%i)\n",deltaX,deltaZ);
					// following code is to correct for error
					// redundant due to error catch in display
						deltaZ--;
						RobZ-=1.0;
					}
				}
				else if(RobOrient==180.0){
					printf ("KEY: Z press detected [DIR=BACK]\n");
					if(deltaX>=0&&deltaX<=100){
						printf ("RESULT: VALID\n");
						deltaX--;
						RobX-=1.0;
					}
					else{
						printf ("RESULT:BOUND X(%i) Z(%i)\n",deltaX,deltaZ);
					// following code is to correct for error
					// redundant due to error catch in display
						deltaX++;
						RobX+=1.0;
					}
				}
				else if(RobOrient==270.0){
					printf ("KEY: Z press detected [DIR=RIGHT]\n");
					if(deltaZ>=0&&deltaZ<=100){
						printf ("RESULT: VALID\n");
						deltaZ--;
						RobZ-=1.0;
					}
					else{
						printf ("RESULT:BOUND X(%i) Z(%i)\n",deltaX,deltaZ);
					// following code is to correct for error
					// redundant due to error catch in display
						deltaZ++;
						RobZ+=1.0;
					}
				}
				else{printf("ERROR UNDEFINED DIRECTION\n");}
				//printf ("Robz = %f \n",RobZ);
			break;	
			case 'r':
				printf ("KEY: r press detected [RESET ROBOT]\n");
				deltaX=0;
				deltaY=0; 
				deltaZ=0;
				
				initMap();

				RobX=0.0;
				RobY=0.0;
				RobZ=0.0;
				RobOrient=0.0;
				antDeg=0.0;
				antSpeed=0.1;
			break;
			case 'q':
				printf("KEY: a press detected\n");
				if(RobX%BUILDINGS<=2 || RobX%BUILDINGS>=8){
					if(RobZ%BUILDINGS<=2 || RobZ%BUILDINGS>=8){
						RobOrient-=90.0;
					}
					else{printf("Sorry, Not a valid street\n");}
				}
				else{printf("Sorry, Not a valid street\n");}
				if(RobOrient < 0.0){
					RobOrient += 360;
				}
			break;	
			case 'a':
				printf("KEY: a press detected\n");
				if(RobX%BUILDINGS<=2 || RobX%BUILDINGS>=8){
					if(RobZ%BUILDINGS<=2 || RobZ%BUILDINGS>=8){
						RobOrient+=90.0;
					}
					else{printf("Sorry, Not a valid street\n");}
				}
				else{printf("Sorry, Not a valid street\n");}
				if(RobOrient >= 360){
					RobOrient -= 360;
				}
			break;
			default:
				printf("UNDEFINED KEY\n");
			break;
		 }
	}
	else{
		switch(key)
		{
			case 'p':
				printf ("KEY: p press detected [UNPAUSE GAME]\n");
				boolpause=0;
			break;
			default:
				printf("Game Paused press 'p' to unpause\n");
			break;
		 }
	}
} 

void processHits(GLint hits, GLuint buffer[])
{
   GLint names, *ptr;

   printf ("hits = %d\n", hits);
   ptr = (GLint *) buffer; 
   /*for (i = 0; i < hits; i++) {	
      

      for (j = 0; j < names; j++) {
         printf("NAME: %i Array ID: %i",*ptr, *ptr-1);
         //Remove building from map */	   
         
         ptr+=3;
        names = *ptr;
        Building_Map[*ptr-1] = 0;
         
         printf("Name: %i, ArrayID: %i" , *ptr, *ptr-1);
         ptr++;
     	
     	
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

#define SIZE 512
void mouse(int button, int state, int x, int y)
{
	GLuint selectBuf[SIZE];
	GLint hits;
	GLint viewport[4];
	
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		glGetIntegerv (GL_VIEWPORT, viewport);
		glSelectBuffer(SIZE, selectBuf);
		
		glRenderMode(GL_SELECT);
		glInitNames();
		glPushName(0);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		
		gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3]-y), 5.0,5.0,viewport);
		
		gluPerspective(60.0f,(float)Window_Width/(float)Window_Height,1.0f,100.0f);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		robotOrientation();
		gluLookAt(deltaX+betaX,deltaY+betaY,deltaZ+betaZ,0.0+deltaX,0.0,0.0+deltaZ, 0.0,1.0,0.0);
		
		drawBuildings(GL_SELECT);
		
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glFlush();
		
		glMatrixMode(GL_MODELVIEW);
		
		hits = glRenderMode(GL_RENDER);
		
		processHits(hits, selectBuf);
		
		//printf("Left Button Press Detected! X: %i Y: %i \n", x, y);
		
		glutPostRedisplay();
		
		
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
	glutKeyboardFunc(&pressKey);
	glutReshapeFunc(&resize);
	glutMouseFunc(&mouse);
	
	init(Window_Width, Window_Height);
	
	initMap();
	
	init(Window_Width,Window_Height);
	glutMainLoop();
	return 1;
}


