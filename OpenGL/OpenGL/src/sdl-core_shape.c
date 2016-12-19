#include "../inc/glus.h"

/*
*	shapes
*/

#define Shapes_count 6

static str  Shapes_name[Shapes_count] =
{
	"axis", "sphere", "cube", "teapot", "grid", "snow"
};

static void(*Shapes_func[Shapes_count])(pvoid) =
{
	glusAxis3D, glusSphere, glusCube, glusTeapot, glusGrid, glusKochSnow
};

void shape(PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
{
	/*
	*	get the shape name
	*/
	char name[20];
	glusFileScanf(file, "%s", name, _countof(name));

	/*
	*	find the shape-draw function
	*/
	int i = 0;
	for (; i < Shapes_count; i++)
	{
		if (strequ(name, Shapes_name[i]))
			break;
	}
	if (i == Shapes_count) { glusLog("Shape name not found.\n"); return; }

	// create a new shape and add to scene
	PGlusShape p = glusSceneCreateNewShape(_scene);

	// set the draw function
	p->Draw = Shapes_func[i];

	/*
	*	try get some parameter
	*/
	int pa, n; 
	glusFileScanfex(file, n, "%d", &pa);
	if (n > 0)	p->Extern = (pvoid)pa;
}
