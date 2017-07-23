#include "../inc/glus.h"

#include "sdl-core-cmd.c"
#include "sdl-core_shape.c"
#include "sdl-core_def-use.c"

/*
//  [8/19/2016 xcv]
add glusSceneLoad	-	glusSceneUnload
add glusSceneDefault
*/
#define	Key_Unknown -1

#define Keys_count 33

str Keys[Keys_count] =
{
	"inc",
	"log",
	"background",
	"projection",
	"camera",
	"diffuse",
	"shininess",
	"transp",
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
	"name",
	"hide",
	"peano",
	"array",
	"chaos",
	"bool",
	"raytrace",
	"fps",
	"select",
};
GLsizei Keys_func_param[Keys_count] =
{
	0,	// inc			: ( s_file-name... )
	1,	// log			: i_mode 
	3,	// background	: v_color
	9,	// projection	: i_angle f_antio	f_near f_far
	9,	// camera		: v_postion v_target v_up
	4,	// diffuse		: v_color
	1,	// shininess	: f_shininess
	1,  // transparency : f_transparency
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
	0,	// name			: s_name
	0,	// hide			: none
	0,	// peano curve	: (...) i_level
	0,	// array		: s_target_name s_methon (n_x,n_y,n_z)<d_x,d_y,d_z> 
	0,	// chaos game	: (...)
	0,	// boolean		: (...)
	1,  // raytrace     : i_block_size
	1,  // fps			: i_fps
	0,	// select		: s_name
};




void(*Keys_func[Keys_count])(PGlusScene, pGLdouble, GLsizei, FILE*) =
{
	inc,
	loglevel,
	background,
	projection,
	camera,
	diffuse,
	shininess,
	transp,
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
	name,
	hide,
	peano,
	array,
	chaos,
	boolean,
	raytrace,
	fps,
	select,
};

int Keys_Get_id(FILE * file)
{
	char	func_name[30];
	int		n_get;
	//
	// get function name
	//
	n_get = glusScanf(file, "%s", func_name, _countof(func_name));
	if (!n_get)	return -1;
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

	for (; !feof(_file); glusSkipSpace(_file))
	{
		/*
		*	get key id
		*/
		int id = Keys_Get_id(_file);
		if (id == Key_Unknown)
			continue;

		glusLog("\n[sdl] loading %s", Keys[id]);

		int n = Keys_Get_param(_file, id, func_param);	// get parameters

		Keys_func[id](_scene, func_param, n, _file);	// resolve the key

		glusLog("\t\t successed.");
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

