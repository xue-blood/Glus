#include "..\inc\glus.h"

/************************************************************************/
/* this model provide the function for vector							*/
/************************************************************************/

/*																		*/
/*																		*/
/*	[2016.8.4]
		add glusVNormalize
*/
/************************************************************************/

//
// normalize the vector 
// for some float isn't correct sometimes
// 
void
glusNormalize(
_Inout_ PGlusVector _v)
{
	normalize(&_v->X);
	normalize(&_v->Y);
	normalize(&_v->Z);
	_v->V = (_v->V)?1:0;
}

//
// get the perpentical vector
// 
void	
glusNormal(
_In_	GlusVector *_in,
_Out_	GlusVector *_out)
{
	assert(_in != NULL && _out != NULL);

	GLdouble temp = _in->X;

	_out->X = _in->Y;
	_out->Y = -temp;
	_out->Z = _in->Z;
	_out->V = 0;

	glusNormalize(_out);
}

//
// convert vector to unit and normalize
// 
void
glusUnit(
_Inout_ PGlusVector _v)
{
	assertp(_v);

	GLdouble	vl = glusLength(_v);

	_v->X /= vl;
	_v->Y /= vl;
	_v->Z /= vl;

	glusNormalize(_v);
}


void
glusAdd(
_In_	GlusVector *_va,
_In_	GLdouble	_pa,
_In_	GlusVector *_vb,
_In_	GLdouble	_pb,
_Out_	GlusVector *_vo)
{
	assert(_va != NULL && _vb != NULL && _vo != NULL);

	_vo->X = _pa * _va->X + _pb * _vb->X;
	_vo->Y = _pa * _va->Y + _pb * _vb->Y;
	_vo->Z = _pa * _va->Z + _pb * _vb->Z;
	_vo->V = _pa * _va->V + _pb * _vb->V;

	glusNormalize(_vo);
}


GLdouble
glusLength(
_In_	GlusVector *_v)
{

	assertp(_v);

	return sqrt(_v->X*_v->X +
		_v->Y*_v->Y +
		_v->Z*_v->Z);
}

double 
glusDistanceSq(
_In_	PGlusVector	_a,
_In_	PGlusVector	_b)
{
	double x = _a->X - _b->X;
	double y = _a->Y - _b->Y;
	double z = _a->Z - _b->Z;

	return (x*x + y*y + z*z);
}
GLdouble 
glusDotPro(
_In_	GlusVector *_va,
_In_	GlusVector *_vb)
{
	assertp(_va);
	assertp(_vb);

	return	_va->X * _vb->X +
			_va->Y * _vb->Y +
			_va->Z * _vb->Z;
}

void
glusCroPro(
_In_	PGlusVector _va,
_In_	PGlusVector _vb,
_Out_	PGlusVector _vo)
{
	assertp(_va && _vb && _vo);

	_vo->X = _va->Y * _vb->Z - _va->Z * _vb->Y;
	_vo->Y = _va->Z * _vb->X - _va->X * _vb->Z;
	_vo->Z = _va->X * _vb->Y - _va->Y * _vb->X;
	_vb->V = 0;

	glusNormalize(_vo);
}



