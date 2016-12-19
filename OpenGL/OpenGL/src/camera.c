#include"../inc/glus.h"

GlusVector	Eye;



/*
 *	set camera data to model-view matrix
 */
// create [10/12/2016 blue]
void 
glusCamera(
_In_	PGlusCamera		_camera)
{
	assert(_camera);

	GLdouble	m[16];

#undef M
#define M(i,field) m[i] = _camera->field;
	/*
	 *	prepare matrix data
	 */
	M(0, U.X);	M(4, U.Y);	M(8, U.Z);	m[12] = - glusVDotPro(&_camera->Eye, &_camera->U);
	M(1, V.X);	M(5, V.Y);	M(9, V.Z);	m[13] = - glusVDotPro(&_camera->Eye, &_camera->V);
	M(2, N.X);	M(6, N.Y);	M(10, N.Z);	m[14] = - glusVDotPro(&_camera->Eye, &_camera->N);
	
	m[3] = 0;	m[7] = 0;	m[11] = 0;	m[15] = 1;
#undef M

	/*
	 *	load opengl matrix
	 */
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(m);


	glusLog("eye:%2.2f %2.2f %2.2f ;n:%2.2f %2.2f %2.2f;u:%2.2f %2.2f %2.2f;v:%2.2f %2.2f %2.2f\n",
		_camera->Eye.X, _camera->Eye.Y, _camera->Eye.Z,
		_camera->N.X, _camera->N.Y, _camera->N.Z,
		_camera->U.X, _camera->U.Y, _camera->U.Z,
		_camera->V.X, _camera->V.Y, _camera->V.Z);

	Eye = _camera->Eye;
}
/*
 *	set camera
 */
// create [10/12/2016 blue]
void
glusCameraSet(
_In_	GLdouble	_eyex,	GLdouble	_eyey,	GLdouble		_eyez,
_In_	GLdouble	_lookx, GLdouble	_looky, GLdouble		_lookz,
_In_	GLdouble	_upx,	GLdouble	_upy,	GLdouble		_upz,
_Inout_	PGlusCamera	_camera)
{
	assert(_camera);


	// store eye 
	glusP3d(_eyex, _eyey, _eyez, &_camera->Eye); 
	// make n
	glusV3d(_eyex - _lookx, _eyey - _looky, _eyez - _lookz, &_camera->N);	
	glusVUnit(&_camera->N);

	// make u
	GlusVector up;	glusV3d(_upx, _upy, _upz, &up); glusVUnit(&up);
	glusVCroPro(&up, &_camera->N,&_camera->U);		glusVUnit(&_camera->U);

	// make v
	glusVCroPro(&_camera->N, &_camera->U, &_camera->V);
	glusVUnit(&_camera->V);

	// set camera
	glusCamera(_camera);
}

/*
 *	sliding camera at it's own axes
 */
// create [10/12/2016 blue]
void 
glusCameraSlide(
_In_	GLdouble	_u_del,
_In_	GLdouble	_v_del,
_In_	GLdouble	_n_del,
_Inout_	PGlusCamera	_camera)
{
	assert(_camera);

#undef S
#define S(field) _camera->Eye.field += _u_del * _camera->U.field + _v_del * _camera->V.field + _n_del *_camera->N.field

	S(X); S(Y); S(Z);

#undef S

	glusCamera(_camera);
}

/*
 *	roll the camera
 */
// create [10/12/2016 blue]
void 
glusCameraRoll(
_In_	GLdouble	_ang,
_Inout_	PGlusCamera	_camera)
{
	assert(_camera);

	GLdouble	s = sina(_ang), c = cosa(_ang);

	GlusVector  t = _camera->U;
#undef v
#define v _camera->V
	glusVAdd(&t, c,		&_camera->V, s,		&_camera->U); glusVUnit(&_camera->U);
	glusVAdd(&t, -s,	&_camera->V, c,		&_camera->V); glusVUnit(&_camera->V);

#undef v
	glusCamera(_camera);
}

/*
 *	yaw the camera
 */
// create [10/17/2016 blue]
void
glusCameraYaw(
_In_	GLdouble	_ang,
_Inout_	PGlusCamera	_camera)
{
	assert(_camera);

	GLdouble	s = sina(_ang), c = cosa(_ang);

	/*
	 *	compute the new vector
	 */
	GlusVector n = _camera->N, u = _camera->U;

	glusVAdd(&n, c, &u, -s, &_camera->N);	glusVUnit(&_camera->N);
	glusVAdd(&n, s, &u, c,  &_camera->U);	glusVUnit(&_camera->U);

	glusCamera(_camera);
}

