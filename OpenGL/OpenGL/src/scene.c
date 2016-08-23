#include "..\inc\glus.h"
#include <strsafe.h>

/*
//  [8/19/2016 xcv]
	add glusSceneLoad	-	glusSceneUnload
	add glusSceneDefault
*/
#define	_Key_Unknown -1
#define Keys_n 12
str Keys[Keys_n] =
{
	"//",
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
	"cube"
};
GLsizei Keys_func_param[Keys_n] = 
{
	0,
	3,
	1,
	6,
	9,
	4,
	3,
	3,
	4,
	0,
	1,
	0
};

//
// key function
//
void comment(PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
{
	char c = 0;

	while (c != '\n')
	{
		c = fgetc(file);
		if (c==EOF)
			break;
	}
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
void projection(PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
{
	_scene->Projection.Left = param[0];
	_scene->Projection.Right = param[1];
	_scene->Projection.Bottom = param[2];
	_scene->Projection.Top = param[3];
	_scene->Projection.Near = param[4];
	_scene->Projection.Far = param[5];
}
void camera(	PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
{
	_scene->Camera.EyeX = param[0];
	_scene->Camera.EyeY = param[1];
	_scene->Camera.EyeZ = param[2];

	_scene->Camera.CenterX = param[3];
	_scene->Camera.CenterY = param[4];
	_scene->Camera.CenterZ = param[5];

	_scene->Camera.UpX = param[6];
	_scene->Camera.UpY = param[7];
	_scene->Camera.UpZ = param[8];

}
void diffuse(	PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
{
	//
	// get the current shape
	//
	PGlusShape s = (PGlusShape)glusLinkData(_scene->Shapes.FLink);
	
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
	PGlusShape s = (PGlusShape)glusLinkData(_scene->Shapes.FLink);
	
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
	PGlusShape s = (PGlusShape)glusLinkData(_scene->Shapes.FLink);

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
	PGlusShape s = (PGlusShape)glusLinkData(_scene->Shapes.FLink);


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
	PGlusShapes p = (PGlusShapes)malloc(sizeof(GlusShapes));
	glusShapeDefault(&p->Shape);

	p->Shape.Draw = glusSphere;

	glusLinkInsertTail(&_scene->Shapes, (PGlusLink)p);	// insert to tail
}
void polyline(PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
{
	//
	// create a shape
	//
	PGlusShapes p = (PGlusShapes)malloc(sizeof(GlusShapes));
	glusShapeDefault(&p->Shape);

	p->Shape.Draw = glusDrawPolyLine;

	glusLinkInsertTail(&_scene->Shapes, (PGlusLink)p);	// insert to tail

	//
	// param[0] is the count of points
	//
	size n = (size)param[0];

	//
	// malloc memory for points
	//
	PGlusLink h = (PGlusLink)malloc(sizeof(GlusLink));
	if (!h)
		goto _polyline_failed_;
	else
		p->Shape.Extern = h;
	glusLinkInit(h);

	char org[30];

	for (size i = 0; i < n;i++)
	{
		PGlusPoints ps = (PGlusPoints)malloc(sizeof(GlusPoints));
		if (!ps)
			goto _polyline_failed_;

		//
		// read the point
		//
		glusFileRead(file, org, _countof(org));
		sscanf_s(org, "(%lf,%lf,%lf)", &ps->Point.X, &ps->Point.Y, &ps->Point.Z);
		ps->Point.V = 1;

		glusLinkInsertTail(h, ps);
	}
	
	return;


_polyline_failed_:
	if (h)	glusLinkClear(h), glusFree(h);
	if (p)	glusFree(p);
}
void cube(PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
{
	PGlusShapes p = (PGlusShapes)malloc(sizeof(GlusShapes));
	glusShapeDefault(&p->Shape);

	p->Shape.Draw = glusCube;

	glusLinkInsertTail(&_scene->Shapes, (PGlusLink)p);	// insert to tail
}
void(*Keys_func[Keys_n])(PGlusScene, pGLdouble, GLsizei,FILE*) =
{
	comment,
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
	cube
};

int Keys_Get_id(FILE * file)
{
	char	func_name[30];

	//
	// get function name
	//
	glusFileRead(file, func_name, _countof(func_name));
	//sscanf_s("%s", func_name, _countof(func_name));
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
	char org[30];
	
	for (; i < n;i++)
	{
		glusFileRead(file, org, _countof(org));

		if(sscanf_s(org, "%lf", param + i)== 0)
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
// 	// allocate memory for GlusScene
// 	//
// 	PGlusScene	scene = (PGlusScene)malloc(sizeof(GlusScene));
// 	if (!scene)
// 		return NULL;

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
		
		if (p->Shape.Extern)	// is an extern data
		{
			glusLinkClear(p->Shape.Extern);
			glusFree(p->Shape.Extern);
		}
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
	//scene->Projection.Bottom = scene->Projection.Left = scene->Projection.Near = -10;
	//scene->Projection.Top = scene->Projection.Right= scene->Projection.Far= 10;
	scene->Projection.Left = -3, scene->Projection.Right = 3;
	scene->Projection.Bottom = -2, scene->Projection.Top = 2;
	scene->Projection.Near = 0.1, scene->Projection.Far = 100;

	//
	// set the camera
	//
	scene->Camera.EyeX = scene->Camera.EyeY = scene->Camera.EyeZ = 2;
	// center is already zero now
	scene->Camera.UpY = 1; // up is (0,1,0)

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
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(_scene->Projection.Left, 
		_scene->Projection.Right, 
		_scene->Projection.Bottom, 
		_scene->Projection.Top, 
		_scene->Projection.Near, 
		_scene->Projection.Far);

	//
	// set the camera
	//	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(_scene->Camera.EyeX,
		_scene->Camera.EyeY,
		_scene->Camera.EyeZ,
		_scene->Camera.CenterX,
		_scene->Camera.CenterY,
		_scene->Camera.CenterZ,
		_scene->Camera.UpX,
		_scene->Camera.UpY,
		_scene->Camera.UpZ);

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
	while (!glusLinkIsEnd(p,&_scene->Shapes))
	{
		//
		// we just call the function already setting 
		// 
		
		// convert pointer to shape
		PGlusShape s = (PGlusShape)glusLinkData(p);
		
		glusPushCT();

		glColor4dv((GLdouble*)&s->Diffuse);	// diffuse color
		glusTranslatev(&s->Transform);		// translate
		glusScalev(&s->Transform);			// scale
		glusRotatev(&s->Transform);			// rotate

		s->Draw(s->Extern);					// draw it

		glusPopCT();
		p = (PGlusShapes)p->Link.BLink;
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