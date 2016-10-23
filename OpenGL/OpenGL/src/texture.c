#include "../inc/glus.h"

#define tex_checkboard	"CHECKBOARD"



/*
 *	load texture to scene
 */
// create [10/23/2016 blue]
Glus_Status
glusTextureLoad(
_In_	FILE*		file,
_Inout_	PGlusScene	_scene)
{
	assert(file && _scene);

	PGlusTexture	tex; glusAlloc(tex, GlusTexture);

	glusFileScanf(file, "%d", &tex->ID);	// get texture id

	/*
	*	load pixmap for texture
	*/

	/*
	*	load name
	*/
	char name[50];
	glusFileScanf(file, "%s", name, _countof(name));

	if (strequ(name, tex_checkboard)) // use check-board direct
	{
		tex->Pixmap = rgbCheckboard();
	}

	glusLinkInsertTail(&_scene->Textures, tex); // add texture to link-list
	rgbSetTexture(tex->Pixmap, tex->ID);

	return Glus_Status_Success;

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
Glus_Status
glusTextureIDLoad(
_In_	FILE *		_file,
_Inout_	PGlusMesh	_mesh)
{
	assert(_file && _mesh);

	// get the texture id
	glusFileScanf(_file, "%d", &_mesh->TextureID);
	
	/*
	 *	get the texture coordinates,
	 *	we just use the point-num
	 */
	glusAllocex(_mesh->Textures, GlusVector, _mesh->PointNum, return Glus_Status_Memory_Allocate_Fail);
	for (Glusnum i = 0; i < _mesh->PointNum;i++)
	{
		glusFileScanf(_file, "(%lf,%lf)", &_mesh->Textures[i].X, &_mesh->Textures[i].Y);
	}

	return Glus_Status_Success;
}