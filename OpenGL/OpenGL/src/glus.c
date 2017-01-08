#include "..\inc\glus.h"


// enable debug ,global
FILE	*_Glus_Std_Null;
FILE	*_Glus_Logs[5] = { 0 };

extern FILE * _F_In;
extern GlusSink	_CurrentState;


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

void
glusLogLevel(
_In_	int		_level)
{
	int i = 0;
	for (; i <= _level;	i++)
		_Glus_Logs[i] = stdout;
	for (; i <= Glus_Log_Info;		i++)
		_Glus_Logs[i] = _Glus_Std_Null;
}
