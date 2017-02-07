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
/*
 *	use the order T * R * S
	refer:http://gamedev.stackexchange.com/questions/16719/what-is-the-correct-order-to-multiply-scale-rotation-and-translation-matrices-f
 */
void
glusTransform(
_In_	PGlusTransform	_trans)
{
	glMatrixMode(GL_MODELVIEW);
	glTranslated(_trans->Dx, _trans->Dy, _trans->Dz);
	glRotated(_trans->Angle, _trans->Ax, _trans->Ay, _trans->Az);

	glScaled(_trans->Sx, _trans->Sy, _trans->Sz);

}

/*
 *	transform vertex and not send to pipeline
 */
void
glusTransformVector(PGlusTransform _trans, PGlusVector _v)
{



	glPushMatrix();
	glLoadIdentity();
	/*
	*	build inverse transform matrix
	*/
	float t[4][4] = { 0 };
	// apply  transform
	glTranslated(_trans->Dx, _trans->Dy, _trans->Dz);
	glRotated(_trans->Angle, _trans->Ax, _trans->Ay, _trans->Az);
	glScaled(_trans->Sx, _trans->Sy, _trans->Sz);
	glGetFloatv(GL_MODELVIEW_MATRIX, (float *)t);


	// mul vector
	float m[4][4] = { 0 };
	glusMatrixIdentity(m);
	m[0][0] = _v->X; m[0][1] = _v->Y; m[0][2] = _v->Z; m[0][3] = _v->V;
	//glLoadMatrixf(m);
	glMultMatrixf((float *)m);

	glGetFloatv(GL_MODELVIEW_MATRIX, (float *)m);

	glPopMatrix();

	/*
	*	store the new vector
	*/
	_v->X = m[0][0]; _v->Y = m[0][1]; _v->Z = m[0][2];
	glusValid(_v);
}

/*
*	inverse transform vertex and not send to pipeline
*/
void
glusTransformInvVector(PGlusTransform _trans, PGlusVector _v)
{

	glPushMatrix();
	glLoadIdentity();
	/*
	 *	build inverse transform matrix
	 */
	
	float t[4][4] = { 0 };
	// apply inverse transform
	// ! for inverse transform ,we need inverse call order
	glScaled(1 / _trans->Sx, 1 / _trans->Sy, 1 / _trans->Sz);
	glRotated(-_trans->Angle, _trans->Ax, _trans->Ay, _trans->Az);
	glTranslated(-_trans->Dx, -_trans->Dy, -_trans->Dz);

	glGetFloatv(GL_MODELVIEW_MATRIX, (float *)t);


	// mul vector
	float m[4][4] = { 0 };
	glusMatrixIdentity(m);
	m[0][0] = _v->X; m[0][1] = _v->Y; m[0][2] = _v->Z; m[0][3] = _v->V;
	//glLoadMatrixf(m);
	glMultMatrixf((float *)m);

	glGetFloatv(GL_MODELVIEW_MATRIX, (float *)m);
	
	glPopMatrix();

	/*
	 *	store the new vector
	 */
	_v->X = m[0][0]; _v->Y = m[0][1]; _v->Z = m[0][2];
	glusValid(_v);

}
