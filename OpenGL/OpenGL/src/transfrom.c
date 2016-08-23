#include "..\inc\glus.h"

//  [8/11/2016 Tld]
//	add glusInitCT
//	add glusScale
//	add glusTranslate
//	add glusRotate

//  [8/13/2016 Tld]
//	add glusPushCT
//	add glusPopCT

void	
glusInitCT()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void
glusPushCT()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
}

void
glusPopCT()
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void	
glusScale(
_In_	GLdouble	_sx,
_In_	GLdouble	_sy,
_In_	GLdouble	_sz)
{
	glMatrixMode(GL_MODELVIEW);
	glScaled(_sx, _sy, _sz);
}

void	
glusTranslate(
_In_	GLdouble	_dx,
_In_	GLdouble	_dy,
_In_	GLdouble	_dz)
{
	glMatrixMode(GL_MODELVIEW);
	glTranslated(_dx, _dy, _dz);
}

void	
glusRotate(
_In_	GLdouble	_angle,
_In_	GLdouble	_x,
_In_	GLdouble	_y,
_In_	GLdouble	_z)
{
	glMatrixMode(GL_MODELVIEW);
	glRotated(_angle, _x, _y, _z);
}

// add [7/10/2016 tld]
//
// set the default transform data
//
void
glusTransformDefault(
_In_	PGlusTransform	_trans)
{
	assertp(_trans);

	ZeroMemory(_trans, sizeof(GlusTransform));

	_trans->Sx = _trans->Sy = _trans->Sz = 1;
}