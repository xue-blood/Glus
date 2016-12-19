#include "../inc/glus.h"
#include "sdl-core_shape.c"

/*
//  [8/19/2016 xcv]
add glusSceneLoad	-	glusSceneUnload
add glusSceneDefault
*/
#define	_Key_Unknown -1

#define Keys_count 19

str Keys[Keys_count] =
{
	"inc",
	"debug",
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
	"light",
	"texture",
	"textureid",
	"shape"
};
GLsizei Keys_func_param[Keys_count] =
{
	0,	// inc new file
	1,	// debug
	3,	// background
	9,	// projection
	9,	// camera
	4,	// diffuse
	3,	// translate
	3,	// scale
	4,	// rotate
	0,	// polyline
	0,	// mesh
	1,	// shade level
	4,	// global ambient
	4,	// ambient
	4,	// specular
	9,	// light
	0,	// texture
	1,	// texture id
	0,	// shape
};

//
// inc new file
//
void inc(PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
{
	/*
	 *	get new file name
	 */
	char s_newfile[100] = { 0 };

	/*
	 *	start resolve
	 */
	glusFileSkipSpace(file);
	char c_s = getc(file);	if (c_s != '(')	return;

	while (true)
	{
		/*
		 *	is resolve end
		 */
		glusFileSkipSpace(file);
		c_s = getc(file);	
		if (c_s == ')')	return;
		else			ungetc(c_s, file);

		glusFileScanf(file, "%s.sdl", s_newfile, _countof(s_newfile));
		if (!s_newfile[0]) return;	// no file input now

		/*
		 *	open file
		 */
		FILE * file_new;
		fopen_s(&file_new, s_newfile, "r");
		Check(file_new, glusLog("Error: ( %s ) file no found.\n",s_newfile), return);

		glusSDL(_scene, file_new);	// resolve the file with current scene
	}
}

//
// key function
//
void debug(PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
{
	if (!param_n)	return;
	glusDebugEnable(param[0]);
}
//
// key function
//
void background(PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
{
	if (param_n == 3)
		_scene->Background.A = 1;
	_scene->Background.R = param[0];
	_scene->Background.G = param[1];
	_scene->Background.B = param[2];
}




// add support of perspective [9/24/2016 blue]
void projection(PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
{

	switch (param_n)
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
void camera(PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
{
	glusCameraSet(param[0], param[1], param[2],
		param[3], param[4], param[5],
		param[6], param[7], param[8],
		&_scene->Camera);

}
/*
*	diffuse
*/
void diffuse(PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
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
	s->Diffuse.A = (param_n == 3) ? 1.0 : param[3];
}
/*
*	ambient
*/
void ambient(PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
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
	s->Ambient.A = (param_n == 3) ? 1.0 : param[3];
}

/*
*	specular for material
*/
void specular(PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
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
	s->Specular.A = (param_n == 3) ? 1.0 : param[3];
}
void translate(PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
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
void scale(PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
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
	if (!glusSuccess(glusMeshAddToScene(p_mesh, _scene)))
		return;
}

/*
*	shade level
*/
void shadelevel(PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
{
	glusSetShadeLevel((Glusenum)param[0]);
}
/*
*	global ambient
*/
void globalambient(PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
{
	_scene->GlobalAmbient.R = param[0];
	_scene->GlobalAmbient.G = param[1];
	_scene->GlobalAmbient.B = param[2];
	_scene->GlobalAmbient.A = (param_n == 3) ? 1 : param[3];
}

/*
*	light
*/
void light(PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
{
	PGlusLights p_light; glusAllocex(p_light, GlusLights, 1, return);
	glusLinkInsertTail(&_scene->Lights, p_light);

	p_light->Light.Type = GL_LIGHT0 + (int)param[0];

	p_light->Light.Position.X = param[1];
	p_light->Light.Position.Y = param[2];
	p_light->Light.Position.Z = param[3];
	p_light->Light.Position.V = param[4];

	p_light->Light.Diffuse.R = param[5];
	p_light->Light.Diffuse.G = param[6];
	p_light->Light.Diffuse.B = param[7];
	p_light->Light.Diffuse.A = param[8];

}

/*
*	load texture
*/
void texture(PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
{
	glusTextureLoad(file, _scene);
}

/*
*	set texture for mesh
*/
void textureid(PGlusScene _scene, pGLdouble param, GLsizei param_n, FILE *file)
{
	//
	// get the current shape
	//
	PGlusShape s = glusSceneGetLastShape(_scene);
	PGlusMesh  mesh = s->Extern;
	mesh->TextureID = param[0];
}





void(*Keys_func[Keys_count])(PGlusScene, pGLdouble, GLsizei, FILE*) =
{
	inc,
	debug,
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
	light,
	texture,
	textureid,
	shape
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
	glusLog("Error: ( %s ) key no found.\n", func_name);
	return _Key_Unknown;

}

GLsizei Keys_Get_param(FILE *file, int id, pGLdouble param)
{
	GLsizei i = 0;
	GLsizei n = Keys_func_param[id];

	for (; i < n && !feof(file); i++)
	{
		ubyte	u;
		glusFileScanfex(file, u, "%lf", param + i);
		if (u == 0)
			break;

		if (feof(file))				// add [9/4/2016 blue],fix for file end
			break;
	}

	return i;
}


void
glusSDL(
_Inout_ PGlusScene	_scene,
_In_	FILE*		_file)
{

	/*
	*	 skip comment
	*/
	fskipcomment(&_file, "//", "/*", "*/");


	GLdouble	func_param[30];	// max parameters

	while (!feof(_file))
	{
		/*
		 *	get key id
		 */
		int id = Keys_Get_id(_file);
		if (id == _Key_Unknown)
			continue;

		int n = Keys_Get_param(_file, id, func_param);	// get parameters

		Keys_func[id](_scene, func_param, n, _file);	// resolve the key
	}

	fclose(_file);
}