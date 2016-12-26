#include "../inc/glus.h"


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
	if (strequ(name, "rect")) { _array->Type = Array_Rect; goto _array_type_ok_; };	// rect
	if (strequ(name, "circle")) { _array->Type = Array_Rect; goto _array_type_ok_; };	// circle
	if (strequ(name, "curve")) { _array->Type = Array_Rect; goto _array_type_ok_; };	// curve
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