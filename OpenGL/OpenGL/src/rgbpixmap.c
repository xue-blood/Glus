#include <stdio.h>
#include "../inc/rgbpixmap.h"
#include "../inc/glus.h"

/*
 *	create a pixmap for checkboard
 */
// create [10/23/2016 blue]
PRgbMap
rgbCheckboard()
{
	PRgbMap	map; glusAllocex(map, RgbMap, 1, goto _checkboar_failed);

	map->nCol = map->nRow = 64; // set row and column count

	// allocate memory for pixels
	glusAllocex(map->Pixels, RGB, 3 * 64 * 64, goto _checkboar_failed);

	/*
	 *	fill data
	 */
	int count = 0;
	for (int i = 0; i < map->nRow;i++)
	{
		for (int j = 0; j < map->nCol;j++)
		{
			int c = ((i & 8) ^ (j & 8)) * 255;
			map->Pixels[count].R = c;
			map->Pixels[count].G = c;
			map->Pixels[count++].B = c;
		}
	}


	return map;

_checkboar_failed:
	if (map->Pixels)	glusFree(map->Pixels);
	if (map)			glusFree(map);

	return NULL;
}

/*
 *	set texture for opengl
 */
void
rgbSetTexture(
_In_	PRgbMap		rgbmap,
_In_	int			_id_texture)
{
	glBindTexture(GL_TEXTURE_2D, _id_texture);	// binding texture id

	/*
	 *	set parameter
	 */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// set image data
	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_RGB, rgbmap->nRow, rgbmap->nCol, 0,
		GL_RGB, GL_UNSIGNED_BYTE, rgbmap->Pixels);
}