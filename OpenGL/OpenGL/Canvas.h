#pragma once
#include"freeglut.h"
#include <math.h>

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
private:
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
class Canvas
{
public:
	Canvas(GLint _width, GLint _height, char *_windowTitle);
	void SetWorld(GLfloat _left, GLfloat _right, GLfloat _bottom, GLfloat _top);
	void SetViewport(GLint _left, GLint _right, GLint _bottom, GLint _top);

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
protected:
private:
	Point2f CP;
	GLfloat CD;
	Rect4i Viewport;
	Rect4f World;
};