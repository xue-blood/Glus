#include "..\inc\glus.h"

int		_Window_Height, _Window_Width;

void
glusInitWin(
_In_	GLint	_left,
_In_	GLint	_top, 
_In_	GLint	_width, 
_In_	GLint	_height, 
_In_	str	_name, 
_In_	GLenum	_mode)
{
	assertp(_name);

	glutInitDisplayMode(_mode);
	glutInitWindowPosition(_left, _top);
	glutInitWindowSize(_width, _height);
	glutCreateWindow(_name);

	_Window_Height = _height;
	_Window_Width = _width;
}

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
		if ((temp - (GLint)temp) <= Glus_Zero+Glus_Zero)
			*data = (GLint)temp;
	}
	else
	{
		temp = *data - Glus_Zero;
		if (((GLint)temp - temp) <= Glus_Zero + Glus_Zero)
			*data = (GLint)temp;
	}
}
