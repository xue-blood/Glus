#include "..\inc\glus.h"

//  [8/11/2016 Tld]
//	add glusScale
//	add glusTranslate
//	add glusRotate



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

// add [12/26/2016 xue]
void
glusTransform(
_In_	PGlusTransform	_trans)
{
	glMatrixMode(GL_MODELVIEW);
	
	glTranslated(_trans->Dx, _trans->Dy, _trans->Dz);
	glScaled(_trans->Sx, _trans->Sy, _trans->Sz);
	glRotated(_trans->Angle,_trans->Ax,_trans->Ay,_trans->Az);

}

/*
 *	transform vertex and not send to pipeline
 */
void
glusTransformVertex(PGlusTransform _trans,PGlusVector _v)
{

	glPushMatrix();
	glLoadIdentity();
	/*
	*	build inverse transform matrix
	*/
	float m[4][4] = { 0 };
	// apply inverse transform
	glTranslated(_trans->Dx, _trans->Dy, _trans->Dz);
	glScaled(_trans->Sx, _trans->Sy, _trans->Sz);
	glRotated(_trans->Angle, _trans->Ax, _trans->Ay, _trans->Az);

	// mul vector
	m[0][0] = _v->X; m[0][1] = _v->Y; m[0][2] = _v->Z;
	glMultMatrixf((float *)m);

	glGetFloatv(GL_MODELVIEW_MATRIX, (float *)m);

	glPopMatrix();

	/*
	*	store the new vector
	*/
	_v->X = m[0][0]; _v->Y = m[0][1]; _v->Z = m[0][2];
	_v->Z = 1;
}

/*
*	inverse transform vertex and not send to pipeline
*/
void
glusTransformInvVertex(PGlusTransform _trans, PGlusVector _v)
{

	glPushMatrix();
	glLoadIdentity();
	/*
	 *	build inverse transform matrix
	 */
	float m[4][4] = { 0 };
	// apply inverse transform
	glTranslated(-_trans->Dx,-_trans->Dy,-_trans->Dz);
	glScaled(1 / _trans->Sx, 1 / _trans->Sy, 1 / _trans->Sz);
	glRotated(-_trans->Angle, _trans->Ax, _trans->Ay, _trans->Az);
	
	// mul vector
	m[0][0] = _v->X; m[0][1] = _v->Y; m[0][2] = _v->Z;
	glMultMatrixf((float *)m);

	glGetFloatv(GL_MODELVIEW_MATRIX, (float *)m);

	glPopMatrix();

	/*
	 *	store the new vector
	 */
	_v->X = m[0][0]; _v->Y = m[0][1]; _v->Z = m[0][2];
	_v->Z = 1;
}
