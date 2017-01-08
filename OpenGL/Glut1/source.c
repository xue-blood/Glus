#include "Header.h"
#include <process.h>

#define Height 400

PGlusScene Scene =NULL;

#define TIME 30

PPeano snow;

int level;

PPixMap pic[2];

void dispaly(void)
{

	//glusSceneLight(Scene);

	snow->Level = level;
	

	glusSceneDraw(Scene);	

	glusUIEntry();
	//glEnable(GL_COLOR_LOGIC_OP);
	glLogicOp(GL_INVERT);
	glRasterPos2i(30, 30);
	pixDraw(pic[0]);

	glRasterPos2i(94, 30);
	pixBlend(pic[0]);

	glDisable(GL_COLOR_LOGIC_OP);
	glusUILeave();

	glutSwapBuffers();

	glClearColor(.3, .3, .3, 1);
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
	case ',': printf("\rlevel: %d", --level); break;
	case '.': printf("\rlevel: %d", ++level); break;
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
	snow = (PPeano)glusSceneGetShapeByName(Scene, "snow")->Extern;
	level = snow->Level;

	pic[0] = pixCheckboard();
	pixChromaKey(pic[0], 0, 0, 0);
	pic[1] = glusMandelbrotSet(40, 60, 0.5);
	
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

void move(int x, int y)
{
	//glRasterPos2i(x, Height - y);
}
void func()
{

	glutDisplayFunc(dispaly);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(move);

	glutCloseFunc(clear);


	glutTimerFunc(TIME, tm, 0);

}
void glusInit()
{
	printf("init\n");



	glusInitWin(300, 100, 600, Height, "glut1", GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	set();
	data();
	func();

	_beginthread(edit,0,0);

}   