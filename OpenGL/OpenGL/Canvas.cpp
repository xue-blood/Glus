#include "Canvas.h"

void Canvas::Draw()
{
	return;
}
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

void Canvas::GetCP(Point2f *_p)
{
	if ((VOID*)_p == NULL)
		return;
	_p->SetX(CP.GetX());
	_p->SetY(CP.GetY());
}

GLfloat Canvas::GetCD()
{
	return this->CD;
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

void Point2f::operator=(Point2f & p)
{
	if (&p != NULL)
		memcpy_s(this, sizeof(Point2f), &p, sizeof(Point2f));
}




//----------------Curve----------------------


void glus_Draw_Polar_Curve2f(Glus_Polar_Curve * _curve)
{
	if (_curve == NULL||
		_curve->F==NULL)
		return;

	if (_curve->Angle_Sweep == 0)
		_curve->Angle_Sweep = 360;
	if (_curve->Count == 0)
		_curve->Count = 100;

	GLfloat ang_inc = _curve->Angle_Sweep / _curve->Count;
	GLfloat ang_end = _curve->Angle_Start + _curve->Angle_Sweep;

	glBegin(GL_LINE_STRIP);
	{
		for (GLfloat t=_curve->Angle_Start;t<ang_end;t+=ang_inc)
		{
			GLfloat f = _curve->F(t*_Rad);

			glVertex2f(_curve->Center.X + f*cosaf(t),
				_curve->Center.Y + f*sinaf(t));


		}
	}
	glEnd();
}

void glusCurve_Ellipse(Glus_Ellipse *_ellipse)
{
	if (_ellipse == NULL)
		return;

	if (_ellipse->Count == 0)
		_ellipse->Count = 100;

	if (_ellipse->Angle_Sweep == 0)
		_ellipse->Angle_Sweep = 360;

	GLfloat ang_start = _ellipse->Angle_Sweep>0?_ellipse->Angle_Start:
		_ellipse->Angle_Start+_ellipse->Angle_Sweep;
	GLfloat ang_sweep =absf(_ellipse->Angle_Sweep);
	GLfloat ang_inc = ang_sweep / _ellipse->Count;
	


	glBegin(GL_LINE_STRIP);
	{
		for (GLfloat t = ang_start;
			t < ang_start+ang_sweep;
			t += ang_inc)
		{
			glVertex2f(_ellipse->Center.X + _ellipse->W*cosaf(t),
				_ellipse->Center.Y + _ellipse->H*sinaf(t));
		}
	}
	glEnd();
}
void glus_Hyperbola(Glus_Ellipse * _hyperbolar)
{
	if (_hyperbolar == NULL)
		return;

	if (_hyperbolar->Count == 0)
		_hyperbolar->Count = 100;

	if (_hyperbolar->Angle_Sweep == 0)
		_hyperbolar->Angle_Sweep = 360;

	GLfloat ang_inc = _hyperbolar->Angle_Sweep / _hyperbolar->Count;

	glBegin(GL_LINE_STRIP);
	{
		for (GLfloat t = _hyperbolar->Angle_Start;
			t < _hyperbolar->Angle_Sweep;
			t += ang_inc)
		{
			glVertex2f(_hyperbolar->Center.X + _hyperbolar->W*secaf(t),
				_hyperbolar->Center.Y + _hyperbolar->H*tanaf(t));
		}
	}
	glEnd();
}
void glus_Curve(Glus_Curve2f * _curve)
{
	if (_curve == NULL ||
		_curve->Points == NULL ||
		_curve->Count == 0)
		return;

	glBegin(GL_LINE_STRIP);
	{
		for (GLint i=0;i<_curve->Count;i++)
		{
			glVertex2f(_curve->Points[i].X, _curve->Points[i].Y);
		}
	}
	glEnd();
}
void glus_Ellipse_Super(Glus_Ellipse * _ellipse, GLfloat _n)
{
	if (_ellipse == NULL)
		return;

	if (_ellipse->Count == 0)
		_ellipse->Count = 100;

	if (_ellipse->Angle_Sweep == 0)
		_ellipse->Angle_Sweep = 360;

	GLfloat ang_inc = _ellipse->Angle_Sweep / _ellipse->Count;

	glBegin(GL_LINE_STRIP);
	{
		GLfloat ca, sa,cb,sb;
		for (GLfloat t = _ellipse->Angle_Start;
			t < _ellipse->Angle_Sweep;
			t += ang_inc)
		{
			ca = cosaf(t), sa = sinaf(t);
			cb = powf(ca, 2 / _n - 1), sb = powf(sa, 2 / _n - 1);

			glVertex2f(_ellipse->Center.X + _ellipse->W*ca*absf(cb),
				_ellipse->Center.Y + _ellipse->H*sa*absf(sb));
		}
	}
	glEnd();
}
//================Curve======================

void Curve::Draw()
{
}
