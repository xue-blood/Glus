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


	for (Glussize z = 0; z < _z; z++)
	{
		glusPushCT();

		for (Glussize y = 0; y < _y; y++)
		{
			glusPushCT();

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

	glLineWidth(2);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	//
	// draw z-axis
	//
	glBegin(GL_LINES); 
	{
		glVertex3d(0.0, 0.0, 0.0);	glVertex3d(0.0, 0.0, _length);
	}
	glEnd();
	glLineWidth(1);

	//
	// draw cone
	//
	glusTranslate(0, 0, _length - 0.2);
	if (glusGetShadeLevel()==Glus_Shade_Wire)	glutWireCone(0.04, 0.2, 12, 9);
	else										glutSolidCone(0.04, 0.2, 12, 9);

	glusPopCT();
}

//
// draw three axis with color:red, green, blue
//
void
glusAxis3D(
_In_	pvoid		_ptr)
{
	int _length = (int)_ptr;

	if (_length == 0) _length = 1;

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

	if (glusGetShadeLevel() == Glus_Shade_Wire)		glutWireSphere(0.5, 24, 16);
	else											glutSolidSphere(0.5, 24, 16);

	glusPopCT();
}

void
glusTeapot(pvoid _pointer)
{
	glusPushCT();

	if (glusGetShadeLevel() == Glus_Shade_Wire)		glutWireTeapot(0.5);
	else											glutSolidTeapot(0.5);

	glusPopCT();
}

void
glusCube(pvoid _pointer)
{
	glusPushCT();

	if (glusGetShadeLevel() == Glus_Shade_Wire)		glutWireCube(1);
	else											glutSolidCube(1);
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

/*
 *	draw grid
 */
// create [9/27/2016 blue]
void 
glusGrid(pvoid _ptr)
{
	glMatrixMode(GL_MODELVIEW);
	
	glPushMatrix();

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	GLint max_x = 15;
	GLint max_z = 15;

	
	/*
	 *	draw align x-axis
	 */
	for (GLint z = -max_z; z <= max_z; z++)
	{
		glLineWidth((!z) * 11 + 1);
		glBegin(GL_LINES);
			glVertex3i(-max_x, 0, z);
			glVertex3i(max_x, 0, z);
		glEnd();
	}

	/*
	 *	draw align z-axis
	 */
	for (GLint x = -max_x; x <= max_x; x += 1)
	{
		glLineWidth((!x) * 11 + 1);
		glBegin(GL_LINES);
			glVertex3i(x, 0, -max_z);
			glVertex3i(x, 0, max_z);
		glEnd();
	}

	

	glPopMatrix();
}

/*
 *	draw "Koch" curve
 */
// create [10/24/2016 blue]
void
glusKoch(double dir,double len,int n)
{
	double  rad_dir = Glus_Rad *dir;

	if (n <= 0)	glusLineRel(len*cos(rad_dir), len*sin(rad_dir),0);
	else
	{
		n--;
		len /= 3;

		glusKoch(dir, len, n);	dir += 60;
		glusKoch(dir, len, n);	dir -= 120;
		glusKoch(dir, len, n);	dir += 60;
		glusKoch(dir, len, n);
	}

}

/*
 *	draw "Koch" snow
 */
// create [10/25/2016 blue]
void
glusKochSnow(pvoid ptr)
{
	int n = (int)ptr;
	if (n > 10) glusLog("level is too high.\n");
	
	glDisable(GL_LIGHTING);

	glusMoveTo(0, 0, 0);				glusKoch(60,  9, n);
	glusMoveTo(4.5, 9*cosa(30), 0);		glusKoch(-60, 9, n);
	glusMoveTo(9, 0, 0);				glusKoch(180, 9, n);
}