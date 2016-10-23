#include "Header.h"
#include "../OpenGL//inc/rgbpixmap.h"

PGlusScene Scene =NULL;
#define TIME 30
void tex()
{
	PRgbMap tex = rgbCheckboard();
	rgbSetTexture(tex, 200);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	

	glBindTexture(GL_TEXTURE_2D, 200);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(0, 0);
	glTexCoord2f(0, 1); glVertex2f(0, 1);
	glTexCoord2f(1,1); glVertex2f(1, 1);
	glTexCoord2f(1,0); glVertex2f(1, 0);
	glEnd();
}
void dispaly(void)
{
	glShadeModel(GL_FLAT);

	

	glusSceneLight(Scene);

	glusSceneDraw(Scene);

	glColor3f(.3, .3, .3);
	tex();
	glusGrid(0);
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

	Scene = glusSceneLoad("scene.sdl");

	
}

void set()
{
	//glusDebugEnable(true);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	
	//glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_DEPTH_TEST);
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
}   