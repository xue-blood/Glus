#include "..\inc\glus.h"

GLdouble
glusPDistance(
_In_	PGlusVector _pa,
_In_	PGlusVector _pb)
{
	assertp(_pa);
	assertp(_pb);

	return	sqrt((_pb->X - _pa->X)*(_pb->X - _pa->X)	+
				(_pb->Y - _pa->Y)*(_pb->Y - _pa->Y)		+
				(_pb->Z - _pa->Z)*(_pb->Z - _pa->Z));
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