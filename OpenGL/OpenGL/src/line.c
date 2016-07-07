#include "..\inc\glus.h"

/*
//  [7/6/2016 Tld]
	glusLIntersect	change,convert to parameter PGlusLine to PGlusVector
*/
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
_In_	PGlusVector	_laa,
_In_	PGlusVector	_lab,
_In_	PGlusVector	_lba,
_In_	PGlusVector	_lbb,
_Out_	PGlusVector _p)
{
	assertp(_laa && _lab && _lba && _lbb); 
	assertp(_p);
	
	//
	// compute the vectors 
	//
	GlusVector	vb = glusVector3fv(_laa, _lab),
				vd = glusVector3fv(_lba, _lbb),
				vc = glusVector3fv(_laa, _lba);
	
	// compute the perpendicular vector
	GlusVector vdn, vbn;
	glusVNormal(&vb, &vbn);
	glusVNormal(&vd, &vdn);
	//
	// compute the parameter and the vector
	//
	GLdouble	r, s, t;
	r = glusVDotPro((&vb), (&vdn));

	// are lines be parallel
	if (r == 0)
		return Intersect_Parallel;

	t = glusVDotPro((&vc), (&vdn)) / r;

	//
	// compute s 
	// 
	glusVOpposite(&vc);
	s = glusVDotPro(&vc, &vbn) / glusVDotPro(&vd, &vbn);
	
	glusVAdd(_laa, 1, (&vb), t, _p);
	_p->V = 1;

	// is the intersection exsit
	if (t > 1 || t < 0 || s >1 || s < 0)
		return Intersect_Not_Exsit;
	return Intersect_Exsit;
}

