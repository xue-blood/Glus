#include "Canvas.h"

#include <math.h>
#include <stdio.h>


#define _ScreeenWidth 640
#define _ScreenHeight 480

#define _ScreenCenter _ScreeenWidth/2,_ScreenHeight/2

#define _Point_Num 255

Canvas cvs(_ScreeenWidth, _ScreenHeight, "Canvas");

#define glusCircle(_center,_radius) glusArc(_center,_radius,0,360)

void glusArc(Point2f _cen, GLfloat _radius, GLfloat _startAng, GLfloat _sweep)
{
	const GLbyte n = 30;
	GLfloat angle = _startAng*_Rad;
	GLfloat inc = _sweep*_Rad / n;

	GLfloat cx = _cen.GetX(), cy = _cen.GetY();

	cvs.MoveTo(cx + _radius*cos(angle), cy + _radius*sin(angle));
	for (GLbyte i = 0; i <= n; i++, angle += inc)
		cvs.LineTo(cx + _radius*cos(angle), cy + _radius*sin(angle));

}
void glusCircleCenPoint(Point2f _c, Point2f _p)
{
	// get the radius of arc
	GLfloat radius = sqrt((_c.GetY() - _p.GetY())*(_c.GetY() - _p.GetY())
		+ (_p.GetX() - _c.GetX())*(_p.GetX() - _c.GetX()));

	glusArc(_c, radius, 0, 360);
}

// draw a circle which pass three  points special
void glusCirclePoint3(Point2f _p1, Point2f _p2, Point2f _p3)
{
	GLfloat ang1, ang2;
	Point2f c1, c2;

	// compute the center point of two line
	// c1 between p1 and p2
	c1.SetX((_p1.GetX() + _p2.GetX()) / 2);
	c1.SetY((_p1.GetY() + _p2.GetY()) / 2);

	// c2 between p2 and p3
	c2.SetX((_p3.GetX() + _p2.GetX()) / 2);
	c2.SetY((_p3.GetY() + _p2.GetY()) / 2);

	GLfloat k1, k2, b1, b2;
	// k1 bellow to p1 and p2
	k1 = -(_p2.GetX() - _p1.GetX()) / (_p2.GetY() - _p1.GetY());
	k2 = -(_p2.GetX() - _p3.GetX()) / (_p2.GetY() - _p3.GetY());
	b1 = c1.GetY() - k1*c1.GetX();
	b2 = c2.GetY() - k2*c2.GetX();

	// now compute the center of circle 
	Point2f center;
	center.SetX((b2 - b1) / (k1 - k2));
	center.SetY(k1*center.GetX() + b1);

	GLfloat radius = sqrt((_p1.GetX() - center.GetX())*(_p1.GetX() - center.GetX()) +
		(_p1.GetY() - center.GetY())*(_p1.GetY() - center.GetY()));

	glusCircle(center, radius);

}

void blendArc(Point2f _p, GLfloat _radius1, GLfloat _sweep, GLfloat _radius2)
{
	// compute the center for arc
	Point2f c1, c2;
	c1.SetX(_p.GetX() - _radius1*cosf((90 - _sweep / 2) * _Rad));
	c1.SetY(_p.GetY() + _radius1*sinf((90 - _sweep / 2) * _Rad));

	// draw the right half arc
	glLineWidth(6); glColor3f(1.0, .3, .5);
	glusArc(c1, _radius1, _sweep / 2 - 90, -_sweep);
	glLineWidth(1); glColor3f(.6, .7, .4);
	glusCircle(c1, _radius1);


	// draw the left part
	c2.SetX(c1.GetX() - (_radius1 + _radius2)*cos((90 - _sweep / 2) * _Rad));
	c2.SetY(c1.GetY() - (_radius1 + _radius2)*sin((90 - _sweep / 2)*_Rad));
	glLineWidth(6); glColor3f(1.0, .3, .5);
	glusArc(c2, _radius2, 90 - _sweep / 2, _sweep);
	glLineWidth(1); glColor3f(.6, .7, .4);
	glusCircle(c2, _radius2);

	// connect the center
	glColor3f(.3, .8, .9);
	glLineWidth(3);
	cvs.MoveTo(_p.GetX(), _p.GetY());
	cvs.LineTo(c1.GetX(), c1.GetY());
	cvs.LineTo(c2.GetX(), c2.GetY());
}

