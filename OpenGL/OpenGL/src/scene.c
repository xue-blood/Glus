#include "..\inc\glus.h"
#include <strsafe.h>
#include <time.h>


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
//	default enable: lighting,depth-test, smooth-shade
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

	/*
	*	light
	*/
	glusSceneLight(_scene);

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
	for (glusLinkFirst(p, &_scene->Shapes);
		!glusLinkIsHead(p, &_scene->Shapes);
		glusLinkNext(p))
	{	
		// convert pointer to shape
		PGlusShape s = glusLinkData(p);
		// is need show it
		if (s->IsHide) continue;
		
// 		if (glusGetShadeLevel() != Glus_Shade_Wire)
// 		{
// 			if (_scene->IsLight) glEnable(GL_LIGHTING);
// 			else				glDisable(GL_LIGHTING);
// 		}

		glusShapeDraw(s);
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

	//
	// is no light
	//
	if (glusLinkIsEmpty(&_scene->Lights))
	{
		//glusLightDefault(); return;
		PGlusLights l;
		glusAllocex(l, GlusLights, 1, return);
		glusLinkInsertTail(&_scene->Lights, l);
		glusLightGetDefault(&l->Light);
	}

	/*
	 *	global ambient
	 
	if (_scene->GlobalAmbient.A > 0)
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (GLfloat*)&_scene->GlobalAmbient);
		*/


	/*
	 *	set light
	*/ 
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);

	
	PGlusLights l = (PGlusLights)_scene->Lights.BLink;
	while (!glusLinkIsHead(l,&_scene->Lights))
	{
		glEnable(l->Light.Type);
	
		glLightfv(l->Light.Type, GL_DIFFUSE, (GLfloat*)&l->Light.Diffuse);
		glLightfv(l->Light.Type, GL_AMBIENT, (GLfloat*)&l->Light.Ambient);
		glLightfv(l->Light.Type, GL_SPECULAR, (GLfloat*)&l->Light.Specular);
		glLightfv(l->Light.Type, GL_POSITION, (GLfloat*)&l->Light.Position);
		


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

/*
 *	try get shape by name
 */
PGlusShape
glusSceneGetShapeByName(
_In_	PGlusScene		_scene,
_In_	str				_name)
{
	assert(_scene && _name);
	
	if (!_name[0]) return NULL;

	PGlusShapes p = (PGlusShapes)_scene->Shapes.BLink;
	while (!glusLinkIsHead(p,&_scene->Shapes))
	{
		if (strequ(_name, p->Shape.Name))
			return	&p->Shape; // find it

		p = (PGlusShapes)p->Link.BLink;
	}

	return NULL;
}

void	
glusSceneReshape(
_In_ PGlusScene	_scene, int w, int h)
{
	_scene->Projection.Persp.AspectRation = ((float)w) / h;
}

/*
 *	get the first hit object
 */
void
glusSceneHit(PGlusScene _scene, PGlusRay _ray,PGlusIntersect _best)
{
	assert(_scene && _ray && _best);

	GlusIntersect inter;
	_best->numHits = 0;
	GlusRay		ray;	// ray in generic coordinate

	for (glusLinkFirst(lk, &_scene->Shapes);
		!glusLinkIsHead(lk, &_scene->Shapes);
		glusLinkNext(lk))
	{
		PGlusShape shape = glusLinkData(lk);

		if (!shape->Hit)	continue;

		ray = *_ray;
		glusTransformInvVector(&shape->Transform, &ray.Point);
		glusTransformInvVector(&shape->Transform, &ray.Direction);
		if (!shape->Hit(shape,&ray, &inter)) continue;

		if (_best->numHits == 0 ||
			inter.Hits[0].hitTime < _best->Hits[0].hitTime)
			*_best = inter;	// set best
	}

}

bool isInShadow(PGlusRay ray,PGlusLink obj)
{
	assert(ray && obj);

	for (glusLinkFirst(lk, obj);
		!glusLinkIsHead(lk, obj);
		glusLinkNext(lk))
	{
		PGlusShape shape = glusLinkData(lk);

		if (!shape->Hit)	continue;

		/*
		 *	convent ray in generic coordinate
		 */
		GlusRay gen_ray = *ray;
		glusTransformInvVector(&shape->Transform, &gen_ray.Point);
		glusTransformInvVector(&shape->Transform, &gen_ray.Direction);
		
		if (shape->Hit(shape, &gen_ray, NULL)) return true;

	}

	return false;
}

/*
 *	shade with specify ray
 *	return false when color is background
 */
bool
glusSceneShade(PGlusScene _scene, PGlusRay _ray, PGlusColor _clr)
{
	assert(_scene && _ray && _clr);

	GlusIntersect	best; 
	// get the best hit
	glusSceneHit(_scene, _ray, &best);
	if (best.numHits == 0)
	{
		*_clr = _scene->Background; // if miss object
		return false;
	}

	PGlusShape  obj = best.HitObject;

	GlusVector	s, h, lp;
	GlusColor	diffuse = null, spec = null;
	real		lambert, phong;

	GlusRay		feeler;	// ray for compute shadow
	GlusVector	hit_point = best.Hits[0].HitPoint;
	glusTransformVector(&obj->Transform, &hit_point); // get the hit point int world coordinate
	glusAdd(&hit_point, 1, &_ray->Direction, 0.000001, &feeler.Point); // feeler point

	/*
	 *	set color
	 */
	for (glusLinkFirst(lk, &_scene->Lights);
		!glusLinkIsHead(lk, &_scene->Lights);
		glusLinkNext(lk))
	{
		
		PGlusLight l = glusLinkData(lk);

		/*
		* ambient
		*/
		rgbPro(&l->Ambient, 1, &obj->Ambient, 1, _clr);

		// is in shadow
		GlusVector l_pos;
		glusFtoDv((float*)&l->Position, (double*)&l_pos);
		glusAdd(&l_pos, 1, &hit_point, -1, &feeler.Direction);// feeler direction
		if (isInShadow(&feeler,&_scene->Shapes))	continue;

		/*
		*	emissive
		*/
		rgbAdd(_clr, &obj->Emissive);

		/*
		*	diffuse
		*/
		// vector from hit point to source
		glusFtoDv((float*)&l->Position, (double*)&lp);
		glusVFromPoint(&best.Hits[0].HitPoint, &lp, &s);
		glusNormalize(&s);

		// compute lambert
		lambert = glusDotPro(&s, &best.Hits[0].HitNormal);
		if (lambert > 0)	// hit point is turned toward the light
		{
			rgbPro(&l->Diffuse, lambert, &obj->Diffuse, 1, &diffuse);
			rgbAdd(_clr, &diffuse); // add diffuse part
		}

		/*
		*	specular,use phong model
		*/
		// halfway vector
		glusAdd(&s, 1, &_ray->Direction, -1, &h);
		glusNormalize(&h);

		phong = glusDotPro(&h, &best.Hits[0].HitNormal);
		if (phong > 0)
		{
			phong = pow(phong, obj->Shininess);
			rgbPro(&l->Specular, phong, &obj->Specular, 1, &spec);
			rgbAdd(_clr, &spec);
		}

	}

	return true;
}
// window size ,extern from canvas.c
extern int		_Window_Height, _Window_Width;

/*
 *	if block size == 0 ,disable ray trace
 */
void 
glusSceneRayTrace(PGlusScene _scene, int _block_size)
{
	if (_block_size <= 0)return;

	assert(_scene);

	
	glDisable(GL_LIGHTING);	// disable lighting

	int nRows = _Window_Height, nCols = _Window_Width;
	GlusRay		ray;		// ray
	GlusColor	clr;		// color 

	int old = glusLogLevel(Glus_Log_Warning);

	int y = glusWinTitleHeight();
	int x = glusWinBorderWidth();
	clock_t t = clock();
	glusLogex(Glus_Log_Important, "\nRay tarce is processing,please wait...");
	/*
	 *	exec ray trace
	 */
	for (int row = 0; row < nRows;row+=_block_size)
	{
		for (int col = 0; col < nCols; col += _block_size)
		{
			// set the ray
			glusCameraRay(col,nRows-row, &ray);

			// find color
			if(!glusSceneShade(_scene, &ray, &clr))
				continue;
			
			/*
			 *	draw block with current color
			 */
			glusUIEnter();
			glColor3d(clr.R, clr.G, clr.B);
			//glRecti(col, row, col + _block_size, row + _block_size);
			//glRecti(col - _block_size, row - _block_size - y, col, row - y);
			glRecti(col - x , row - y, col - x + _block_size, row - y + _block_size);
			glusUILeave();


		}
	}
	
	t = clock() - t;
	glusLogex(Glus_Log_Important, "use %.2f ms.", (float)t);
	glusLogLevel(old);

}