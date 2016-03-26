#include "..\OpenGL\inc\glus.h"
#pragma comment(lib,"../Debug/opengl.lib")
#include <stdio.h>


void dispaly(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1, 0, 0);

	GlusLine2f		l = { { 0, 0 }, { 30, 30 } };
	GlusVertex2f	pc = { 30, 0 }, p, n = { 30, 30 };
	glusLineHit2f(&l, &pc, &n, &p);
	


	glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP);
	{
		glVertex2fv(&l.A);
		glVertex2fv(&l.B);
		glVertex2fv(&pc);
	}
	glEnd();


	glFlush();
}

void glusInit()
{
	glusInitWin(300, 100, 600, 400, "glut1", GLUT_SINGLE | GLUT_RGB);
	
	glClearColor(0.3, 0.3, 0.3, 1.0);
	glColor3f(1, 1, 1);
	glPointSize(2.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 60, 0, 40);


	glutDisplayFunc(dispaly);

	
}   