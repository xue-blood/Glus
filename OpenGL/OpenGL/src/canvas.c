#include "../inc/glus.h"

/*
 *	the height and width of current windows
 */
int		_Window_Height, _Window_Width;

static GlusVector	_CurrentPosition = { 0, 0, 0, 1 };

static double		_CurrentDirection = 0;

glusSink(_CurrentState);

typedef	struct _GlusCS
{
	GlusSink	Sink;
	GlusVector	CP;
	double		CD;
}GlusCS, *PGlusCS;

GlusVector
glusGetCP()
{
	return _CurrentPosition;
}



/*
//  [7/6/2016 Tld]
glusLIntersect	change,convert to parameter PGlusLine to PGlusVector
*/
void
glusLDraw(
_In_	PGlusLine	_pLine)
{
	assertp(_pLine);

	glBegin(GL_LINES);
	{
		glVertex3dv((pGLdouble)&_pLine->A);
		glVertex3dv((pGLdouble)&_pLine->B);
	}
	glEnd();
}

void
glusDrawCoord()
{
	glBegin(GL_LINES);
	{

		glColor3f(1.0, 0, 0); glVertex3i(-100, 0, 0); glVertex3i(100, 0, 0); // x
		glColor3f(0, 1.0, 0); glVertex3i(0, -100, 0); glVertex3i(0, 100, 0); // y
		glColor3f(0, 0, 1.0); glVertex3i(0, 0, -100); glVertex3i(0, 0, 100); // z

	}
	glEnd();
}

Glus_Intersect
glusLIntersect(
_In_	PGlusVector	_laa,
_In_	PGlusVector	_lab,
_In_	PGlusVector	_lba,
_In_	PGlusVector	_lbb,
_Out_	PGlusVector _p)
{
	assertp(_laa && _lab && _lba && _lbb);
	assertp(_p);

	//
	// compute the vectors 
	//
	GlusVector	vb = glusVector3fv(_laa, _lab),
		vd = glusVector3fv(_lba, _lbb),
		vc = glusVector3fv(_laa, _lba);

	// compute the perpendicular vector
	GlusVector vdn, vbn;
	glusVNormal(&vb, &vbn);
	glusVNormal(&vd, &vdn);
	//
	// compute the parameter and the vector
	//
	GLdouble	r, s, t;
	r = glusVDotPro((&vb), (&vdn));

	// are lines be parallel
	if (r == 0)
		return Intersect_Parallel;

	t = glusVDotPro((&vc), (&vdn)) / r;

	//
	// compute s 
	// 
	glusVOpposite(&vc);
	s = glusVDotPro(&vc, &vbn) / glusVDotPro(&vd, &vbn);

	glusVAdd(_laa, 1, (&vb), t, _p);
	_p->V = 1;

	// is the intersection exsit
	if (t > 1 || t < 0 || s >1 || s < 0)
		return Intersect_Not_Exsit;
	return Intersect_Exsit;
}




/*
*	draw line relative from current position
*/
// create [10/25/2016 blue]
void
glusLineRel(
_In_	double	_x,
_In_	double	_y,
_In_	double	_z)
{
	if (_x == 0 && _y == 0 && _z == 0)	return;

	GlusVector	cp = glusGetCP(), next;
	next.X = cp.X + _x;
	next.Y = cp.Y + _y;
	next.Z = cp.Z + _z;
	next.V = 1;

	glBegin(GL_LINES);
	{
		glVertex3d(cp.X, cp.Y, cp.Z);
		glVertex3d(next.X, next.Y, next.Z);
	}
	glEnd();

	glusMoveTo(next.X, next.Y, next.Z);
}

/*
*	move current position
*/
// create [10/25/2016 blue]
void
glusMoveTo(
_In_	double	_x,
_In_	double	_y,
_In_	double	_z)
{
	_CurrentPosition.X = _x;
	_CurrentPosition.Y = _y;
	_CurrentPosition.Z = _z;
}

/*
*	draw line
*/
// create [10/25/2016 blue]
void
glusLineTo(
_In_	double	_x,
_In_	double	_y,
_In_	double	_z)
{
	if (_x == 0 && _y == 0 && _z == 0)	return;

	GlusVector cp = glusGetCP();

	glBegin(GL_LINES);
	{
		glVertex3d(cp.X, cp.Y, cp.Z);
		glVertex3d(_x, _y, _z);
	}
	glEnd();

	glusMoveTo(_x, _y, _z);

}

