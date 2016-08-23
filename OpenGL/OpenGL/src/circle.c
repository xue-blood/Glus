#include "..\inc\glus.h"


bool
glusCExTri(
_In_	PGlusVector	_pa,
_In_	PGlusVector	_pb,
_In_	PGlusVector	_pc,
_Out_	PGlusCircle _c)
{
	assert(_pa != NULL);
	assert(_pb != NULL);
	assert(_pc != NULL);
	assert(_c != NULL);

	
	//
	// compute vectors
	//
	GlusVector	va = glusVector3fv(_pc, _pa),
				vb = glusVector3fv(_pa, _pb),
				vc = glusVector3fv(_pb, _pc),
				vbn = glusVPerpende((&vb));

	GLdouble	r , t;

	//
	// check is the three point at one line
	//
	r = glusVDotPro(&va, &vbn);
	if (r == 0)
		return false;
	//
	// compute the parameter
	//
	t = glusVDotPro(&vc, &va) / r / 2;

	_c->Center.X = _pa->X + vb.X / 2 + vbn.X*t;
	_c->Center.Y = _pa->Y + vb.Y / 2 + vbn.Y*t;
	_c->Center.Z = _pa->Z + vb.Z / 2 + vbn.Z*t;
	_c->Center.V = 1;
	_c->Radius = glusPDistance(&_c->Center, _pa);

	return true;

}


bool 
glusCGetTangentPoints(
_In_	PGlusVector	_pa,
_In_	PGlusVector	_pb,
_In_	PGlusVector	_pc,
_Out_	PGlusVector	_pr,
_Out_	PGlusVector	_ps,
_Out_	PGlusVector	_pt)
{
	assert(_pa && _pb&&_pc&&_pr&&_ps&&_pt);
	
	if (glusPIs3PointOnLine(_pa, _pb, _pc))
		return false;

	
	// 
	// compute vector for each side
	// 
	GlusVector		va, vb, vc;
	glusVFromPoint(_pc, _pa, &va);
	glusVFromPoint(_pa, _pb, &vb);
	glusVFromPoint(_pb, _pc, &vc);

	// 
	// compute the vector length
	// 
	GLdouble		val, vbl, vcl;
	val = glusVLength(&va);
	vbl = glusVLength(&vb);
	vcl = glusVLength(&vc);

	// 
	// compute the Point distance
	// 
	GLdouble		a, c;
	a = (val + vbl - vcl) / 2;
	c = (val + vcl - vbl) / 2;

	// 
	// now we can get the three tangent points
	// 
	glusVAdd(_pc, 1, &va, c / val, _pr); _pr->V = 1;
	glusVAdd(_pc, 1, &vc, -c / vcl, _ps); _ps->V = 1;
	glusVAdd(_pa, 1, &vb, a / vbl, _pt); _pt->V = 1;
	
	return true;
}


bool
glusCInTri(
_In_	PGlusVector	_pa,
_In_	PGlusVector	_pb,
_In_	PGlusVector	_pc,
_Out_	PGlusCircle _c)
{
	assert(_pa	!=	NULL);
	assert(_pb	!=	NULL);
	assert(_pc	!=	NULL);
	assert(_c	!=	NULL);

	// 
	// first get the tangent points
	// 
	GlusVector		r, s, t;
	if (!glusCGetTangentPoints(_pa, _pb, _pc, &r, &s, &t))
		return false;

	// 
	// then we use the three point to get the circle
	// 
	return	glusCExTri(&r, &s, &t, _c);
}

bool
glusCGetAltitudeFeet(
_In_	PGlusVector	_pa,
_In_	PGlusVector	_pb,
_In_	PGlusVector	_pc,
_Out_	PGlusVector	_pr,
_Out_	PGlusVector	_ps,
_Out_	PGlusVector	_pt)
{
	assert(_pa && _pb&&_pc&&_pr&&_ps&&_pt);

	if (glusPIs3PointOnLine(_pa, _pb, _pc))
		return false;

	//
	// fist 
	// compute the vector and prepentical vector of side 
	// 
	GlusVector		va, vb, vc, van, vbn, vcn;
	glusVFromPoint(_pa, _pb, &vb);
	glusVFromPoint(_pb, _pc, &vc);
	glusVFromPoint(_pc, _pa, &va);
	glusVNormal(&va, &van);
	glusVNormal(&vb, &vbn);
	glusVNormal(&vc, &vcn);

	// 
	// second
	// we get point on each altitude
	// 
	GlusVector		prr, pss, ptt;
	glusVAdd(_pc, 1, &vbn, 1, &prr);
	glusVAdd(_pa, 1, &vcn, 1, &pss);
	glusVAdd(_pb, 1, &van, 1, &ptt);

	// 
	// so we can compute the feet of the three altitudes
	// 
	glusLIntersect(_pa,_pb,_pc,&prr, _pr);
	
	glusLIntersect(_pb,_pc,_pa,&pss, _ps);

	glusLIntersect(_pa,_pc,_pb,&ptt, _pt);

	return true;
}

bool
glusCGetMiddlePoints(
_In_	PGlusVector	_pa,
_In_	PGlusVector	_pb,
_In_	PGlusVector	_pc,
_Out_	PGlusVector	_pr,
_Out_	PGlusVector	_ps,
_Out_	PGlusVector	_pt)
{
	assertp(_pa && _pb&&_pc&&_pr&&_ps&&_pt);

	if (glusPIs3PointOnLine(_pa, _pb, _pc))
		return false;

	glusLMiddle(_pa, _pb, _pr);
	glusLMiddle(_pb, _pc, _ps);
	glusLMiddle(_pc, _pa, _pt);

	return true;
}

bool
glusCNinePoint(
_In_	PGlusVector	_pa,
_In_	PGlusVector	_pb,
_In_	PGlusVector	_pc,
_Out_	PGlusCircle _c)
{
	assertp(_pa && _pb && _pc && _c);

	// 
	// first
	// get the middle point of each side
	// 
	GlusVector	pr, ps, pt;
	if (!glusCGetMiddlePoints(_pa, _pb, _pc,&pr,&ps,&pt))
	//if (!glusCGetAltitudeFeet(_pa, _pb, _pc, &pr, &ps, &pt))
		return false;

	// 
	// then 
	// we get circle from the points
	// 
	return	glusCExTri(&pr, &ps, &pt, _c);
}

void
glusDrawArc(
_In_	PGlusCircle	_c,
_In_	GLdouble	_angle_start,
_In_	GLdouble	_angle_sweep,
_In_	size		_n)
{
	assertp(_c);
	assert(_n > 0);

	GLdouble	ang_step = _angle_sweep / _n;
	
	GLdouble	T = tana(ang_step / 2);	// tan of half angle
	GLdouble	S = 2 * T / (1 + T*T);	// sin of half angle

	GLdouble	x = _c->Radius * sina(_angle_start);
	GLdouble	y = _c->Radius * cosa(_angle_start);

	glBegin(GL_LINE_STRIP);
	{
		for (size i = 0; i <= _n;i++)
		{
			glVertex2d(_c->Center.X + x, _c->Center.Y + y);

			//
			// use shear to respent cos and sin function
			//
			x += T*y;
			y -= S*x;
			x += T*y;
		}
	}
	glEnd();

}