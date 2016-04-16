#pragma once
#include"Glut\freeglut.h"
#include <math.h>

#define _PI (3.14159265)
#define _2PI (2*_PI)
#define _Rad (_PI/180)

#define _FI 1.618033989

class Shape
{
public:
	virtual void Draw()=0;
};
class Point2f:Shape
{
public:
	Point2f() :Point2f(0, 0) {};
	Point2f(GLfloat _x, GLfloat _y) :x(_x), y(_y) {};

	void operator =(Point2f &p);
	void SetX(float _x) { x = _x; }
	void SetY(float _y) { y = _y; }

	GLfloat GetX() { return x; }
	GLfloat GetY() { return y; }

	void Draw()
	{
		glBegin(GL_POINTS);
		{
			glVertex2f(x, y);
		}
		glEnd();
	}
protected:
	GLfloat x, y;
};

class Rect4i :Shape
{
public:
	Rect4i() :Rect4i(0, 0, 100, 100) {};
	Rect4i(GLint _left, GLint _bottom, GLint _right, GLint _top) :
		left(_left),bottom(_bottom),right(_right),top(_top){};

	void Set(GLint _left, GLint _bottom, GLint _right, GLint _top)
	{
		left=_left, bottom=_bottom, right=_right, top=_top;
	}
	void Draw()
	{
		glRecti(left, bottom, right, top);
	}
private:
	GLint left, bottom, right, top;
};
class Rect4f :Shape
{
public:
	Rect4f() :Rect4f(0, 0, 100, 100) {};
	Rect4f(GLfloat _left, GLfloat _bottom, GLfloat _right, GLfloat _top) :
		left(_left), bottom(_bottom), right(_right), top(_top) {};

	void Set(GLfloat _left, GLfloat _bottom, GLfloat _right, GLfloat _top)
	{
		left = _left, bottom = _bottom, right = _right, top = _top;
	}
	void Draw()
	{
		glRectf(left, bottom, right, top);
	}
private:
	GLfloat left, bottom, right, top;
};
class Canvas:Shape
{
public:
	Canvas(GLint _width, GLint _height, char *_windowTitle);
	void SetWorld(GLfloat _left, GLfloat _right, GLfloat _bottom, GLfloat _top);
	void SetViewport(GLint _left, GLint _right, GLint _bottom, GLint _top);

	void GetCP(Point2f *_p);
	GLfloat GetCD();
	
	// draw line with absolute coordinate
	void LineTo(GLfloat _x, GLfloat _y);
	void MoveTo(GLfloat _x, GLfloat _y);
	

	// draw line using relative coordinate 
	void LineRel(GLfloat _x, GLfloat _y);
	void MoveRel(GLfloat _x, GLfloat _y);
	
	// draw line using Polar coordinate 
	void TurnTo(GLfloat _angle);
	void Turn(GLfloat _angle);
	void Forward(GLfloat _dist, GLboolean _visible);

	void Draw();
protected:
private:
	Point2f CP;
	GLfloat CD;
	Rect4i Viewport;
	Rect4f World;
};