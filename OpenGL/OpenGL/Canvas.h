#pragma once
#include"Glut\freeglut.h"
#include <math.h>

#include "circle.h"


#ifndef null
#define null {0}
#endif

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

//----------------Point----------------------
typedef struct _Point2f
{
	GLfloat X, Y;
}Glus_Point2f;



typedef GLfloat(*Glus_Polar_Func)(GLfloat _theta);
typedef struct _Glus_Polar_Curve
{
	Glus_Point2f Center;
	GLfloat Angle_Start, Angle_Sweep;
	Glus_Polar_Func F;
	GLbyte Count;
}Glus_Polar_Curve;
void glus_Draw_Polar_Curve2f(Glus_Polar_Curve *_curve);

typedef struct _Glus_Line2f
{
	Glus_Point2f P1, P2;
}Glus_Line2f;

typedef struct _Glus_Rect2f
{
	Glus_Point2f LeftTop,RightBottom;
};
//================Point======================

//----------------Curve----------------------

//--------for c----------
typedef GLfloat(*GlusCurveFunc)(GLfloat _t);
	

// curve
typedef struct _Glus_Curve2f
{
	Glus_Point2f *Points;
	GLint Count;
}Glus_Curve2f;

// ellipse
typedef struct _Glus_Ellipse
{
	Glus_Point2f Center;
	GLfloat W, H;
	GLfloat Angle_Start, Angle_Sweep;
	GLint Count;

}Glus_Ellipse;

void glusCurve_Ellipse(Glus_Ellipse *_ellipse);
void glus_Hyperbola(Glus_Ellipse *_hyperbolar);
void glus_Curve(Glus_Curve2f *_curve);
void glus_Ellipse_Super(Glus_Ellipse *_ellipse, GLfloat _n);
//=========for c===========

#ifdef __cplusplus
class Curve:Shape
{
public:
	void Draw();
protected:
private:
};
#endif
//================Curve======================
