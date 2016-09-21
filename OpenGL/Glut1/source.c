#include "Header.h"

PGlusScene Scene =NULL;


void dispaly(void)
{
	glusSceneDraw(Scene);

	glutSwapBuffers();

	glClear(GL_COLOR_BUFFER_BIT);
}

void 
mouse(int button,int state,int x,int y)
{
	glusMouseLoop(button, state, x, y);
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
	glutPostRedisplay();

	glutTimerFunc(1000, tm, 0);
}

void data()
{
	
	PGlusMesh mesh = glusMeshSurface(20, 0, Glus_PI  , 20, -Glus_PI / 4, Glus_PI/2, glusSurfaceSphere);
	glusMeshAddToScene(mesh, Scene);
}
void glusInit()
{
	printf("init\n");

	glusInitWin(300, 100, 600, 400, "glut1", GLUT_DOUBLE | GLUT_RGB);

	Scene = glusSceneLoad("scene.sdl");
	glusSceneLight(Scene);

	glusDebugEnable(true);

	data();
	
	glutDisplayFunc(dispaly);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutCloseFunc(clear);

	glutTimerFunc(1000, tm, 0);

}   