
#include "源.h"

void eye()
{
	Glus_Ellipse ellipse = null;
	ellipse.Center = { 300,200 };
	ellipse.H = 50;
	ellipse.W = 50;

	glusCurve_Ellipse(&ellipse);

	ellipse.W *= 2;
	glusCurve_Ellipse(&ellipse);

	ellipse.H *= 2;
	glusCurve_Ellipse(&ellipse);

	ellipse.W *= 2;
	glusCurve_Ellipse(&ellipse);



}

void superellipse()
{
	Glus_Ellipse ellipse = null;
	ellipse.Center = { 300,200 };
	ellipse.H = 150;
	ellipse.W = 150;

	glus_Ellipse_Super(&ellipse, 10);
	return;

	for (GLfloat f=.5;f<3;f+=.5)
	{
		glus_Ellipse_Super(&ellipse, f);
	}
}


#define _N 1
GLfloat cardioid(GLfloat _t)
{
	return  45*(1+cosf(_t));
}
GLfloat Rose(GLfloat _t)
{
	return 90*cos(_N*_t);
}
GLfloat Archimedean(GLfloat _t)
{
	return .3*_t;
}

#define _LK 10
#define _LA .3063489
GLfloat goldenCut(GLfloat _t)
{
	return _LK* exp(_LA*_t);
}

#define _L_La .85
#define _L_Func(f) 4 * _L_La*f*(1 - f)
#define _L_Start .1
void logisticMap()
{
	static GLfloat x=0;

	//set the world
	cvs.SetWorld(0, x, 0, x);

	glColor3f(1, 1, 1);
	// draw the y=x
	cvs.MoveTo(0, 0);
	cvs.LineTo(1, 1);

	// draw the parabola
	glBegin(GL_LINE_STRIP);
	{
		for (GLfloat i=0;i<1;i+=.01)
		{
			glVertex2f(i, _L_Func(i));
		}
	}
	glEnd();

	glColor3f(.3, .8, .7);
	glBegin(GL_LINE_STRIP);
	// draw logistic map now
	GLfloat p = _L_Start, f = 0;
	for(GLubyte i=0;i<255;i++)
	{
		//the vertical line
		glVertex2f(p, f);
		f = _L_Func(p);
		glVertex2f(p, f);


		if(absf(p-f)<.001)
			break;

		// the horizontial line
		p = f;
		f = p;

		if(p>x)
			x = p*1.2;
	}
	glEnd();

}

//arc
void arcRound(Glus_Point2f _center, GLfloat _w, GLfloat _h)
{
	cvs.MoveTo(_center.X - _w, _center.Y);
	cvs.LineRel(0, -_h);

	cvs.LineRel(2 * _w, 0);
	cvs.LineRel(0, _h);

	Glus_Ellipse arc=null;
	arc.Center = _center;
	arc.Angle_Start = 0;
	arc.Angle_Sweep = 180;
	arc.H = arc.W = _w;
	
	glusCurve_Ellipse(&arc);
}
void arcPointed(Glus_Point2f _center, GLfloat _w, GLfloat _h)
{
	cvs.MoveTo(_center.X - _w, _center.Y);
	cvs.LineRel(0, -_h);

	cvs.LineRel(2 * _w, 0);
	cvs.LineRel(0, _h);

	GLfloat theta = acos(1.0 / 2) / _Rad;
	Glus_Ellipse arc = null;

	arc.H = arc.W = 2 * _w;
	arc.Center = _center;

	// draw left arc
	arc.Angle_Sweep = theta;
	arc.Center.X -= _w;
	arc.Angle_Start = 0;
	glusCurve_Ellipse(&arc);

	// the right one 
	arc.Center.X += 2 * _w;
	arc.Angle_Start = 180-theta;


	glusCurve_Ellipse(&arc);
}
void arcOgee(Glus_Point2f _center, GLfloat _w, GLfloat _h, GLfloat _fraction)
{
	cvs.MoveTo(_center.X - _w, _center.Y);
	cvs.LineRel(0, -_h);

	cvs.LineRel(2 * _w, 0);
	cvs.LineRel(0, _h);
	
	GLfloat ang = 90-asin(_fraction/(1+_fraction)) / _Rad;

	Glus_Ellipse cir = null;
	cir.Angle_Sweep = ang;
	
	// right bottom arcs 
	cir.Angle_Start = 0;
	cir.Center = _center;
	cir.H = cir.W = _w;
	glusCurve_Ellipse(&cir);

	//left bottom
	cir.Angle_Start = 180;
	cir.Angle_Sweep = -ang;
	glusCurve_Ellipse(&cir);

	// right top arcs
	cir.Angle_Sweep = ang;
	cir.H = cir.W = _fraction*_w;
	cir.Center.X += (1 + _fraction)*_w*cosaf(ang);
	cir.Center.Y += (1 + _fraction)*_w*sinaf(ang);
	glusCurve_Ellipse(&cir);

	// left top
	cir.Center.X -= 2 * _fraction*_w;
	cir.Angle_Start = 0;
	cir.Angle_Sweep = -ang;
	glusCurve_Ellipse(&cir);

}

