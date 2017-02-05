#include "../inc/glus.h"

/*
*	shapes
*/

#define Shapes_count 9

static str  Shapes_name[Shapes_count] =
{
	"axis", "sphere", "cube", "teapot", "grid", "snow", "square", "cylinder", "cone"
};

static void(*Shapes_func[Shapes_count])(pvoid) =
{
	glusAxis3D, glusSphere, glusCube, glusTeapot, glusGrid, glusKochSnow,glusSquare,glusCylinder,glusCone
};

static bool(*Shapes_func_hit[Shapes_count])(PGlusShape _s, PGlusRay _r, PGlusIntersect _inter) = 
{
	NULL, glusHitSphere, glusHitCube, NULL, NULL, NULL,glusHitSquare,glusHitCylinder,glusHitCone
};

void shape(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	/*
	*	get the shape name
	*/
	char name[20];
	glusScanf(file, "%s", name, _countof(name));
	name[19] = 0;

	/*
	*	find the shape-draw function
	*/
	int i = 0;
	for (; i < Shapes_count; i++)
	{
		if (strequ(name, Shapes_name[i]))
			break;
	}
	if (i == Shapes_count) { glusLog("\nShape name not found."); return; }

	// create a new shape and add to scene
	PGlusShape p = glusSceneCreateNewShape(_scene);

	// set the draw function
	p->Draw = Shapes_func[i];

	// hit function
	p->Hit = Shapes_func_hit[i];

	/*
	*	try get some parameter
	*/
	int pa, n; 
	n= glusScanf(file, "%d", &pa);
	if (n > 0)	p->Extern = (pvoid)pa;
}
