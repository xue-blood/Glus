#include "..\inc\glus.h"

void 
glusLinePToV2f(
_In_	GlusLine2f	*_pl,
_Out_	GlusLine2f	*_vl)
{

}
GLenum
glusIsLineIntersect2f(
_In_ GlusVertex2f *_a,
_In_ GlusVertex2f *_b,
_In_ GlusVertex2f *_c,
_In_ GlusVertex2f *_d,
_Out_ GlusVertex2f *_v)
{
	assert(_a != NULL);
	assert(_b != NULL);
	assert(_c != NULL);
	assert(_d != NULL);
	assert(_v != NULL);


	GlusVector2f	vb, vd, vc;
	glusInitVector2f(_a, _b, &vb);
	glusInitVector2f(_c, _d, &vd);
	glusInitVector2f(_a, _c, &vc);

	//
	// test is two line parallel
	//

	// get the normal vector for vector
	GlusVector2f	vdn,vbn;
	glusVectorNormal2f(&vd, &vdn);
	glusVectorNormal2f(&vb, &vbn);

	if (glusDotProduct(&vdn, &vb) == 0)
	{
		// line is parallel 
		// then test is overlap
		if (vb.X*vc.Y- vb.Y*vc.X == 0 )
			return Glus_Line_Parallel_Overlap;

		// there is no intersection between them
		return Glus_Line_Parallel;
	}
	else
	{
		//
		// compute the paramters 
		//
		GLfloat t = glusDotProduct(&vdn, &vc) / 
					glusDotProduct(&vdn, &vb);
		GLfloat u = glusDotProduct(&vbn, &vc) /
					glusDotProduct(&vdn, &vb);

		//
		// compute the point 
		// use t or u
		//
		_v->X = _a->X + vb.X * t;
		_v->Y = _a->Y + vb.Y * t;

		// is the intersection exist
		if (t > 1 || t<0 || u >1 || u < 0)
			return Glus_Line_Intersect_NotExsit;
		return Glus_Line_Intersect;
	}
}

GLfloat
glusLineDistance2f(
_In_	GlusVertex2f	*_pa,
_In_	GlusVertex2f	*_pb)
{
	assert(_pa != NULL);
	assert(_pb != NULL);
	
	GLfloat	dis =	sqrtf((_pa->X - _pb->X)*(_pa->X - _pb->X) +
						(_pa->Y - _pa->Y)*(_pa->Y - _pa->Y));

	return dis;
}

bool
glusLineHit2f(
_In_	GlusLine2f		*_l,
_In_	GlusVertex2f	*_p,
_In_	GlusVector2f	*_n,
_Out_	GlusVertex2f	*_phit)
{
	assert(_l != NULL);
	assert(_p != NULL);
	assert(_n != NULL);
	assert(_phit != NULL);

	GlusVector2f	vl,vb;
	glusInitVector2f(&_l->A, &_l->B,&vl);
	glusInitVector2f(&_l->A, _p,&vb);

	GLfloat temp, t;
	temp = glusDotProduct(_n, &vl);
	if (temp == 0)
		return	false;

	t = glusDotProduct(_n, &vb) / temp;
	
	_phit->X = _l->A.X + vl.X *t;
	_phit->Y = _l->A.Y + vl.Y *t;
	return true;
}