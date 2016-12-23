#include "../inc/glus.h"
#include "sdl-core_shape.c"
#include "sdl-core_def-use.c"

/*
//  [8/19/2016 xcv]
add glusSceneLoad	-	glusSceneUnload
add glusSceneDefault
*/
#define	Key_Unknown -1

#define Keys_count 23

str Keys[Keys_count] =
{
	"inc",
	"log",
	"background",
	"projection",
	"camera",
	"diffuse",
	"translate",
	"scale",
	"rotate",
	"polyline",
	"mesh",
	"shadelevel",
	"globalambient",
	"ambient",
	"specular",
	"islight",
	"light",
	"texture",
	"textureid",
	"shape",
	"def",
	"use",
	"peano"
};
GLsizei Keys_func_param[Keys_count] =
{
	0,	// inc			: ( s_file-name... )
	1,	// log			: i_mode 
	3,	// background	: v_color
	9,	// projection	: i_angle f_antio	f_near f_far
	9,	// camera		: v_postion v_target v_up
	4,	// diffuse		: v_color
	3,	// translate	: v_t
	3,	// scale		: v_s
	4,	// rotate		: v_r
	0,	// polyline		: n_point v_p ... 
	0,	// mesh			: n_p n_n n_t n_f \n v_p ... \n v_n ... \n v_t ... \n v_f ... 
	1,	// shade level	: i_level
	4,	// global ambient	: v_c
	4,	// ambient		: v_c
	4,	// specular		: v_c
	1,	// is light		: b_l
	8,	// light		: i_id v_p v_c
	0,	// texture		: i_id	s_name/s_file_name
	1,	// texture id	: i_id
	0,	// shape		: s_name [ n_param ... ]
	0,  // def			: s_name { ... }
	0,	// use			: s_name
	0,	// peano curve	: (...) i_level
};

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
		Check(file_new, glusLogex(Glus_Log_Error, "Error: ( %s ) file no found.\n", s_newfile), return);

		glusLog("[sd] inc new file %s.\n", s_newfile);

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
	PGlusShape s = glusSceneGetLastShape(_scene);

	//
	// then set diffuse
	//
	s->Diffuse.R = param[0];
	s->Diffuse.G = param[1];
	s->Diffuse.B = param[2];
	s->Diffuse.A = (n_param == 3) ? 1.0 : param[3];
}
/*
*	ambient
*/
void ambient(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	//
	// get the current shape
	//
	PGlusShape s = glusSceneGetLastShape(_scene);

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
	PGlusShape s = glusSceneGetLastShape(_scene);

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
	PGlusShape s = glusSceneGetLastShape(_scene);

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
	PGlusShape s = glusSceneGetLastShape(_scene);

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
	PGlusShape s = glusSceneGetLastShape(_scene);


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
	PGlusLights p_light; glusAllocex(p_light, GlusLights, 1, return);
	glusLinkInsertTail(&_scene->Lights, p_light);

	p_light->Light.Type = GL_LIGHT0 + (int)param[0];

	p_light->Light.Position.X = param[1];
	p_light->Light.Position.Y = param[2];
	p_light->Light.Position.Z = param[3];

	p_light->Light.Diffuse.R = param[4];
	p_light->Light.Diffuse.G = param[5];
	p_light->Light.Diffuse.B = param[6];
	p_light->Light.Diffuse.A = param[7];

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
	PGlusShape s = glusSceneGetLastShape(_scene);
	PGlusMesh  mesh = s->Extern;
	mesh->TextureID = param[0];

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



void(*Keys_func[Keys_count])(PGlusScene, pGLdouble, GLsizei, FILE*) =
{
	inc,
	loglevel,
	background,
	projection,
	camera,
	diffuse,
	translate,
	scale,
	rotate,
	polyline,
	mesh,
	shadelevel,
	globalambient,
	ambient,
	specular,
	islight,
	light,
	texture,
	textureid,
	shape,
	def,
	use,
	peano
};

int Keys_Get_id(FILE * file)
{
	char	func_name[30];
	int		n_get;
	//
	// get function name
	//
	n_get = glusScanf(file, "%s", func_name, _countof(func_name));
	if (!n_get)	return-1;
	if (feof(file))	// add [9/4/2016 blue],fix for file end
		return -1;

	strlower(func_name);

	//
	// find the function id
	//
	for (int i = 0; i < Keys_count; i++)
	{
		if (strcmp(Keys[i], func_name) == 0)
		{
			return i;
		}
	}

	/*
	 *	key no found
	 */
	if(func_name[0]) glusLogex(Glus_Log_Error,"Error: ( %s ) key no found.\n", func_name);
	return Key_Unknown;

}

GLsizei Keys_Get_param(FILE *file, int id, pGLdouble param)
{
	GLsizei i = 0;
	GLsizei n = Keys_func_param[id];

	for (; i < n && !feof(file); i++)
	{
		ubyte	u=glusScanf(file, "%lf", param + i);
		if (u == 0)
			break;

		if (feof(file))				// add [9/4/2016 blue],fix for file end
			continue;
	}

	return i;
}

void
glusSDLex(
_Inout_ PGlusScene	_scene,
_In_	FILE*		_file)
{
	assert(_scene &&_file);
	

	GLdouble	func_param[30];	// max parameters

	while (!feof(_file))
	{
		/*
		*	get key id
		*/
		int id = Keys_Get_id(_file);
		if (id == Key_Unknown)
			continue;

		int n = Keys_Get_param(_file, id, func_param);	// get parameters

		Keys_func[id](_scene, func_param, n, _file);	// resolve the key
	}
}

void
glusSDL(
_Inout_ PGlusScene	_scene,
_In_	FILE*		_file)
{
	assert(_scene &&_file);

	/*
	*	skip comment
	*	except stdin
	*/
	fskipcomment(&_file, "//", "/*", "*/");

	glusSDLex(_scene, _file);
	
}

void 
glusSDLClear()
{
	def_clear();
}

