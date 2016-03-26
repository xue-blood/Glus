#include "Canvas.h"

#include <math.h>
#include <stdio.h>


#define _ScreeenWidth 640
#define _ScreenHeight 480
#define _ScreenCenter _ScreeenWidth/2,_ScreenHeight/2

#define _Length 30

Canvas cvs(_ScreeenWidth, _ScreenHeight, "Canvas");

void draw(char *str)
{
	for (GLsizei i = 0; str[i];i++)
	{
		switch (str[i])
		{
		case 'F':
			cvs.Forward(_Length, 1);
			break;
		case 'L':
			cvs.Turn(60);
			break;
		case 'R':
			cvs.Turn(-60);
			break;
		default:
			break;
		}
	}
}

#define _TempFile "r:\\temp.txt"
#define _Buff_Size 100

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clear the screen

	FILE *file;
	fopen_s(&file, _TempFile, "r");

	char buffer[_Buff_Size] = { 0 };
	
	cvs.MoveTo(_ScreenCenter);
	
	while (fread_s(buffer, _Buff_Size, sizeof(char), 1, file))
		draw(buffer);

	glutSwapBuffers();	// swap the color buffer
	fclose(file);

}
void main(int argc, char **argv)
{
	glColor3f(.9, .0, .0);
	glClearColor(.3, .3, .3, 1);

	glutDisplayFunc(display);


	glutMainLoop();
}

