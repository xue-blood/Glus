#include "../inc/glus.h"

int 
glusWinTitleHeight()
{
#ifdef WIN32
	return GetSystemMetrics(SM_CYSIZE)/2;
#else
	return 0;	// current not support
#endif
}

/*
*	convert the mouse input to world point
*	it will auto handle inveres y and title bar height
*/
// create [9/4/2016 blue]
void
glusWinToWorld(
_In_ int _x, _In_ int _y,
_Inout_	PGlusVector _world)
{
	assertp(_world);


	float ZDepth;
	glReadPixels(_x, _y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &ZDepth);

	glusWinToWorldex(_x, _y, ZDepth, _world);
}

/*
 *	window coord to world coord
 *	(with the z-depth)
 */
void
glusWinToWorldex(
_In_ int _x, _In_ int _y, int _z,
_Inout_	PGlusVector _world)
{
	assertp(_world);

	GLint		r_viewport[4];
	GLdouble	m_modelview[16], m_projection[16];


	glGetDoublev(GL_MODELVIEW_MATRIX, m_modelview);	// get the modelview matrix
	glGetDoublev(GL_PROJECTION_MATRIX, m_projection);	// get the projection matrix
	glGetIntegerv(GL_VIEWPORT, r_viewport);	// get the viewport rect

	_y = r_viewport[3] - _y + 2;	// convert the y for mouse to window
	_y -= glusWinTitleHeight();		// the title bar height
	// unproject the window coordinate to world 
	gluUnProject(_x, _y, 0, m_modelview, m_projection, r_viewport, &_world->X, &_world->Y, &_world->Z);
	_world->V = 1;

	glusLog("\nwin: (%d,%d) <--> world: (%.2lf,%.2lf,%.2lf)", _x, _y, _world->X, _world->Y, _world->Z);
}