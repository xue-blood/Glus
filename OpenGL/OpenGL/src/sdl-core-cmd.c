#include "../inc/glus.h"


void glusSDLDefaultClear(pvoid p)
{
	glusFree(p);
}



//
// inc new file
//
void inc(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	/*
	*	get new file name
	*/
	char s_newfile[100] = { 0 };

	/*
	*	start resolve
	*/
	glusScanf(file, "%*[^(](");

	while (true)
	{
		/*
		*	is resolve end
		*/
		glusSkipSpace(file);
		char c_s = getc(file);
		if (c_s == ')')	return;
		else			ungetc(c_s, file);

		glusScanf(file, "%s.sdl", s_newfile, _countof(s_newfile));
		if (!s_newfile[0]) return;	// no file input now

		/*
		*	open file
		*/
		FILE * file_new;
		fopen_s(&file_new, s_newfile, "r");
		Check(file_new, glusLogex(Glus_Log_Error, "\nError: ( %s ) file no found.", s_newfile), return);

		glusLog("\n[sdl] inc new file %s.", s_newfile);

		glusSDL(_scene, file_new);	// resolve the file with current scene

		fclose(file_new);
	}
}

//
// key function
//
void loglevel(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	if (!n_param)	return;
	glusLogLevel(param[0]);
}
//
// key function
//
void background(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	if (n_param == 3)
		_scene->Background.A = 1;
	_scene->Background.R = param[0];
	_scene->Background.G = param[1];
	_scene->Background.B = param[2];
}




