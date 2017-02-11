#include "..\inc\glus.h"


// enable debug ,global
FILE	*_Glus_Std_Null;
FILE	*_Glus_Logs[5] = { 0 };
int		_Glus_Log_level = Glus_Log_Normal;

extern FILE * _F_In;
extern GlusSink	_CurrentState;

extern int		_Window_Height, _Window_Width;


void m_exit(void)
{
	glusLog("Main exit.\n");

	fclose(_Glus_Std_Null);

	if (_F_In)	fclose(_F_In);

	glusSinkClear(&_CurrentState);
}



void main(int argc,char **argv)
{
	glutInit(&argc, argv);
	
	/*
	 *	init the debug stream
	 *	default the debug is off
	 */
	fopen_s(&_Glus_Std_Null,"nul", "w");
	glusLogLevel(0);

	

	atexit(m_exit);

	// call user define function
	glusInit();


	glutMainLoop();
}

int
glusLogLevel(
_In_	int		_level)
{
	int i = 0;
	for (; i <= _level;	i++)
		_Glus_Logs[i] = stdout;
	for (; i <= Glus_Log_Info;		i++)
		_Glus_Logs[i] = _Glus_Std_Null;

	int old = _Glus_Log_level;
	_Glus_Log_level = _level;

	return old;
}



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



/*
*	single control  ,refer:http://stackoverflow.com/questions/3294972/setting-max-frames-per-second-in-opengl
*/
static int redisplay_interval;

void timer(int a) 
{
	if (redisplay_interval == 0)	// needn't not auto refresh
		return;

	glutPostRedisplay();
	glutTimerFunc(redisplay_interval, timer, 0);
}

void glusFPS(int fps)
{
	if (fps == 0)	// needn't not auto refresh
		return;
	redisplay_interval = 1000 / fps;
	glutTimerFunc(redisplay_interval, timer, 0);
}
