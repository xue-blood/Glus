#include <stdio.h>
#include "../inc/pixmap.h"
#include "../inc/glus.h"

/*
 *	create a pixmap for checkboard
 */
// create [10/23/2016 blue]
PPixMap
pixCheckboard()
{
	PPixMap map; glusAllocex(map, PixMap, 1, goto _checkboar_failed);

	map->nCol = map->nRow = 64; // set row and column count

	// allocate memory for pixels
	glusAllocex(map->Pixels, RGBA, 3 * 64 * 64, goto _checkboar_failed);

	/*
	 *	fill data
	 */
	int count = 0;
	for (int i = 0; i < map->nRow;i++)
	{
		for (int j = 0; j < map->nCol;j++)
		{
			int c = ((i & 8) ^ (j & 8)) * 255;
			map->Pixels[count].R = 0;
			map->Pixels[count].G = c;
			map->Pixels[count].B = c;
			map->Pixels[count++].A = 255;
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
pixSetTexture(
_In_	PPixMap		_map,
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
		GL_RGBA, _map->nRow, _map->nCol, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, _map->Pixels);
}

void	
pixChromaKey(PPixMap p, float fr, float fg, float fb)
{
	assert(p);

	long count = 0;
	unsigned char r = fr * 255, g = fg * 255, b = fb * 255;

	for (int row = 0; row < p->nRow;row ++)
	{
		for (int col = 0; col < p->nCol;col ++)
		{
			PRGBA c = p->Pixels + count++;	

			// is color equal background
			if (c->R == r && c->G == g && c->B == b)
				c->A = 0;
			else
				c->A = 255;
			
		}
	}
}
