#include "Header.h"

PGlusScene Scene =NULL;

void dispaly(void)
{
	glusSceneDraw(Scene);
	
	glutSwapBuffers();
}

void 
keyboard(
unsigned char _key, 
int _x, int _y)
{
	switch (_key)
	{
	case 'q': glutExit(); break;
	
	default:
		break;
	}
}

void clear(void)
{
	printf("exit\n");

	glusSceneUnload(Scene);
}

void tm(int id)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glutPostRedisplay();

	glutTimerFunc(1000, tm, 0);
}
void glusInit()
{
	printf("init\n");

	glusInitWin(300, 100, 600, 400, "glut1", GLUT_DOUBLE | GLUT_RGB);

	Scene = glusSceneLoad("scene.sdl");
	glusSceneLight(Scene);

	glutDisplayFunc(dispaly);
	glutKeyboardFunc(keyboard);
	glutCloseFunc(clear);

	glutTimerFunc(1000, tm, 0);
}   