// add support of perspective [9/24/2016 blue]
void projection(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{

	switch (n_param)
	{
	case 4:
		glusPerspective(param[0], param[1], param[2], param[3], &_scene->Projection);
		break;

	case 6:
		glusOrtho(param[0], param[1], param[2], param[3], param[4], param[5], &_scene->Projection);
		break;

	case 9:
		glusOrtho(param[0], param[1], param[2], param[3], param[4], param[5], &_scene->Projection);
		glusOblique(param[6], param[7], param[8], &_scene->Projection);
		break;
	default:
		break;
	}

}
void camera(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	glusCameraSet(param[0], param[1], param[2],
		param[3], param[4], param[5],
		param[6], param[7], param[8],
		&_scene->Camera);

}
/*
*	diffuse
*/
void diffuse(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	//
	// get the current shape
	//
	PGlusShape s = _scene->SelectShape;
	if (!s) return;

	//
	// then set diffuse
	//
	s->Diffuse.R = param[0];
	s->Diffuse.G = param[1];
	s->Diffuse.B = param[2];
	s->Diffuse.A = (n_param == 3) ? 1.0 : param[3];
}

/*
*	shininess
*/
void shininess(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	if (n_param != 1) return;
	//
	// get the current shape
	//
	PGlusShape s = _scene->SelectShape;
	if (!s) return;

	//
	// then set shininess
	//
	s->Shininess = param[0];
}
/*
*	transparency
*/
void transp(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	if (n_param != 1) return;
	//
	// get the current shape
	//
	PGlusShape s = _scene->SelectShape;
	if (!s) return;

	//
	// then set shininess
	//
	s->Transparency = param[0];
}
/*
*	ambient
*/
void ambient(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	//
	// get the current shape
	//
	PGlusShape s = _scene->SelectShape;
	if (!s) return;

	//
	// then set diffuse
	//
	s->Ambient.R = param[0];
	s->Ambient.G = param[1];
	s->Ambient.B = param[2];
	s->Ambient.A = (n_param == 3) ? 1.0 : param[3];
}

/*
*	specular for material
*/
void specular(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	//
	// get the current shape
	//
	PGlusShape s = _scene->SelectShape;
	if (!s) return;
	//
	// then set diffuse
	//
	s->Specular.R = param[0];
	s->Specular.G = param[1];
	s->Specular.B = param[2];
	s->Specular.A = (n_param == 3) ? 1.0 : param[3];
}
void translate(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	//
	// get the current shape
	//
	PGlusShape s = _scene->SelectShape;
	if (!s) return;
	//
	// set translation
	//
	s->Transform.Dx = param[0];
	s->Transform.Dy = param[1];
	s->Transform.Dz = param[2];
}
void scale(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	//
	// get the current shape
	//
	PGlusShape s = _scene->SelectShape;
	if (!s) return;
	//
	// set scale
	//
	s->Transform.Sx = param[0];
	s->Transform.Sy = param[1];
	s->Transform.Sz = param[2];
}
void rotate(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	//
	// get the current shape
	//
	PGlusShape s = _scene->SelectShape;
	if (!s) return;

	//
	// set scale
	//
	s->Transform.Angle = param[0];
	s->Transform.Ax = param[1];
	s->Transform.Ay = param[2];
	s->Transform.Az = param[3];
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
	glusScanf(file, "%d", &n);

	// get points' data
	glusPointsLoad_L(file, h, n);

	return;


_polyline_failed_:
	if (h)	glusLinkClear(h); glusFree(h);
	if (p)	glusFree(p);
}


/*
*	add support for mesh
*/
// add [8/31/2016 blue]
void mesh(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	/*
	*	load a mesh from file
	*/
	PGlusMesh	p_mesh;
	if (!glusSuccess(glusMeshLoad(file, &p_mesh)))
		return;

	// add to scene
	if (!glusSuccess(glusMeshAddToScene(p_mesh, _scene)))
		return;
}

/*
*	shade level
*/
void shadelevel(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	glusSetShadeLevel((Glusenum)param[0]);
}
/*
*	global ambient
*/
void globalambient(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	_scene->GlobalAmbient.R = param[0];
	_scene->GlobalAmbient.G = param[1];
	_scene->GlobalAmbient.B = param[2];
	_scene->GlobalAmbient.A = (n_param == 3) ? 1 : param[3];
}


/*
*	is enable light
*/
void islight(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	_scene->IsLight = param[0];
}
/*
*	light
*/
void light(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	if (n_param != 9) 
	{
		glusLogex(Glus_Log_Error, "light format error.\n"); return;
	}

	// is a valid light
	int type = (int)param[0];
	if (type < 0 || type >= 8)
	{
		glusLogex(Glus_Log_Error, "Error: light must in 0 - 7 \n");
		return;
	}

	PGlusLights p_light = NULL; 
	
	// is light already define
	PGlusLights l = (PGlusLights)_scene->Lights.BLink;
	while (!glusLinkIsHead(l, &_scene->Lights))
	{
		if (l->Light.Type == GL_LIGHT0 + (int)param[0])
		{
			// find it
			p_light = l;
			break;
		}

		l = (PGlusLights)l->Link.BLink;
	}

	// not found,create a new light 
	if (!p_light)
	{
		glusAllocex(p_light, GlusLights, 1, return);
		glusLinkInsertTail(&_scene->Lights, p_light);
	}


	p_light->Light.Type = GL_LIGHT0 + (int)param[0];

	// need use float ,so we use color for vector
	p_light->Light.Position.R = param[1];
	p_light->Light.Position.G = param[2];
	p_light->Light.Position.B = param[3];
	p_light->Light.Position.A = param[4];

	p_light->Light.Diffuse.R = param[5];
	p_light->Light.Diffuse.G = param[6];
	p_light->Light.Diffuse.B = param[7];
	p_light->Light.Diffuse.A = param[8];

}

/*
*	load texture
*/
void texture(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	glusTextureLoad(file, _scene);
}

/*
*	set texture for mesh
*/
void textureid(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	//
	// get the current shape
	//
	PGlusShape s = _scene->SelectShape;
	if (!s) return;
	PGlusMesh  mesh = s->Extern;
	mesh->TextureID = param[0];

}

/*
*	set a name for shape
*/
void name(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	//
	// get the name
	//
	PGlusShape s = _scene->SelectShape;
	if (!s) return;

	glusScanf(file, "%s", s->Name, _countof(s->Name));
	glusLog("\rCreate a shape name:%s", s->Name);
}

/*
*	show or no current shape
*/
void hide(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	//
	// get the name
	//
	PGlusShape s = _scene->SelectShape;
	if (!s) return;

	s->IsHide = true;
}

/*
*	peano curve
*/
void peano(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	//
	// create a shape
	//
	PGlusShape p = glusSceneCreateNewShape(_scene);
	p->Draw = glusPeanoDraw;
	p->Clear = glusSDLDefaultClear;	// use default clear

	/*
	*	set curve
	*/
	glusAllocex(p->Extern, Peano, 1, return);

	glusPeanoLoad(file, p->Extern); // load the peano curve

}

/*
*	array
*/
void array(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	/*
	*	get the target by name
	*/
	char name[64];
	glusScanf(file, "%s", name, _countof(name));
	PGlusShape t = glusSceneGetShapeByName(_scene, name, _countof(name));
	if (!t)	{ glusLogex(Glus_Log_Error, "Error: ( %s ) name not found.\n", name); return; }

	//
	// create a shape
	//
	PGlusShape p = glusSceneCreateNewShape(_scene);
	p->Draw = glusArrayDraw;
	p->Clear = glusSDLDefaultClear;	// use default clear


	/*
	*	set curve
	*/
	glusAllocex(p->Extern, Array, 1, return);
	glusArrayLoad(file, p->Extern, t);
}


/*
*	chaos game
*/
void chaos(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	PChaosGame cas = glusChaosGameLoad(file);
	if (!cas)
	{
		glusLog("Choas game load failed.\n");
		return;
	}

	PGlusShape p = glusSceneCreateNewShape(_scene);
	p->Draw = glusChaosGame;
	p->Clear = glusSDLDefaultClear;	// use default clear
	p->Extern = cas;		// the data for chaos game
}

/*
*	boolean object
*/
void boolean(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	PGlusBool	b = glusBoolLoad(file, _scene);
	if (!b)
	{
		glusLog("Boolean load failed.\n");
		return;
	}

	PGlusShape p = glusSceneCreateNewShape(_scene);
	p->Draw = glusBoolDraw;
	p->Clear = glusBoolClear;	// use default clear
	p->Hit = glusBoolHit;
	p->Extern = b;		// the data for chaos game

	p->Transform = b->A->Transform; // copy the tranform from A
}

void raytrace(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	if (n_param != 1) return;

	// set ray trace block size
	_scene->RaySize = (int)param[0];
	glusLog("\t ray trace size: %d", _scene->RaySize);
}


void fps(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	if (n_param != 1) return;

	// set fps
	glusFPS(param[0]);
}

void select(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	/*
	*	get the target by name
	*/
	char name[64];
	glusScanf(file, "%s", name, _countof(name));
	PGlusShape t = glusSceneGetShapeByName(_scene, name, _countof(name));
	if (!t)	{ glusLogex(Glus_Log_Error, "Error: ( %s ) name not found.\n", name); return; }

	glusLog("Select object: %s\n", name);

	// select this shape
	_scene->SelectShape = t;

}