void philosophy(Point2f _center, GLfloat _radius)
{
	// draw the outter circle
	glusCircle(_center, _radius);

	// draw the two small circle
	Point2f c;
	c.SetX(_center.GetX() - _radius / 2);
	c.SetY(_center.GetY());
	glusCircle(c, _radius / 10);
	c.SetX(_center.GetX() + _radius / 2);
	glusCircle(c, _radius / 10);

	// draw the blend arc
	c.SetX(_center.GetX() + _radius);
	blendArc(c, _radius / 2, 180, _radius / 2);
}

void sevenPennies(Point2f _center, GLfloat _radius)
{
	// draw the center circle
	glusCircle(_center, _radius / 3);

	Point2f c;
	// draw the other six circles
	for (GLbyte i = 0; i < 6; i++)
	{
		// compute the center
		c.SetX(_center.GetX() + _radius * 2 / 3 * cosf(60 * i*_Rad));
		c.SetY(_center.GetY() + _radius * 2 / 3 * sinf(60 * i*_Rad));

		// draw it
		glusCircle(c, _radius / 3);
	}
}


void glusRoundRect(Point2f _leftTop,GLfloat _w,GLfloat _h,GLfloat _g)
{
	GLfloat radius = _g*_w;
	Point2f p;

	// draw the rect without corners
	// top
	cvs.MoveTo(_leftTop.GetX() + radius, _leftTop.GetY());
	cvs.LineTo(_leftTop.GetX() + _w-radius, _leftTop.GetY());
	cvs.GetCP(&p); p.SetY(p.GetY() - radius);
	glusArc(p, radius, 90, -90);
	
	// right 
	cvs.LineTo(_leftTop.GetX() + _w, _leftTop.GetY() -_h +radius);
	cvs.GetCP(&p); p.SetX(p.GetX() - radius);
	glusArc(p, radius, 0, -90);

	// bottom
	cvs.LineTo(_leftTop.GetX() + radius, _leftTop.GetY() - _h);
	cvs.GetCP(&p); p.SetY(p.GetY()+ radius);
	glusArc(p, radius, -90, -90);

	// left 
	cvs.LineTo(_leftTop.GetX(), _leftTop.GetY() - radius);
	cvs.GetCP(&p); p.SetX(p.GetX() + radius);
	glusArc(p, radius, 180, -90);

	
}

void glusNGon(Point2f _center, GLfloat _radius,GLbyte _n,GLfloat _startAng,Point2f *_buff)
{
	GLfloat ang_inc = 360 / _n*_Rad;
	_startAng *= _Rad;

	Point2f p;
	p.SetX(_center.GetX() + _radius*cos(_startAng));
	p.SetY(_center.GetY() + _radius*sin(_startAng));
	cvs.MoveTo(p.GetX(), p.GetY());

	for (GLbyte i=0;i<_n;i++)
	{
		if(_buff)
			_buff[i] = p;
		_startAng += ang_inc;

		p.SetX(_center.GetX() + _radius*cos(_startAng));
		p.SetY(_center.GetY() + _radius*sin(_startAng));
		cvs.LineTo(p.GetX(), p.GetY());
	}
}
void involveArcs(Point2f _center,GLfloat _radius,GLbyte _n)
{
	Point2f *points = new Point2f[_n];
	glusNGon(_center, _radius, _n, 90,points);

	GLfloat radius = sqrt((points->GetX() - points[1].GetX())*(points->GetX() - points[1].GetX())+
		(points->GetY() - points[1].GetY())*(points->GetY() - points[1].GetY()));

	GLfloat ang_inc = 180 * (_n - 2) / _n;
	GLfloat ang_sweep = -ang_inc;
	GLfloat ang_start = -(180 - ang_inc) / 2;
	ang_inc = 180 - ang_inc;

	for (GLbyte i=0;i<_n;i++)
	{
		glusArc(points[i], radius, ang_start, ang_sweep);
		ang_start += ang_inc;
	}

	delete[] points;
}

