#include "..\inc\glus.h"

/*
//  [8/19/2016 xcv]
	add glusTiling
	add glusAxis
	add glusAxis3D
*/




//
// draw a single z-axis and a cone at end with the specify length
//
void 
glusAxis(
_In_	GLdouble	_length)
{
	glPushMatrix();

	glLineWidth(2);
	glDisable(GL_DEPTH_TEST);

	//
	// draw z-axis
	//
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES); 
	{
		glVertex3d(0.0, 0.0, 0.0);	glVertex3d(0.0, 0.0, _length);
	}
	glEnd();
	glLineWidth(1);

	//
	// draw cone
	//
	glEnable(GL_LIGHTING);

	glusTranslate(0, 0, _length - 0.2);
	if (glusGetShadeLevel()==Glus_Shade_Wire)	glutWireCone(0.04, 0.2, 12, 9);
	else										glutSolidCone(0.04, 0.2, 12, 9);

	glPopMatrix();
	glEnable(GL_DEPTH_TEST);

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

	glPushMatrix();
	
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


	glPopMatrix();
}

void 
glusSphere(pvoid _pointer)
{
	glPushMatrix();

	if (glusGetShadeLevel() == Glus_Shade_Wire)		glutWireSphere(1, 24, 16);
	else											glutSolidSphere(1, 24, 16);

	glPopMatrix();
}

void
glusTeapot(pvoid _pointer)
{
	glPushMatrix();

	if (glusGetShadeLevel() == Glus_Shade_Wire)		glutWireTeapot(1);
	else											glutSolidTeapot(1);

	glPopMatrix();
}

void
glusCube(pvoid _pointer)
{
	glPushMatrix();

	glShadeModel(GL_FLAT);
	if (glusGetShadeLevel() == Glus_Shade_Wire)		
		glutWireCube(2);
	else											
		glutSolidCube(2);
	
	glShadeModel(GL_SMOOTH);
	glPopMatrix();
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

	GLint max_x = 15;
	GLint max_z = 15;

	
	/*
	 *	draw align x-axis
	 */
	for (GLint z = -max_z; z <= max_z; z++)
	{
		glLineWidth((!z) * 3 + 1);
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
		glLineWidth((!x) * 3 + 1);
		glBegin(GL_LINES);
			glVertex3i(x, 0, -max_z);
			glVertex3i(x, 0, max_z);
		glEnd();
	}

	glPopMatrix();

	glEnable(GL_LIGHTING);
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
	// check the param
	int n = (int)ptr;
	if (n > 10) glusLog("level is too high.\n");
	if (n <= 0) n = 3;


	glDisable(GL_LIGHTING);

	glusMoveTo(0, 0, 0);				glusKoch(60,  9, n);
	glusMoveTo(4.5, 9*cosa(30), 0);		glusKoch(-60, 9, n);
	glusMoveTo(9, 0, 0);				glusKoch(180, 9, n);
}

// add [12/26/2016 xue]
void
glusShapeDraw(
_In_	PGlusShape	_p)
{
	assertp(_p);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glColor4fv((GLfloat*)&_p->Diffuse);	// no material

	
	//
	// transform
	//
	glusTransform(&_p->Transform);

	/*
	 *	material
	 */
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat*)&_p->Diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat*)&_p->Specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat*)&_p->Ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, (GLfloat*)&_p->Shininess);

	glEnable(GL_DEPTH_TEST);
	//
	// we just call the function already setting 
	//
	_p->Draw(_p->Extern);

	glPopMatrix();
}

void glusSquare(pvoid p)
{
	glPushMatrix();

	if (glusGetShadeLevel() == Glus_Shade_Wire)
		glBegin(GL_LINE_LOOP);
	else
		glBegin(GL_QUADS);

	glNormal3f(0, 0, 1);
	glVertex2f(-1, -1);
	glVertex2f(1, -1);
	glVertex2f(1, 1);
	glVertex2f(-1, 1);

	glEnd();
	glPopMatrix();
}

void
glusCylinder(pvoid _pointer)
{
	glPushMatrix();

	if (glusGetShadeLevel() == Glus_Shade_Wire)		
		glutWireCylinder(1,1, 24, 16);
	else											
		glutSolidCylinder(1,1, 24, 16);

	glPopMatrix();
}

void
glusCone(pvoid _pointer)
{
	glPushMatrix();

	if (glusGetShadeLevel() == Glus_Shade_Wire)
		glutWireCone(1, 1, 24, 16);
	else
		glutSolidCone(1, 1, 24, 16);

	glPopMatrix();
}