// add [12/22/2016 xue]
void glusTurn(double _angle)
{
	_CurrentDirection += _angle;
}

// add [12/22/2016 xue]

void glusTurnTo(double _angle)
{
	_CurrentDirection = _angle;
}

// add [12/22/2016 xue]

void
glusForward(
_In_	double	_dist,
_In_	bool	_is_visible)
{
	GLfloat x, y;
	x = _CurrentPosition.X + _dist*cos(_CurrentDirection*3.14 / 180);
	y = _CurrentPosition.Y + _dist*sin(_CurrentDirection*3.14 / 180);

	if (_is_visible)
		glusLineTo(x, y, 0);
	else
		glusMoveTo(x, y, 0);
}

// add [12/22/2016 xue]

void
glusPushCS()
{
	PGlusCS cs; glusAllocex(cs, GlusCS, 1, return);

	cs->CD = _CurrentDirection;
	cs->CP = _CurrentPosition;

	glusSinkPush(&_CurrentState, (PGlusSink)cs);
}

// add [12/22/2016 xue]
void
glusPopCS()
{
	PGlusCS cs = (PGlusCS)glusSinkPop(&_CurrentState);
	if (!cs) return;

	_CurrentDirection = cs->CD;
	_CurrentPosition = cs->CP;

	glusFree(cs);
}

/*
 *	entry 2d draw mode
 *	refer : https://www.opengl.org/discussion_boards/showthread.php/123009-glRasterPos-problems
 */
void 
glusUIEnter()
{
	// Set matrix mode
	glMatrixMode(GL_PROJECTION);
	// push current projection matrix on the matrix stack
	glPushMatrix();
	// Set an ortho projection based on window size
	glLoadIdentity();
	glOrtho(0, _Window_Width, 0, _Window_Height, 0, 1);

	// Switch back to model-view matrix
	glMatrixMode(GL_MODELVIEW);
	// Store current model-view matrix on the stack
	glPushMatrix();
	// Clear the model-view matrix
	glLoadIdentity();

	// You can specify this in window coordinates now
}

void
glusUILeave()
{
	// Restore the model-view matrix
	glPopMatrix();

	// Switch to projection matrix and restore it
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// Switch back to model-view matrix. This isn't
	// strictly necessary, but I tend to try and
	// keep the model-view matrix as the current
	// matrix to keep things simple.
	glMatrixMode(GL_MODELVIEW);
}


int glusGetHeight()
{
	return _Window_Height;
}

int glusGetWidth()
{
	return _Window_Width;
}

void glusReshape(int w,int h)
{
	_Window_Width	= w;
	_Window_Height	= h;

	// Set the viewport to be the entire window
	// refer: http://www.lighthouse3d.com/tutorials/glut-tutorial/preparing-the-window-for-a-reshape/
	glViewport(0, 0, w, h);

}

void 
bresenham(int ax, int ay, int bx, int by)
{
	/*
	// line is vertical
	*/
	if (ax == bx) 
	{
		for (; ay <= by; ay++)
			drawPoint(ax, ay);
		return;
	}
	/*
	 *	line is horizontal
	 */
	if (ay==by)
	{
		for (; ax <= bx; ax++)
			drawPoint(ax, ay);
		return;
	}

	if (ax > bx)
	{
		int y = by, W = bx - ax, H = by - ay;
		int	F = 2 * H - W;

		for (int x = bx; x <= ax; x++)
		{
			drawPoint(x, y);
			/*
			*	find the next F
			*/
			if (F < 0)	F += 2 * H;
			else		y++, F += 2 * (H - W);

		}
		return;
	}

	/*
	 *	restiction a.x < b.x, 0 < H/W < 1
	 */
	int y = ay, W = bx - ax, H = by - ay;
	int	F = 2 * H - W;

	for (int x = ax; x <= bx; x++)
	{
		drawPoint(x, y);
		/*
		*	find the next F
		*/
		if (F < 0)	F += 2 * H;
		else		y++, F += 2 * (H - W);

	}
}