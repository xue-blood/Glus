#include "Header.h"

PGlusScene Scene =NULL;
#define TIME 30

void dispaly(void)
{
	glusSceneDraw(Scene);

	glusGrid(0);
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
	case '0': glutExit(); break;
	case 'w': glusCameraSlide(0, 0, -.1, &Scene->Camera); break;
	case 's': glusCameraSlide(0, 0, .1, &Scene->Camera); break;
	case 'd': glusCameraSlide(.1, 0, 0, &Scene->Camera); break;
	case 'a': glusCameraSlide(-.1, 0, 0, &Scene->Camera); break;
	case 'q': glusCameraRoll(1, &Scene->Camera); break;
	case 'e': glusCameraRoll(-1, &Scene->Camera); break;
	case 'j': glusCameraYaw(-1, &Scene->Camera); break;
	case 'l' : glusCameraYaw(1, &Scene->Camera); break;
	case 'i': glusCameraPitch(-1, &Scene->Camera); break;
	case 'k': glusCameraPitch(1, &Scene->Camera); break;

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

	glutTimerFunc(TIME, tm, 0);
}

void data()
{
	
}
void glusInit()
{
	printf("init\n");

	glusInitWin(300, 100, 600, 400, "glut1", GLUT_DOUBLE | GLUT_RGB);

	Scene = glusSceneLoad("scene.sdl");
	glusSceneLight(Scene);

	//glusDebugEnable(true);

	data();
	
	glutDisplayFunc(dispaly);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutCloseFunc(clear);

	glutTimerFunc(TIME, tm, 0);

	
}   