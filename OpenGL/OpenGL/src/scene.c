#include "..\inc\glus.h"
#include <strsafe.h>

/*
//  [8/19/2016 xcv]
	add glusSceneLoad	-	glusSceneUnload
	add glusSceneDefault
*/
#define	_Key_Unknown -1
#define Keys_n 14
str Keys[Keys_n] =
{
	"//",
	"/*",
	"background",
	"axis",
	"projection",
	"camera",
	"diffuse",
	"translate",
	"scale",
	"rotate",
	"sphere",
	"polyline",
	"cube",
	"mesh"
};
GLsizei Keys_func_param[Keys_n] = 
{
	0,	// comment
	0,	// comments
	3,	// background
	1,	// axis
	6,	// projection
	9,	// camera
	4,	// diffuse
	3,	// translate
	3,	// scale
	4,	// rotate
	0,	// sphere
	0,	// polyline
	0,	// cube
	0	// mesh
};

//
// key function
//
void comment(PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
{
	fscanf_s(file, "%*[^\n]\n");	// just skip all char at current line
}
void comments(PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
{
	fscanf_s(file, "%*[^*/]*/");	// just skip all char until find "*/"
}
void background(PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
{
	if (param_n == 3)
		_scene->Background.A = 1;
	_scene->Background.R = param[0];
	_scene->Background.G = param[1];
	_scene->Background.B = param[2];
}
void axis(		PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
{
	if (param[0]==0)
		_scene->EnableAxis = false;
	_scene->AxisLength = param[0];
}
// add support of perspective [9/24/2016 blue]
void projection(PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
{
	if (param_n == 6) // use ortho
	{
		_scene->Projection.IsPerspective = false;
		_scene->Projection.Ortho.Left		= param[0];
		_scene->Projection.Ortho.Right = param[1];
		_scene->Projection.Ortho.Bottom = param[2];
		_scene->Projection.Ortho.Top = param[3];
		_scene->Projection.Near		= param[4];
		_scene->Projection.Far		= param[5];
	}
	else
	{
		_scene->Projection.IsPerspective	= true;
		_scene->Projection.Persp.AngleView		= param[0];
		_scene->Projection.Persp.AspectRation = param[1];
		_scene->Projection.Near		= param[2];
		_scene->Projection.Far		= param[3];
	}
}
void camera(	PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
{
	glusCameraSet(param[0], param[1], param[2],
		param[3], param[4], param[5],
		param[6], param[7], param[8],
		&_scene->Camera);

}
void diffuse(	PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
{
	//
	// get the current shape
	//
	PGlusShape s = glusSceneGetLastShape(_scene);
	
	//
	// then set diffuse
	//
	s->Diffuse.R = param[0];
	s->Diffuse.G = param[1];
	s->Diffuse.B = param[2];
	s->Diffuse.A = (param_n == 3)? 1.0:		param[3];
}
void translate(	PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
{
	//
	// get the current shape
	//
	PGlusShape s = glusSceneGetLastShape(_scene);
	
	//
	// set translation
	//
	s->Transform.Dx = param[0];
	s->Transform.Dy = param[1];
	s->Transform.Dz = param[2];
}
void scale( PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
{
	//
	// get the current shape
	//
	PGlusShape s = glusSceneGetLastShape(_scene);

	//
	// set scale
	//
	s->Transform.Sx = param[0];
	s->Transform.Sy = param[1];
	s->Transform.Sz = param[2];
}
void rotate(PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
{
	//
	// get the current shape
	//
	PGlusShape s = glusSceneGetLastShape(_scene);


	//
	// set scale
	//
	s->Transform.Angle = param[0];
	s->Transform.Ax = param[1];
	s->Transform.Ay = param[2];
	s->Transform.Az = param[3];
}
void sphere(PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
{
	PGlusShape p = glusSceneCreateNewShape(_scene);

	p->Draw = glusSphere;
}
void polyline(PGlusScene _scene, pGLdouble p_param, GLsizei n_param, FILE *file)
{
	//
	// create a shape
	//
	PGlusShape p = glusSceneCreateNewShape(_scene);
	
	// the draw function
	p->Draw = glusDrawPolyLine;
	
	// and the clear function for we use link list
	p->Clear = glusPointsClear_L;

	//
	// malloc memory for points head
	//
	PGlusLink h = (PGlusLink)malloc(sizeof(GlusLink));
	if (!h)
		goto _polyline_failed_;
	else
		p->Extern = h;
	glusLinkInit(h);

	//
	// get the point number
	//
	Glusnum	n;
	glusFileScanf(file, "%d", &n);

	// get points' data
	glusFileLoadPoints_L(file, h, n);

	return;


_polyline_failed_:
	if (h)	glusLinkClear(h); glusFree(h);
	if (p)	glusFree(p);
}
void cube(PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
{
	// create a new shape and add to scene
	PGlusShape p = glusSceneCreateNewShape(_scene);

	// set the draw function
	p->Draw = glusCube;
}

/*
*	add support for mesh
*/
// add [8/31/2016 blue]
void mesh(PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
{
	/*
	 *	load a mesh from file
	 */
	PGlusMesh	p_mesh;
	if (!glusSuccess(glusMeshLoad(file, &p_mesh)))
		return;

	// add to scene
	if(!glusSuccess(glusMeshAddToScene(p_mesh, _scene)))
		return;
}
void(*Keys_func[Keys_n])(PGlusScene, pGLdouble, GLsizei,FILE*) =
{
	comment,
	comments,
	background,
	axis,
	projection,
	camera,
	diffuse,
	translate,
	scale,
	rotate,
	sphere,
	polyline,
	cube,
	mesh
};

int Keys_Get_id(FILE * file)
{
	char	func_name[30];

	//
	// get function name
	//
	glusFileScanf(file, "%s", func_name, _countof(func_name));
	if (feof(file))	// add [9/4/2016 blue],fix for file end
		return -1;

	strlower(func_name);

	//
	// find the function id
	//
	for (int i = 0; i < Keys_n;i++)
	{
		if (strcmp(Keys[i], func_name) == 0)
		{
			return i;
		}
	}
	return _Key_Unknown;

}
GLsizei Keys_Get_param(FILE *file, int id,pGLdouble param)
{
	GLsizei i=0;
	GLsizei n = Keys_func_param[id];
	
	for (; i < n && !feof(file); i++)
	{
		ubyte	u;
		glusFileScanfex(file,u, "%lf", param + i);
		if (u==0)
			break;

		if (feof(file)) // add [9/4/2016 blue],fix for file end
			break;
	}

	return i;
}

PGlusScene
glusSceneLoad(
_In_	str	_fileName)
{
	assert(_fileName);
	

	//
	// load default data
	//
	PGlusScene scene = glusSceneDefault();

	//
	// open file
	//
	FILE * file;
	fopen_s(&file, _fileName, "r");
	if (!file)
		return scene;

	GLdouble	func_param[30];
	while (!feof(file))
	{
		int id = Keys_Get_id(file);
		if (id == _Key_Unknown)
			continue;

		int n = Keys_Get_param(file, id,func_param);
		Keys_func[id](scene,func_param,n,file);
	}

	fclose(file);

	return scene;
}

void
glusSceneUnload(
_In_	PGlusScene	_scene)
{
	assert(_scene);

	//
	// clear shapes
	//
	while (!glusLinkIsEmpty(&_scene->Shapes))
	{
		PGlusShapes p = (PGlusShapes)glusLinkRemoveHead(&_scene->Shapes); // get the shape 
		
		if (p->Shape.Extern && p->Shape.Clear)	// is an extern data and a function for clear
			p->Shape.Clear(p->Shape.Extern);	// clear by himself

		glusFree(p);		// clear shape
	}

	//
	// clear light
	//
	glusLinkClear(&_scene->Lights);

	glusFree(_scene); // clear scene
}

//
// set the default scene data
//		black background
//
PGlusScene			// change from void to PGlusScene [7/9/2016 tld]
glusSceneDefault()
{
	PGlusScene	scene = (PGlusScene)malloc(sizeof(GlusScene));
	if (!scene)
		return NULL;

	//
	// clear all data
	//
	ZeroMemory(scene, sizeof(GlusScene));

	//
	// set the projection
	//
	scene->Projection.IsPerspective = false;	// use ortho
	scene->Projection.Ortho.Left = -3, scene->Projection.Ortho.Right = 3;
	scene->Projection.Ortho.Bottom = -2, scene->Projection.Ortho.Top = 2;
	scene->Projection.Near = 0.1, scene->Projection.Far = 100;

	//
	// set the camera
	//
	glusCameraSet(2, 3, 1,  0, 0, 0,  0, 0, 1,  &scene->Camera);


	//
	// init the shapes
	//
	glusLinkInit(&scene->Shapes);

	//
	// init the lights
	//
	glusLinkInit(&scene->Lights);

	//
	// enable axis
	//
	scene->EnableAxis = true;
	scene->AxisLength = 1;

	
	//
	// the return the default scene
	//
	return scene;
}

// add [7/9/2016 tld]
//
// draw a scene
//
void
glusSceneDraw(
_In_	PGlusScene	_scene)
{
	assert(_scene);

	//
	// set the projection
	//
	glusProjection(&_scene->Projection);

	//
	// set the camera
	//	
	glusCamera(&_scene->Camera);

	//
	// the background
	//
	glClearColor((GLclampf)_scene->Background.R, (GLclampf)_scene->Background.G, (GLclampf)_scene->Background.B, (GLclampf)_scene->Background.A);


	
	//
	// draw axis
	//
	if (_scene->EnableAxis)
	{
		glusAxis3D(_scene->AxisLength);
	}

	

	//
	// draw the shapes
	//
	PGlusShapes p = (PGlusShapes)_scene->Shapes.BLink;
	while (!glusLinkIsHead(p,&_scene->Shapes))
	{
		//
		// we just call the function already setting 
		// 
		
		// convert pointer to shape
		PGlusShape s = (PGlusShape)glusLinkData(p);
		
		glusPushCT();

		

		//
		// transform
		//
		glColor4dv((GLdouble*)&s->Diffuse);	// diffuse color
		glusTranslatev(&s->Transform);		// translate
		glusScalev(&s->Transform);			// scale
		glusRotatev(&s->Transform);			// rotate

		s->Draw(s->Extern);					// draw it

		glusPopCT();
		p = (PGlusShapes)p->Link.BLink;


		// use gray as default color
		glColor3d(0.45, 0.45, 0.45);
	}
}

//
// set light
//
// add [7/9/2016 tld]
void
glusSceneLight(
_In_	PGlusScene	_scene)
{
	assert(_scene);

	PGlusLink	p = _scene->Lights.BLink;
	
	//
	// is require use light
	//
	if (glusLinkIsEmpty(p))
		return;

	glEnable(GL_LIGHTING);	// enable light
	glShadeModel(GL_SMOOTH);// shade model
	glEnable(GL_DEPTH_TEST);// enabLe depth test
	glEnable(GL_NORMALIZE); // enable normalize vector

	while (!glusLinkIsEmpty(p))
	{
		PGlusLights l = (PGlusLights)p->BLink;
		glEnable(l->Light.Type);
		glLightfv(l->Light.Type, GL_POSITION, (GLfloat*)&l->Light.Position);
		glLightfv(l->Light.Type, GL_DIFFUSE, (GLfloat*)&l->Light.Diffuse);

		p = p->BLink;
	}
}

/*
 *	add a new shape to scene
 *	the shape has the default setting
 *	return the pointer to shape so you can custom it
 */
// create [9/1/2016 blue]
PGlusShape	
glusSceneCreateNewShape(
_In_	PGlusScene	_scene)
{
	assert(_scene);

	/*
	 *	allocate memory for a new shape
	 */
	PGlusShapes	shape; 
	glusAllocex(shape, GlusShapes,1,return NULL);

	// set to default
	glusShapeDefault(&shape->Shape);
	
	// and insert to scene
	glusLinkInsertTail(&_scene->Shapes, shape);

	// return the pointer
	return &shape->Shape;
}