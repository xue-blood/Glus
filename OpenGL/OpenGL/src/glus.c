#include "..\inc\glus.h"


// enable debug ,global
FILE	*_Glus_Std_Debug, *_Glus_Std_Null;

void m_exit(void)
{
	glusLog("Main exit.\n");

	fclose(_Glus_Std_Null);
}

void main(int argc,char **argv)
{
	glutInit(&argc, argv);
	
	/*
	 *	init the debug stream
	 *	default the debug is off
	 */
	fopen_s(&_Glus_Std_Null,"nul", "w");
	_Glus_Std_Debug = _Glus_Std_Null;

	atexit(m_exit);

	// call user define function
	glusInit();

	glutMainLoop();
}

void
glusDebugEnable(
_In_	bool	_is_debug)
{
	if (_is_debug)
		_Glus_Std_Debug = stdout;
	else
		_Glus_Std_Debug = _Glus_Std_Null;
}
