#include "../inc/glus.h"

#define Def_Name_Length 12
typedef struct _Def
{
	GlusLink	Link;
	char		Name[Def_Name_Length];
	FILE*		File;
}Def,*PDef;

static glusLink(_Lk_def);

void def(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	PDef p; glusAllocex(p, Def,1,return);
	/*
	 *	get the def name
	 */
	glusFileScanf(file, "%s", p->Name, Def_Name_Length);

	/*
	 *	find the start
	 */
	fscanf_s(file, "%*[^{]{");	
	int		i_start = ftell(file);	 

	/*
	 *	find the end
	 */
	fscanf_s(file, "%*[^}]}");
	int		i_end = ftell(file);

	/*
	 *	now we can store the def object
	 */
	int len = i_end - i_start + 1;
	str	as_buf; glusAllocex(as_buf, char, len, goto _def_failed_);
	
	fseek(file, i_start, SEEK_SET);	 // goto the start

	fread_s(as_buf, len, sizeof(char), len - 1, file);
	as_buf[len - 2] = 0;

	p->File= fmemopen(as_buf, len, "r");	// store the object to file

	glusFree(as_buf);	// free current buff

	glusLinkInsertTail(&_Lk_def, p);	// insert to link list
	return;

_def_failed_:
	glusLog("Fail: ( %s ) can't define.\n", p->Name);
	if (p)		glusFree(p);
	if (as_buf) glusFree(as_buf);
	return;
}

void use(PGlusScene _scene, pGLdouble param, GLsizei n_param, FILE *file)
{
	/*
	 *	get use name
	 */
	char	name[Def_Name_Length];
	glusFileScanf(file, "%s", name, Def_Name_Length);
	if (!name[0])	return;


	PGlusLink	p = _Lk_def.BLink;

	while (!glusLinkIsHead(p, &_Lk_def))
	{
		PDef d = (PDef)p;
		/*
		 *	is name defined 
		 */
		if (strequ(d->Name, name))
		{
			glusSDL(_scene, d->File);
			return;
		}

		p = p->BLink;
	}

	/*
	 *	name no defined
	 */
	glusLog("Error: ( %s ) name no defined.\n", name);
}

void def_clear()
{
	while (!glusLinkIsEmpty(&_Lk_def))
	{
		PDef d = (PDef)glusLinkRemoveTail(&_Lk_def);
		
		fclose(d->File);

		glusFree(d);
	}
}