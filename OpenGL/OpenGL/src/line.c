#include "..\inc\glus.h"

void	
glusLDraw(
_In_	PGlusLine	_pLine)
{
	assertp(_pLine);

	glBegin(GL_LINES);
	{
		glVertex3dv((pGLdouble)&_pLine->A);
		glVertex3dv((pGLdouble)&_pLine->B);
	}
	glEnd();
}

void
glusDrawCoord()
{
	glBegin(GL_LINES);
	{
		
		glColor3f(1.0, 0, 0); glVertex3i(-100, 0, 0); glVertex3i(100, 0, 0); // x
		glColor3f(0, 1.0, 0); glVertex3i(0, -100, 0); glVertex3i(0, 100, 0); // y
		glColor3f(0, 0, 1.0); glVertex3i(0, 0, -100); glVertex3i(0, 0, 100); // z
		
	}
	glEnd();
}

Glus_Intersect
glusLIntersect(
_In_	PGlusLine	_la,
_In_	PGlusLine	_lb,
_Out_	PGlusVector _p)
{
	assertp(_la);
	assertp(_lb);
	assertp(_p);
	
	//
	// compute the vectors 
	//
	GlusVector	vb = glusVector3fv((&_la->A), (&_la->B)),
				vd = glusVector3fv((&_lb->A), (&_lb->B)),
				vc = glusVector3fv((&_la->A), (&_lb->A));
	
	// compute the perpendicular vector
	GlusVector vdn = glusVPerpende((&vd));

	//
	// compute the parameter and the vector
	//
	GLdouble	r, t;
	r = glusVDotPro((&vb), (&vdn));

	// are lines be parallel
	if (r == 0)
		return Intersect_Parallel;

	t = glusVDotPro((&vc), (&vdn)) / r;

	

	glusVAdd((&_la->A), 1, (&vb), t, _p);
	_p->V = 1;

	// is the intersection exsit
	if (t > 1 || t < 0)
		return Intersect_Not_Exsit;
	return Intersect_Exsit;
}

