#include "Canvas.h"

#include <math.h>
#include <stdio.h>


#define _ScreeenWidth 640
#define _ScreenHeight 480

#define _ScreenCenter _ScreeenWidth/2,_ScreenHeight/2

#define _PI (3.14159265)
#define _2PI (2*_PI)
#define _Rad (_PI/180)

#define _FI 1.618033989


Canvas cvs(_ScreeenWidth, _ScreenHeight, "Canvas");

#define _Point_Num 255


void glusNGon(GLint _n, GLfloat _cx, GLfloat _cy, GLfloat _radius, GLfloat _rot)
{
	if (_n < 3)
		return;

	GLdouble angle = _rot*_Rad; // the original angle 
	GLdouble inc = _2PI / _n;	// the angle to inc for each interation

	cvs.MoveTo(_cx + _radius*cos(angle), _cy + _radius*sin(angle));

	for (GLbyte k = 0; k < _n; k++)
	{
		// draw line to next point
		angle += inc;
		cvs.LineTo(_cx + _radius*cos(angle), _cy + _radius*sin(angle));
	}


}
void glusTurNGon(GLint _n, GLfloat _x, GLfloat _y, GLfloat _len, GLfloat _rot)
{
	if (_n < 3) // the bad gon
		return;

	GLdouble angle = _rot / _Rad;
	GLdouble turn = 360 / _n;

	cvs.MoveTo(_x + _len*cos(angle), _y + _len*sin(angle));
	for (GLbyte i = 0; i < _n; i++)
	{
		cvs.Forward(_len, 1);
		cvs.Turn(turn);
	}
}

enum RosetteType
{
	RosetteNormal,
	RosetteStar,
	RosetteFull
};

void glusRosetteVertex(GLint _n, GLfloat* _buffer, GLfloat _cx, GLfloat _cy, GLfloat _radius, GLfloat _rot)
{
	if (_n < 3)
		return;

	GLfloat angle = _rot*_Rad;
	GLfloat inc = _2PI / _n;

	for (GLint i = 0; i < _n; i++,angle+=inc)
	{
		_buffer[2 * i] = _cx + _radius*cos(angle);
		_buffer[2 * i + 1] = _cy + _radius*sin(angle);
	}
}
void glusRosette(GLint _n, GLfloat* _buffer, RosetteType _type)
{
	if (_buffer[0] == 0)
		return;

	glBegin(GL_LINE_LOOP);

	switch (_type)
	{
	case RosetteNormal:
		for (GLint i = 0; i < _n; i++)
		{
			glVertex2f(_buffer[2 * i], _buffer[2 * i + 1]);
		}

		break;
	case RosetteStar:
	{
		GLint i = 0;
		do
		{
			glVertex2f(_buffer[2 * i], _buffer[2 * i + 1]);
			i = (i + 2) % (_n);
		} while (i != 0);

		if (_n % 2 == 0)
		{
			glEnd();
			glBegin(GL_LINE_LOOP);
			for (i = 1; i < _n;i+=2)
			{
				glVertex2f(_buffer[2 * i], _buffer[2 * i + 1]);
			}
		}
		break;
	}
	case RosetteFull:
		for (GLint i = 0; i < _n;i++)
		{
			for (GLint j = 0; j < _n; j++)
			{
				glVertex2f(_buffer[2 * i], _buffer[2 * i + 1]);
				glVertex2f(_buffer[2 * j], _buffer[2 * j + 1]);

			}
		}
		break;
	default:
		break;
	}
	glEnd();
}

void pentagram(GLfloat _cx, GLfloat _cy, GLfloat _radius)
{
	if (_radius < .01)
		return;

	// draw the wide 
	glusNGon(5, _cx, _cy, _radius, 0);
	
	GLboolean b = true;
	GLfloat points[2 * _Point_Num];

	while (_radius>.01)
	{
		glusRosetteVertex(5,points, _cx, _cy, _radius, b?0:36);
		glusRosette(5, points, RosetteStar);

		_radius /= _FI*_FI;
		b = !b;
	}
}

