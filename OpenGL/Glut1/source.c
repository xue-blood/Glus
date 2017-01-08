#include "Header.h"
#include <process.h>


PGlusScene Scene =NULL;


PPeano snow;

int level;

PPixMap pic[2];

int x, y;

void dispaly(void)
{

	//glusSceneLight(Scene);

	snow->Level = level;
	

	glusSceneDraw(Scene);	

	glusUIEnter();

	//glEnable(GL_COLOR_LOGIC_OP);
	glLogicOp(GL_XOR);
	glRasterPos2i(x-16, y+8);

	//glPixelZoom(0.5, -0.5);
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


void data()
{

	Scene = glusSceneNew("scene.sdl");
	snow = (PPeano)glusSceneGetShapeByName(Scene, "snow")->Extern;
	level = snow->Level;

	pic[0] = pixCheckboard();
	pixChromaKey(pic[0], 0, 0, 0);
	pic[1] = glusMandelbrotSet(160, 120, 0.5);
	
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

void move(int cx, int cy)
{
	x=cx, y= glusGetHeight() - cy;
}

void reshape(int w, int h)
{
	glusReshape(w, h);
	glusSceneReshape(Scene, w, h);
}

void func()
{

	glutDisplayFunc(dispaly);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(move);
	glutMotionFunc(move);
	glutCloseFunc(clear);
	glutReshapeFunc(reshape);

	glusHideCursor();
	
	glusFPS(30);


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