#include "..\inc\glus.h"

void
glusP(
_In_	GLdouble	_x,
_In_	GLdouble	_y,
_In_	GLdouble	_z,
_Out_	PGlusVector _o)
{
	assertp(_o);

	_o->X = _x;
	_o->Y = _y;
	_o->Z = _z;
	_o->V = 1;
}

void
glusV(
_In_	GLdouble	_x,
_In_	GLdouble	_y,
_In_	GLdouble	_z,
_Out_	PGlusVector _o)
{
	assertp(_o);

	_o->X = _x;
	_o->Y = _y;
	_o->Z = _z;
	_o->V = 0;
}


void
glusL(
_In_	GLdouble	_ax,
_In_	GLdouble	_ay,
_In_	GLdouble	_az,
_In_	GLdouble	_bx,
_In_	GLdouble	_by,
_In_	GLdouble	_bz,
_Out_	PGlusLine	_o)
{
	assertp(_o);

	_o->A.X = _ax;
	_o->A.Y = _ay;
	_o->A.Z = _az;
	_o->A.V = 1;

	_o->B.X = _bx;
	_o->B.Y = _by;
	_o->B.Z = _bz;
	_o->B.V = 1;
}

void
glusPlane(
_In_	GLdouble	_ax,
_In_	GLdouble	_ay,
_In_	GLdouble	_az,
_In_	GLdouble	_bx,
_In_	GLdouble	_by,
_In_	GLdouble	_bz,
_In_	GLdouble	_cx,
_In_	GLdouble	_cy,
_In_	GLdouble	_cz,
_Out_	PGlusPlane	_o)
{
	assertp(_o);

	_o->A.X = _ax;
	_o->A.Y = _ay;
	_o->A.Z = _az;
	_o->A.V = 1;

	_o->B.X = _bx;
	_o->B.Y = _by;
	_o->B.Z = _bz;
	_o->B.V = 1;

	_o->C.X = _cx;
	_o->C.Y = _cy;
	_o->C.Z = _cz;
	_o->C.V = 1;
}

void
normalize(
pGLdouble	data)
{
	GLdouble temp;
	if (*data > 0)
	{
		temp = *data + Glus_Zero;
		if ((temp - (GLint)temp) <= Glus_Zero + Glus_Zero)
			*data = (GLint)temp;
	}
	else
	{
		temp = *data - Glus_Zero;
		if (((GLint)temp - temp) <= Glus_Zero + Glus_Zero)
			*data = (GLint)temp;
	}
}

bool	
glusPIs3PointOnLine(
_In_ PGlusVector _pa,
_In_ PGlusVector _pb,
_In_ PGlusVector _pc)
{
	assertp(_pa && _pb && _pc);

	GlusVector	va, vb, van;
	glusVFromPoint(_pa, _pb, &vb);
	glusVFromPoint(_pc, _pa, &va);
	glusVNormal(&va, &van);

	if (glusVDotPro(&vb, &van) == 0)
		return true;
	return false;
}

bool
glusPIsInPolygon(
_In_	PGlusVector		_point,
_In_	PGlusLink		_polygon)
{
	assertp(_point && _polygon);

	// is it a valid polygon
	if (glusLinkLength(_polygon) < 3)
		return false;

	// 
	// if( Q -P) * n <0,then Q lies in P
	// 
	GlusVector		vn, vt, vp;
	PGlusPoints	pa = (PGlusPoints)_polygon->BLink, pb;

	while (!glusLinkIsHead(pa,_polygon))
	{
		pb = (PGlusPoints)pa->Link.BLink;

		// line Pn -- P0
		if (pb == (PGlusPoints)_polygon)
			pb = (PGlusPoints)_polygon->BLink;


		// compute the normal vector
		glusVFromPoint(&pa->Point, &pb->Point, &vt);
		glusVNormal(&vt, &vn);

		// compute the vector for point
		glusVFromPoint(&pb->Point, _point, &vp);

		if (glusVDotPro(&vn, &vp) > 0)
			return false;

		pa = (PGlusPoints)pa->Link.BLink;
	}
	return true;
}

bool
glusPIsInPolygonS(
_In_	PGlusVector		_point,
_In_	PGlusSink		_polygon)
{
	assertp(_point && _polygon);
	
	// is it a valid polygon
	if (glusSinkLength(_polygon) < 3)
		return false;

	// 
	// if( Q -P) * n <0,then Q lies in P
	// 
	GlusVector		vn,vt,vp;
	PGlusPointsS	pa = (PGlusPointsS)_polygon->Next, pb;

	while (pa)
	{
		pb = (PGlusPointsS)pa->Sink.Next;

		// line Pn -- P0
		if (!pb)
			pb = (PGlusPointsS)_polygon->Next;


		// compute the normal vector
		glusVFromPoint(&pa->Point, &pb->Point, &vt);
		glusVNormal(&vt, &vn);

		// compute the vector for point
		glusVFromPoint(&pb->Point, _point, &vp);

		if (glusVDotPro(&vn, &vp) > 0)
			return false;

		pa = (PGlusPointsS)pa->Sink.Next;
	}
	return true;
}