void congons(GLint _n, GLfloat _cx, GLfloat _cy, GLfloat _radius, GLfloat _fraction, GLfloat _rot)
{
	if (_fraction <= 0 || _n < 3 || _radius <= 0)
		return;

	GLfloat outer[2 * _Point_Num], inner[2 * _Point_Num];
	// get the vertexs
	glusRosetteVertex(_n, outer, _cx, _cy, _radius, _rot);
	glusRosetteVertex(_n, inner, _cx, _cy, _fraction*_radius, _rot+360/_n/2);

	// draw outer n-gon
	glusRosette(_n, outer, RosetteNormal);
	glusNGon(30, _cx, _cy, _radius, _rot);

	glusRosette(_n, inner, RosetteNormal);
	glusNGon(30, _cx, _cy, _fraction*_radius, _rot);

	// draw polygon between outer and inner 
	glBegin(GL_LINE_LOOP);
 	for (GLint i = 0; i < _n;i++)
 	{
 		glVertex2f(outer[2*i], outer[2*i + 1]);
 		glVertex2f(inner[2*i], inner[2*i + 1]);
 
 	}

	glEnd();
}

void patternStar(GLfloat _cx,GLfloat _cy,GLfloat _radius)
{
	if (_radius <= 0)
		return;
	
	GLfloat buffer[10];

	// draw center pentagram
	glusRosetteVertex(5, buffer, _cx, _cy, _radius, 360/5/4);
	glusRosette(5, buffer, RosetteStar);

	// draw middle pentagram arrays
	GLfloat position[20];
	glusRosetteVertex(10, position, _cx, _cy, 1.5*_radius, 360 / 5 / 2);// computer position
	for (GLbyte i = 0; i < 10;i++)
	{
		glusRosetteVertex(5, buffer, position[2*i], position[2 * i+1], .1*_radius, 360 / 5 / 4);
		glusRosette(5, buffer, RosetteStar);
	}
		

	// draw outer pentagram arrays
	glusRosetteVertex(10, position, _cx, _cy, 2*_radius, 360 / 5 /4);// computer position
	for (GLbyte i = 0; i < 10; i++)
	{
		glusRosetteVertex(5, buffer, position[2 * i], position[2 * i + 1], .15*_radius, 360 / 5 / 4);
		glusRosette(5, buffer, RosetteStar);
	}

}

void turtleNgon(GLfloat _x,GLfloat _y,GLfloat _len)
{
	if (_len <= 0)
		return;


	for (GLbyte i = 3; i < 10;i++)
	{
		glusTurNGon(i, _x, _y, _len,0);
	}
}

void esteemedLogo(GLfloat _cx, GLfloat _cy, GLfloat _radius)
{
	if (_radius <= 0)
		return;

	GLfloat center[6];
	// compute center
	glusRosetteVertex(3, center, _cx, _cy, _radius*.1, -30);

#define _Short _radius*.5
#define _Long _radius*.8

	for (GLbyte i = 0; i <3; i++)
	{
		cvs.MoveTo(center[2 * i], center[2 * i + 1]);
		cvs.TurnTo(-90 + i * 120);

		cvs.Forward(_Long, 1);cvs.Turn(60);
		cvs.Forward(_Short,1);cvs.Turn(120);
		cvs.Forward(_Long, 1); cvs.Turn(-60);
		cvs.Forward(_Long, 1); cvs.Turn(120);
		cvs.Forward(_Short, 1); cvs.Turn(60);
		cvs.Forward(_Long, 1);

	}
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
/*
	glColor3f(1.0, .0, .0);
	glusTurNGon(9, 100, 100, 100, 0);

	glColor3f(.0, 1.0, .0);
	glusNGon(6, 100, 300, 100, 0);


	glColor3f(.3, .7, .6);
	pentagram(350, 200, 100);

*/
	glColor3f(.7, .8, .3);
	
// 	for (GLint i = 3; i < 9;i++)
// 	for (GLfloat angle = 0; angle < 360; angle += 3)
// 	{
//  		glClear(GL_COLOR_BUFFER_BIT); // clear the screen
//  
//  		congons(i, 300, 300, 150, .2, 90+angle);
//  		glutSwapBuffers();	// swap the color buffer
//  
//  		Sleep(30);
//  	}

	//patternStar(300, 240, 90);

//	turtleNgon(50,100, 100);

	esteemedLogo(300, 300, 100);
	glutSwapBuffers();
}
void main(int argc, char **argv)
{
	glColor3f(.9, .0, .0);
	glClearColor(.3, .3, .3, 1);

	glutDisplayFunc(display);


	glutMainLoop();
}

