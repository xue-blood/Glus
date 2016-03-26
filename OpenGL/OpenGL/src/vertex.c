#include "..\inc\glus.h"

void 
glusInitWin(
GLint _left, GLint _top, GLint _width, GLint _height, char *_name, GLenum _mode)
{
	glutInitDisplayMode(	_mode);
	glutInitWindowPosition(	_left,		_top);
	glutInitWindowSize(		_width,		_height);
	glutCreateWindow(		_name);
}


