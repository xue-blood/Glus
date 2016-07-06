#include "..\inc\glus.h"

void main(int argc,char **argv)
{
	glutInit(&argc, argv);
	
	// call user define function
	glusInit();

	glutMainLoop();
}