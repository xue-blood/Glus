#include "..\inc\glus.h"

/*
//  [8/19/2016 xcv]
	add glusTiling
	add glusAxis
	add glusAxis3D
*/


void
glusTiling(
_In_	PGlusTransform	_orgTrans,
_In_	PGlusTransform	_eachTrans,
_In_	GLdouble		_x,
_In_	GLdouble		_y,
_In_	GLdouble		_z,
_In_	void(*callback)(void))
{
	assertp(_orgTrans && _eachTrans && callback);

	// store the current CT
	glusPushCT();


	// handle some unbelieve data
	if (_z == 0)	_z = 1;
	if (_y == 0)	_y = 1;
	if (_orgTrans->Sx == 0)	_orgTrans->Sx = 1;
	if (_orgTrans->Sy == 0)	_orgTrans->Sy = 1;
	if (_orgTrans->Sz == 0)	_orgTrans->Sz = 1;
	if (_eachTrans->Sx == 0)	_eachTrans->Sx = 1;
	if (_eachTrans->Sy == 0)	_eachTrans->Sy = 1;
	if (_eachTrans->Sz == 0)	_eachTrans->Sz = 1;

	//
	// go to the start
	//
	glusTranslate(_orgTrans->Dx, _orgTrans->Dy, _orgTrans->Dz);
	glusScale(_orgTrans->Sx, _orgTrans->Sy, _orgTrans->Sz);
	glusRotate(_orgTrans->Angle, _orgTrans->Ax, _orgTrans->Ay, _orgTrans->Az);


	for (size z = 0; z < _z; z++)
	{
		glusPushCT();

		for (size y = 0; y < _y; y++)
		{
			glusPushCT();

			for (size x = 0; x < _x; x++)	// draw at x axis
			{
				// draw the motif
				callback();

				//
				// go to the next 
				//
				glusTranslate(_eachTrans->Dx, 0, 0);
				glusScale(_eachTrans->Sx, _eachTrans->Sy, _eachTrans->Sz);
				glusRotate(_eachTrans->Angle, _eachTrans->Ax, _eachTrans->Ay, _eachTrans->Az);

			}
			glusPopCT();

			glusTranslate(0, _eachTrans->Dy, 0);
		}
		glusPopCT();

		glusTranslate(0, 0, _eachTrans->Dz);
	}

	glusPopCT();
}


//
// draw a single z-axis and a cone at end with the specify length
//
void 
glusAxis(
_In_	GLdouble	_length)
{
	glusPushCT();

	//
	// draw z-axis
	//
	glBegin(GL_LINES); 
	{
		glVertex3d(0.0, 0.0, 0.0);	glVertex3d(0.0, 0.0, _length);
	}
	glEnd();

	//
	// draw cone
	//
	glusTranslate(0, 0, _length - 0.2);
	glutWireCone(0.04, 0.2, 12, 9);

	glusPopCT();
}

//
// draw three axis with color:red, green, blue
//
void
glusAxis3D(
_In_	GLdouble	_length)
{

	//
	// z-axis
	//
	glColor3d(0, 0, 1);
	glusAxis(_length);

	glusPushCT();
	
	//
	// y-axis
	//
	glusRotateX(-90);
	glColor3d(0, 1, 0);
	glusAxis(_length);

	
	//
	// x-axis
	//
	glusRotateY(90);
	glColor3d(1, 0, 0);
	glusAxis(_length);


	glusPopCT();
}

void 
glusSphere(pvoid _pointer)
{
	glusPushCT();

	glutWireSphere(0.5, 12, 8);

	glusPopCT();
}

void
glusCube(pvoid _pointer)
{
	glusPushCT();

	glutWireCube(1);

	glusPopCT();
}

void
glusShapeDefault(PGlusShape _shape)
{
	assertp(_shape);

	ZeroMemory(_shape, sizeof(GlusShape));

	// diffuse : white
	glusColor(&_shape->Diffuse, 1, 1, 1, 1);

	// transform : default
	glusTransformDefault(&_shape->Transform);
}