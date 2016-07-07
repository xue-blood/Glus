#include "Header.h"

glusLink(Head);
glusLinks(Ha);
glusLinks(Hb);
glusLink(P);

GlusPolygon A[4], B[4];

int Width, Height;

GlusRay ray, nRay;
void redata()
{
	glusP2i(15, 0, &ray.Point);
	glusV2i(12, 3, &ray.Direction);
	memset(&nRay, 0, sizeof(GlusRay));

}
void initData()
{

	glusP2i(-20,0, &A[0].Point);
	glusP2i(20, 0, &A[1].Point);
	glusP2i(20, 15, &A[2].Point);
	glusP2i(-20, 15, &A[3].Point);

	glusP2i(-5, -5, &B[0].Point);
	glusP2i(5, -5, &B[1].Point);
	glusP2i(5, 10, &B[2].Point);
	glusP2i(-5,10, &B[3].Point);
 	
	glusLinkInsertTail(&Ha.Data, A);
	glusLinkInsertTail(&Ha.Data, A + 1);
	glusLinkInsertTail(&Ha.Data, A + 2);
	glusLinkInsertTail(&Ha.Data, A + 3);

	glusLinkInsertTail(&Hb.Data, B);
	glusLinkInsertTail(&Hb.Data, B + 1);
	glusLinkInsertTail(&Hb.Data, B + 2);
	glusLinkInsertTail(&Hb.Data, B + 3);

	glusLinkInsertTail(&Head, &Ha);
	glusLinkInsertTail(&Head, &Hb);

	redata();

}

void dispaly(void)
{
	
	glusDrawCoord();

	glColor3f(1, 1, 1);
	glusDrawPolygon(&Ha.Data);

	glColor3f(0, 1, 1);
	glusDrawPolygon(&Hb.Data);

	glusPolygonUnion(&Ha.Data, &Hb.Data, &P);
	glColor3f(1, 1, 0);
	glusDrawPolygon(&P);

	

	glutSwapBuffers();
	glClear(GL_COLOR_BUFFER_BIT);

}

void
mouse(
int _button, int _state, int _x, int _y)
{

	glutPostRedisplay();
}

void 
keyboard(
unsigned char _key, 
int _x, int _y)
{
	switch (_key)
	{
	case 'q': glutExit(); break;
	case 'r': redata(); break;
	default:
		break;
	}
}

void clear(void)
{
	printf("exit\n");

}

void reshape(int _width,int _height)
{
	int barHeight = GetSystemMetrics(SM_CYCAPTION);
	_height -= barHeight;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(- _width/2, _width/2, - _height/2, _height/2);

	Width = _width, Height = _height;

	printf("%d,%d\n", _width, _height);

}

void tm(int id)
{
	glutPostRedisplay();

	glutTimerFunc(1000, tm, 0);
}
void glusInit()
{
	printf("init\n");

	glusInitWin(300, 100, 600, 400, "glut1", GLUT_DOUBLE | GLUT_RGB);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-30, 30, -20, 20);

	

	glClearColor(0.3, 0.3, 0.3, 1.0);
	glColor3f(1, 1, 1);
	glPointSize(2.0);

	glutCloseFunc(clear);


	glutDisplayFunc(dispaly);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	//glutReshapeFunc(reshape);


	initData();

	glutTimerFunc(1000, tm, 0);
}   