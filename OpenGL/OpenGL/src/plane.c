#include "..\inc\glus.h"

Glus_Intersect
glusPlaneIntersect(
_In_	PGlusPlane	_pa,
_In_	PGlusPlane	_pb,
_Out_	PGlusLine	_l)
{
	assertp(_pa && _pb && _l);

	// compute the normal for the first plane
	GlusVector	vaa = glusVector3fv((&_pa->A), (&_pa->B)),
				vab = glusVector3fv((&_pa->A), (&_pa->C));
	GlusVector	vn;
	glusCroPro(&vaa, &vab, &vn);

	// change point form to parameter form for the second plane
	GlusVector	vba = glusVector3fv((&_pb->A), (&_pb->B)),
				vbb = glusVector3fv((&_pb->A), (&_pb->C));

	// are the two plane parallel
	GLdouble	r = glusVDotPro(&vn, &vba);
	if (r < Zero)
		return Intersect_Parallel;
	else
	{
		GlusVector	v = glusVector3fv((&_pa->A), (&_pb->A));
		GLdouble	e = glusVDotPro(&vn, &v) / r,
					t = glusVDotPro(&vn, &vbb) / r;

		// so we get a point on line
		glusVAdd(&_pb->A, 1, &vba, e, &_l->A); 

		// and get the vector for line
		glusVAdd(&vbb, 1, &vba, -t, &_l->B);

		// so convert the form to point
		glusVAdd(&_l->A, 1, &_l->B, 1, &_l->B);

		if (e > 1 || e<0 || t>1 || t < 0)
			return Intersect_Not_Exsit;
	}

	return Intersect_Exsit;
}