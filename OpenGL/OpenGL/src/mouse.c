#include "..\inc\glus.h"


// global var for mouse function link list
glusLink(_Glus_Mouse_Head);

/*
 *	convert the mouse input to world point
 */
// create [9/4/2016 blue]
void	
glusWinToWorld(
_In_ int _x,	_In_ int _y, 
_Inout_	PGlusVector _world)
{
	assertp(_world);


	GLint		r_viewport[4];
	GLdouble	m_modelview[16], m_projection[16];


	glGetDoublev(GL_MODELVIEW_MATRIX, m_modelview);	// get the modelview matrix
	glGetDoublev(GL_PROJECTION_MATRIX, m_projection);	// get the projection matrix
	glGetIntegerv(GL_VIEWPORT, r_viewport);	// get the viewport rect

	_y = r_viewport[3] - _y - 1;	// convert the y for mouse to window

	// unproject the window coordinate to world 
	gluUnProject(_x, _y, 0.5, m_modelview, m_projection, r_viewport, &_world->X, &_world->Y, &_world->Z);
	_world->V = 1;

	glusDebug("\nwin: (%d,%d) <--> world: (%.2lf,%.2lf,%.2lf)", _x, _y, _world->X, _world->Y, _world->Z);
}

/*
 *	some mouse handle
 */
/*
 *	mouse select function
 */
void glusMouseSelect(int button, int state, int x, int y, pvoid p)
{
	static int x1, y1, x2, y2;
	static bool is_valid = false;

	GlusVector world;
	glusWinToWorld(x, y, &world);

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			x1 = (int)world.X;
			y1 = (int)world.Y;
		}
		else
		{
			x2 = (int)world.X, y2 = (int)world.Y;
			is_valid = true;
		}
	}

	if (is_valid)
	{
		glBegin(GL_LINE_LOOP);
			glVertex2i(x1, y1); 
			glVertex2i(x1, (int)world.Y);
			glVertex2i((int)world.X, (int)world.Y);
			glVertex2i((int)world.X, y1);
		glEnd();
		glutPostRedisplay();
		is_valid = false;
	}
}

/*
 *	mouse function loop
 */
// create [9/5/2016 blue]
void 
glusMouseLoop(
_In_	int _button,_In_ int _state,
_In_	int	_x,		_In_ int _y)
{
	PGlusMouse	p = (PGlusMouse)_Glus_Mouse_Head.BLink;

	while (!glusLinkIsHead(p,&_Glus_Mouse_Head))
	{
		if (p->glusMouse)
			p->glusMouse(_button,_state,_x, _y, p->Custom); // just call the function

		p = (PGlusMouse)p->Link.BLink;
	}
}

/*
 *	insert new function to link list
 */
// create [9/5/2016 blue]
void	
glusMouseInsert(
_In_	pvoid	_p_function,
_In_	pvoid	_p_data)
{
	assertp(_p_function);

	/*
	 *	allocate memory for struct
	 */
	PGlusMouse p_mouse;
	glusAllocex(p_mouse, GlusMouse,1, );

	p_mouse->glusMouse = _p_function;
	p_mouse->Custom = _p_data;

	glusLinkInsertTail(&_Glus_Mouse_Head, p_mouse);	// insert to tail
}

/*
 *	remove from link list
 */
void
glusMouseRemove(
_In_	pvoid	_p_function)
{
	assertp(_p_function);

	PGlusMouse p_mouse = (PGlusMouse)_Glus_Mouse_Head.BLink;
	while (!glusLinkIsHead(p_mouse, &_Glus_Mouse_Head))
	{
		// find the entry
		if (p_mouse->glusMouse == _p_function)
		{
			glusLinkRemoveEntry(p_mouse);// remove it
			return;
		}

		p_mouse = (PGlusMouse)p_mouse->Link.BLink;
	}
}