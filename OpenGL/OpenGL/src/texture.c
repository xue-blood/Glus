#include "../inc/glus.h"

#define tex_checkboard	"CHECKBOARD"



/*
 *	load texture to scene
 */
// create [10/23/2016 blue]
bool
glusTextureLoad(
_In_	FILE*		file,
_Inout_	PGlusScene	_scene)
{
	assert(file && _scene);

	glusAlloc(tex, GlusTexture);

	glusScanf(file, "%d", &tex->ID);	// get texture id

	/*
	*	load pixmap for texture
	*/

	/*
	*	load name
	*/
	char name[50];
	glusScanf(file, "%s", name, _countof(name));

	name[49] = 0;
	if (strequ(name, tex_checkboard)) // use check-board direct
	{
		tex->Pixmap = pixCheckboard();
	}

	glusLinkInsertTail(&_scene->Textures, tex); // add texture to link-list
	pixSetTexture(tex->Pixmap, tex->ID);

	return true;

	goto _texture_failed;

_texture_failed:
	if (tex->Pixmap->Pixels)	glusFree(tex->Pixmap->Pixels);
	if (tex->Pixmap)			glusFree(tex->Pixmap);
	return	-1;
}




/*
*	load texture id from file
*/
// create [10/23/2016 blue]
bool
glusTextureIDLoad(
_In_	FILE *		_file,
_Inout_	PGlusMesh	_mesh)
{
	assert(_file && _mesh);

	/*
	 *	get the texture coordinates,
	 *	we just use the point-num
	 */
	glusAllocex(_mesh->Textures, GlusVector, _mesh->PointNum, return false);
	for (Glusnum i = 0; i < _mesh->PointNum;i++)
	{
		glusScanf(_file, "(%lf,%lf)", &_mesh->Textures[i].X, &_mesh->Textures[i].Y);
	}

	return true;
}