/*
*	pitch the camera
*/
// create [10/17/2016 blue]
void
glusCameraPitch(
_In_	GLdouble	_ang,
_Inout_	PGlusCamera	_camera)
{
	assert(_camera);

	GLdouble	s = sina(_ang), c = cosa(_ang);

	/*
	*	compute the new vector
	*/
	GlusVector n = _camera->N, v = _camera->V;
	glusVAdd(&n, c,		&v, s, &_camera->N);	glusVUnit(&_camera->N);
	glusVAdd(&n, -s,	&v, c, &_camera->V);	glusVUnit(&_camera->V);

	glusCamera(_camera);
}

static GlusCamera	CameraOld;

/*
 *	push current camera
 */
// create [10/18/2016 blue]
void 
glusCameraPush(
_In_	PGlusCamera	_camera)
{
	assert(_camera);
	CameraOld = *_camera;
}

/*
*	pop current camera
*/
// create [10/18/2016 blue]
void
glusCameraPop(
_In_	PGlusCamera	_camera)
{
	assert(_camera);
	*_camera = CameraOld;
}

/*
 *	set stereo view
 */
// create [10/18/2016 blue]
void
glusCameraStereo(
_In_	GLdouble	_d,
_Inout_	PGlusCamera	_camera)
{
	assert(_camera);

	if (_d == 0)		return;

	/*
	 *	compute the new camera
	 */
	glusVAdd(&_camera->Eye, 1, &_camera->U, _d, &_camera->Eye);
	glusCamera(_camera);
}
/*
 *	set projection matrix
 */
// create [9/25/2016 blue]
void
glusProjection(
_In_	PGlusProjection		_proj)
{
	assert(_proj);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	switch (_proj->Type)
	{
	case ProjectPers:
		gluPerspective(_proj->Persp.AngleView, _proj->Persp.AspectRation, _proj->Near, _proj->Far);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		break;

	case ProjectOrtho:
		glOrtho(_proj->Ortho.Left, _proj->Ortho.Right,
			_proj->Ortho.Bottom, _proj->Ortho.Top,
			_proj->Near, _proj->Far);
		break;
	case ProjectOblique:
		glOrtho(_proj->Ortho.Left, _proj->Ortho.Right,
			_proj->Ortho.Bottom, _proj->Ortho.Top,
			_proj->Near, _proj->Far);
		glusObli(_proj->Ortho.Dx,_proj->Ortho.Dy,_proj->Ortho.Dz);
		break;
	default:
		break;
	}
}

/*
 *	set perspective projection
 */
// create [10/17/2016 blue]
void 
glusPerspective(
_In_	GLdouble	_angle,
_In_	GLdouble	_ration,
_In_	GLdouble	_near,
_In_	GLdouble	_far,
_Inout_	PGlusProjection	_projection)
{
	assert(_projection);

	_projection->Type = ProjectPers;	// set type

	_projection->Near = _near;
	_projection->Far = _far;
	_projection->Persp.AngleView = _angle;
	_projection->Persp.AspectRation = _ration;
}

/*
 *	set orthographic projection
 */
// create [10/17/2016 blue]
void 
glusOrtho(
_In_	GLdouble	_left,
_In_	GLdouble	_right,
_In_	GLdouble	_bottom,
_In_	GLdouble	_top,
_In_	GLdouble	_near,
_In_	GLdouble	_far,
_Inout_	PGlusProjection	_projection)
{

	assert(_projection);

	_projection->Type = ProjectOrtho;

	_projection->Near = _near;
	_projection->Far = _far;
	_projection->Ortho.Left = _left;
	_projection->Ortho.Right = _right;
	_projection->Ortho.Top = _top;
	_projection->Ortho.Bottom = _bottom;

}

/*
 *	create oblique projection on parallel projection
 */
// create [10/18/2016 blue]
void 
glusOblique(
_In_	GLdouble	_dx,
_In_	GLdouble	_dy,
_In_	GLdouble	_dz,
_Inout_	PGlusProjection	_projection)
{
	assert(_projection);

	_projection->Type = ProjectOblique;

	_projection->Ortho.Dx = _dx;
	_projection->Ortho.Dy = _dy;
	_projection->Ortho.Dz = _dz;
}

/*
 *	set oblique projection
 */
// create [10/18/2016 blue]
void
glusObli(
_In_	GLdouble	_dx,
_In_	GLdouble	_dy,
_In_	GLdouble	_dz)
{
	if (_dz == 0)
		return;

	glMatrixMode(GL_PROJECTION);

	float  m[16] = { 0 };

	m[0] = m[5] = m[10] = m[15] = 1;
	m[8] = (float)(-_dx/_dz);
	m[9] = (float)(-_dy / _dz);

	glMultMatrixf(m);
}

/*
 *	get eye position
 */
// create [10/18/2016 blue]
PGlusVector
glusGetEye()
{
	return &Eye;
}