#define _TruchetNum 9
GLubyte TruchetPlot[_TruchetNum * _TruchetNum];
void truchetData()
{
	for (GLubyte i = 0; i < _TruchetNum * _TruchetNum; i++)
	{
		TruchetPlot[i] = rand() % 2;
	}
}
void truchetTile(GLbyte _b,GLfloat _w)
{
	Glus_Ellipse arc = null;

	if (_b)
	{
		arc.Center = { _w,0 };
		arc.H = arc.W = _w/2;
		arc.Angle_Start = 90;
		arc.Angle_Sweep = 90;
		glusCurve_Ellipse(&arc);

		arc.Center = { 0,_w };
		arc.Angle_Start = 0;
		arc.Angle_Sweep = -90;
		glusCurve_Ellipse(&arc);
	}
	else
	{
		arc.H = arc.W = _w / 2;
		arc.Angle_Sweep = 90;
		glusCurve_Ellipse(&arc);

		arc.Center = { _w,_w };
		arc.Angle_Start = 180;
		arc.Angle_Sweep = 90;
		glusCurve_Ellipse(&arc);
	}
}
void truchetDraw(Glus_Point2f _left_bottom,GLfloat _width)
{
	cvs.SetWorld(0, _width, 0, _width);

	for (GLbyte i=0;i<_TruchetNum;i++)
	{
		for (GLbyte j=0;j<_TruchetNum;j++)
		{
			cvs.SetViewport(_left_bottom.X + j*_width, _left_bottom.X + (1 + j)*_width,
				_left_bottom.Y + i*_width, _left_bottom.Y + (i + 1)*_width);
			truchetTile(TruchetPlot[8 * i + j], _width);
		}
	}
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	Glus_Polar_Curve curve=null;
	curve.Center = { 300,200 };
	//curve.F = cardioid;
	//curve.F = Rose;
	//curve.F = Archimedean;
	//curve.F = goldenCut;
	//curve.Angle_Sweep = 360*4;
	//curve.Count = 100;

	//glus_Draw_Polar_Curve2f(&curve);
	
	//superellipse();

	//logisticMap();

// 	Glus_Point2f c = { 300,200 };
// 	arcRound(c, 90, 60);
//	arcPointed(c, 90, 60);
	//arcOgee(c, 90, 60,2.0/3);

	// truchet
//	truchetData();
//	truchetDraw({ 0,0 }, 60);

	glutSwapBuffers();


}
void main(int argc, char **argv)
{
	glColor3f(.9, .0, .0);
	glClearColor(.3, .3, .3, 1);

	glutDisplayFunc(display);

	
	glutMainLoop();
}

