#include "../inc/glus.h"

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
	glPushMatrix();


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


	for (Glussize z = 0; z < _z; z++)
	{
		glPushMatrix();

		for (Glussize y = 0; y < _y; y++)
		{
			glPushMatrix();

			for (Glussize x = 0; x < _x; x++)	// draw at x axis
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
			glPopMatrix();

			glusTranslate(0, _eachTrans->Dy, 0);
		}
		glPopMatrix();

		glusTranslate(0, 0, _eachTrans->Dz);
	}

	glPopMatrix();
}


void glusArrayRect(
_In_	PArray	_p)
{
	assert(_p);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	for (int x = 0; x < _p->Data.Rect.n_X; x++)
	{
		glPushMatrix();

		for (int y = 0; y < _p->Data.Rect.n_Y; y++)
		{
			glPushMatrix();
			for (int z = 0; z < _p->Data.Rect.n_Z; z++)
			{
				glusShapeDraw(_p->Shape);

				glusTranslate(0,0,_p->Data.Rect.d_Z);
			}
			glPopMatrix();
			glusTranslate(0, _p->Data.Rect.d_Y, 0);
		}

		glPopMatrix();
		glusTranslate(_p->Data.Rect.d_X, 0,0);
	}
	glPopMatrix();
}

void 
glusArrayDraw(
_In_	PArray	_p)
{
	assert(_p);

	switch (_p->Type)
	{
	case Array_Rect:	glusArrayRect(_p); break;
	}
}

void
glusArrayLoad(
_In_	FILE *		_file,
_Out_	PArray		_array,
_In_	PGlusShape	_target)
{
	assert(_file && _array);

	/*
	 *	get the type
	 */
	char	name[10];
	glusScanf(_file, "%s", name, _countof(name));
	if (strequ(name, "rect", _countof("rect"))) { _array->Type = Array_Rect; goto _array_type_ok_; };	// rect
	if (strequ(name, "circle", _countof("circle"))) { _array->Type = Array_Rect; goto _array_type_ok_; };	// circle
	if (strequ(name, "curve", _countof("curve"))) { _array->Type = Array_Rect; goto _array_type_ok_; };	// curve
	return;	// Unknown type

_array_type_ok_:
	/*
	 *	get data for each type
	 */
	switch (_array->Type)
	{
	case Array_Rect:
		// get count of array
		glusSkipSpace(_file);
		glusScanf(_file, "(%d,%d,%d)", &_array->Data.Rect.n_X, 
			&_array->Data.Rect.n_Y, 
			&_array->Data.Rect.n_Z);

		// get the distance of array
		glusSkipSpace(_file);
		glusScanf(_file, "<%f,%f,%f>", &_array->Data.Rect.d_X,
			&_array->Data.Rect.d_Y,
			&_array->Data.Rect.d_Z);
		break;
	default:
		break;
	}
	
	_array->Shape = _target;
}