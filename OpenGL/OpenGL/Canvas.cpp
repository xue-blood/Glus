#include "Canvas.h"

Canvas::Canvas(GLint _width, GLint _height, char * _windowTitle)
{
	char *argv[1];
	char dummyString[8];
	argv[0] = dummyString;
	int argc = 1;
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(_width, _height);
	glutInitWindowPosition(300, 100);
	glutCreateWindow(_windowTitle);

	SetWorld(0, _width, 0, _height);
	SetViewport(0, _width, 0, _height);
	CP.SetX(0.0);
	CP.SetY(0.0);
}

void Canvas::SetWorld(GLfloat _left, GLfloat _right, GLfloat _bottom, GLfloat _top)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(_left, _right, _bottom, _top);
}

void Canvas::SetViewport(GLint _left, GLint _right, GLint _bottom, GLint _top)
{
	glViewport(_left, _bottom, _right - _left, _top - _bottom);
}

void Canvas::LineTo(GLfloat _x, GLfloat _y)
{
	glBegin(GL_LINES);
	{
		glVertex2f(CP.GetX(), CP.GetY());
		glVertex2f(_x, _y);
	}
	glEnd();
	glFlush();
	CP.SetX(_x), CP.SetY(_y);
	
}

void Canvas::MoveTo(GLfloat _x, GLfloat _y)
{
	CP.SetX(_x), CP.SetY(_y);
}

void Canvas::LineRel(GLfloat _x, GLfloat _y)
{
	glBegin(GL_LINES);
	{
		glVertex2i(CP.GetX(), CP.GetY());
		glVertex2i(CP.GetX() + _x, CP.GetY() + _y);
	}
	glEnd();


	CP.SetX(CP.GetX() + _x);
	CP.SetY(CP.GetY() + _y);
}

void Canvas::MoveRel(GLfloat _x, GLfloat _y)
{
	CP.SetX(CP.GetX() + _x);
	CP.SetY(CP.GetY() + _y);
}

void Canvas::TurnTo(GLfloat _angle)
{
	CD = _angle;
}

void Canvas::Turn(GLfloat _angle)
{
	CD += _angle;
}

void Canvas::Forward(GLfloat _dist, GLboolean _visible)
{
	GLfloat x, y;
	x = CP.GetX() + _dist*cos(CD*3.14 / 180);
	y = CP.GetY() + _dist*sin(CD*3.14 / 180);

	if (_visible)
		LineTo(x, y);
	else
		MoveTo(x, y);
}