// draw a teardrop
void tearDrop(Point2f _root, GLfloat _len, GLfloat _ang_sweep,GLfloat _ang)
{
	cvs.MoveTo(_root.GetX(),_root.GetY());
	cvs.TurnTo(_ang - _ang_sweep/2);
	cvs.Forward(_len, 1);

	cvs.MoveTo(_root.GetX(),_root.GetY());
	cvs.TurnTo(_ang + _ang_sweep/2);
	cvs.Forward(_len, 1);

	Point2f cen;
	GLfloat radius = _len*tanf(_ang_sweep / 2 * _Rad);
	GLfloat l = sqrt(radius*radius + _len*_len);
	cen.SetX(_root.GetX() + l*cosf(_ang*_Rad));
	cen.SetY(_root.GetY() + l*sinf(_ang*_Rad));

	GLfloat ang_arc_start, ang_arc_sweep;
	ang_arc_sweep = (180 - (90 - _ang_sweep / 2));
	ang_arc_start = _ang - ang_arc_sweep;

	glusArc(cen, radius,ang_arc_start,2*ang_arc_sweep);
}
void tearDrops(Point2f _center,
	GLfloat _len,
	GLfloat _poly_radius,GLfloat _polygon_n,
	GLfloat _arc_radius,GLfloat _ang_start)
{
	if (_polygon_n == 0)
		return;

	GLfloat angle = atanf(_arc_radius / _len) / _Rad; // the angle of one teardrop

	Point2f *points = new Point2f[_polygon_n];

	if (_poly_radius > 0)
		glusNGon(_center, _poly_radius, _polygon_n, _ang_start, points);
	else
		memset(points, 0, _polygon_n * sizeof(Point2f));

	GLfloat ang_inc = 360 / _polygon_n;

	GLfloat ang = atanf(_arc_radius / _len)/_Rad;
	_ang_start += (180-(180-360/_polygon_n)/2)-ang;
	  
	for (GLbyte i = 0; i < _polygon_n; i++)
	{
		tearDrop(points[i], _len, _arc_radius, _ang_start);
		_ang_start += ang_inc;
	}

//	if(_poly_radius !=0)
	delete[] points;
}

// draw a pie
void glusPie(Point2f _root, GLfloat _radius, GLfloat _ang_start,GLfloat _ang_sweep)
{
	// draw arc first now
	glusArc(_root, _radius, _ang_start, _ang_sweep);

	// and then draw two side
	// first
	cvs.MoveTo(_root.GetX(), _root.GetY());
	cvs.TurnTo(_ang_start);
	cvs.Forward(_radius, 1);

	//seconf
	cvs.MoveTo(_root.GetX(), _root.GetY());
	cvs.TurnTo(_ang_start+_ang_sweep);
	cvs.Forward(_radius, 1);

}

void glusPies(Point2f _center, GLfloat _radius,GLbyte _n,GLfloat *_angles,GLfloat *offsets)
{
	if (_angles == 0 || _n == 0)
		return;

	GLfloat ang = 0, ang_sweep = 0;
	Point2f p;

	for (GLbyte i = 0; i < _n; i++)
	{
		ang_sweep = _angles[i];
		if (offsets)
		{
			p.SetX(_center.GetX() + offsets[i] * cosf((ang+ang_sweep/2) * _Rad));
			p.SetY(_center.GetY() + offsets[i] * sinf((ang + ang_sweep / 2)* _Rad));
		}
		else
			p = _center;
		glusPie(p, _radius, ang, ang_sweep);
		ang += _angles[i];

	}
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(.7, .8, .3);
	//Point2f p = { 300,210 };

	//Point2f cen = { 300,300 };
	//glusArc(cen, 100, 30, 69);

	//glusCircleCenPoint(cen, p);

	//philosophy(p, 210);

	//sevenPennies(p,210);

	//glusRoundRect(p, 300, 100, .1);

	//involveArcs(p, 180, 5);

// 	size_t i = 3;
// 	for (; i <18; i++)
// 	{
// 		glClear(GL_COLOR_BUFFER_BIT);
// 		tearDrops(p, 100, 30, i, 30, 0);
// 		Sleep(30);
// 		glutSwapBuffers();
// 	}
// 	for (; i >=3; i--)
// 	{
// 		glClear(GL_COLOR_BUFFER_BIT);
// 		tearDrops(p, 100, 30, i, 30, 0);
// 		Sleep(30);
// 		glutSwapBuffers();
// 	}

// 	GLfloat angles[6] =
// 	{
// 		100,20,30,30,60,120
// 	};
// 	GLfloat offsets[6] =
// 	{
// 		10,0,30,0,0,50
// 	};
// 	glusPies(p, 210, 6, angles,offsets);

	Point2f p[3] =
	{
		{100,100},
		{200,200},
		{300,100}
	};
	glusCirclePoint3(p[0], p[1], p[2]);

	glutSwapBuffers();
}
void main(int argc, char **argv)
{
	glColor3f(.9, .0, .0);
	glClearColor(.3, .3, .3, 1);

	glutDisplayFunc(display);

	
	glutMainLoop();
}

