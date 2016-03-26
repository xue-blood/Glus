#include "Canvas.h"

#include <math.h>
#include <stdio.h>


#define _ScreeenWidth 640
#define _ScreenHeight 480

#define _ScreenCenter _ScreeenWidth/2,_ScreenHeight/2

#define _Length 30

#define turn(r) cvs.Turn(r)

Canvas cvs(_ScreeenWidth, _ScreenHeight, "Canvas");

void drawA(GLbyte _loop)
{
#define _LengthA 30
#define _LengthB 20
#define _LengthC 10

#define aDrawA cvs.Forward(_LengthA,1)
#define aDrawB cvs.Forward(_LengthB,1)
#define aDrawC cvs.Forward(_LengthC,1)

	cvs.MoveTo(0,10);

	for (GLbyte i = 0; i < _loop;i++)
	{
		cvs.TurnTo(0);

		aDrawA; turn(90);
		aDrawB; turn(90);
		aDrawC; turn(90);
		aDrawC; turn(-90);

		aDrawC; turn(-90);
		aDrawB; turn(-90);
		aDrawA; turn(-90);
		aDrawA;
	}
}
void polyspiral(GLfloat _len,GLfloat _angle,GLfloat _inc,GLint _num)
{
	cvs.MoveTo(_ScreenCenter);
	cvs.TurnTo(0);

	for (GLint i = 0; i < _num; i++)
	{
		cvs.Forward(_len + i*_inc, 1);
		cvs.Turn(_angle);
	}
}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clear the screen

	glColor3f(1.0, .0, .0);
	drawA(30);

	glColor3f(.0, 1.0, .0);
	polyspiral(3, 30, 3, 30);

	glutSwapBuffers();	// swap the color buffer

}
void main(int argc, char **argv)
{
	glColor3f(.9, .0, .0);
	glClearColor(.3, .3, .3, 1);

	glutDisplayFunc(display);


	glutMainLoop();
}

