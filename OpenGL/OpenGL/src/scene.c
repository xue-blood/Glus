#include "..\inc\glus.h"
#include <strsafe.h>



PGlusScene
glusSceneNew(
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
	if (!file)		return scene;

	glusSDL(scene, file);

	fclose(file);
	return scene;
}

void
glusSceneLoad(
_In_	PGlusScene	_scene,
_In_	str			_file_name)
{
	assert(_file_name);

	FILE * file;
	fopen_s(&file, _file_name, "r");
	if (!file)		return;

	glusSDL(_scene, file);
	fclose(file);
}

void
glusSceneClear(
_In_	PGlusScene	_scene)
{
	assert(_scene);

	// clear sdl
	glusSDLClear();

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

	/*
	 *	clear texture
	 */
	while (!glusLinkIsEmpty(&_scene->Textures))
	{
		PGlusTexture p = (PGlusTexture)glusLinkRemoveHead(&_scene->Textures);

		glusFree(p->Pixmap->Pixels);
		glusFree(p->Pixmap);
	}
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
	glusOrtho(-3, 3, -2, 2, .1, 100, &scene->Projection);

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

	/*
	 *	init the textures
	 */
	glusLinkInit(&scene->Textures);

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

		glColor4fv((GLfloat*)&s->Diffuse);	// no material
		
		if (glusGetShadeLevel()!= Glus_Shade_Wire)
		{
			/*
			*	set material
			*/
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat*)&s->Diffuse);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat*)&s->Specular);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat*)&s->Ambient);

			glEnable(GL_LIGHTING);
		}

		//
		// transform
		//
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

	/*
	 *	global ambient
	 */
	if (_scene->GlobalAmbient.A <= 0)
		return;
	glEnable(GL_LIGHTING);	// enable light
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (GLfloat*)&_scene->GlobalAmbient);

	//
	// is require use light
	//
	if (glusLinkIsEmpty(&_scene->Lights))
		return;

	/*
	 *	other light
	 */
	glEnable(GL_DEPTH_TEST);// enabLe depth test
	glEnable(GL_NORMALIZE); // enable normalize vector
	
	PGlusLights l = (PGlusLights)_scene->Lights.BLink;
	while (!glusLinkIsHead(l,&_scene->Lights))
	{
		glEnable(l->Light.Type);
		glLightfv(l->Light.Type, GL_POSITION, (GLfloat*)&l->Light.Position);
		glLightfv(l->Light.Type, GL_DIFFUSE, (GLfloat*)&l->Light.Diffuse);

		l = (PGlusLights)l->Link.BLink;
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