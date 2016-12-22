#include "Header.h"
#include "../OpenGL//inc/rgbpixmap.h"
#include <process.h>

PGlusScene Scene =NULL;
Peano pea;
#define TIME 30
int level = 5;

void dispaly(void)
{

	glusSceneLight(Scene);

	glusSceneDraw(Scene);
	
	glColor3f(1, 1, 1);

	glusMoveTo(1, 1, 0);	glusTurnTo(0);
	glusPeanoDraw(&pea, level);
	
	glusTurn(120);
	glusPeanoDraw(&pea, level); 

	glusTurn(120);
	glusPeanoDraw(&pea, level);


	
	glutSwapBuffers();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
	case ' ': glusCameraSlide(0, .1, 0, &Scene->Camera); break;
	case 'x': glusCameraSlide(0, -.1, 0, &Scene->Camera); break;
	case 'd': glusCameraSlide(.1, 0, 0, &Scene->Camera); break;
	case 'a': glusCameraSlide(-.1, 0, 0, &Scene->Camera); break;
	case 'q': glusCameraRoll(1, &Scene->Camera); break;
	case 'e': glusCameraRoll(-1, &Scene->Camera); break;
	case 'j': glusCameraYaw(-1, &Scene->Camera); break;
	case 'l' : glusCameraYaw(1, &Scene->Camera); break;
	case 'i': glusCameraPitch(-1, &Scene->Camera); break;
	case 'k': glusCameraPitch(1, &Scene->Camera); break;
	case '=': glusSetShadeLevel(Glus_Shade_Wire); break;
	case '+': glusSetShadeLevel(Glus_Shade_Solid); break;
	case ',': glusPeanoProduc(&pea, --level); printf("\rlevel :%d", level); break;
	case '.': glusPeanoProduc(&pea, ++level); printf("\rlevel :%d", level); break;
	default:
		break;
	}
}

void clear(void)
{
	printf("\nexit\n");

	glusSceneClear(Scene);
}

void tm(int id)
{
	glutPostRedisplay();

	glutTimerFunc(TIME, tm, 0);
}

void data()
{

	Scene = glusSceneNew("scene.sdl");

	glusPeano("snow.pea", &pea, level);
	
}

void set()
{
	
	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	
	
}

void edit(pvoid p)
{
	while (true)
	{
		putchar('>');
		glusSDLex(Scene, fgetstdin());
	}
}

void func()
{

	glutDisplayFunc(dispaly);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutCloseFunc(clear);

	glutTimerFunc(TIME, tm, 0);

}
void glusInit()
{
	printf("init\n");



	glusInitWin(300, 100, 600, 400, "glut1", GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	set();
	data();
	func();

	_beginthread(edit,0,0);

}   