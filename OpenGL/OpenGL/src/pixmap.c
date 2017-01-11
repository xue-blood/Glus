#include <stdio.h>
#include "../inc/pixmap.h"
#include "../inc/glus.h"


/*
 *	convet for rgba to unsinged
 *	(only on x86 pc)
 */
unsigned int rgbg2uint(PRGBA p)
{
	unsigned int r = p->R; r <<= 8;
	r += p->G; r <<= 8;
	r += p->B; r <<= 8;
	r += p->A;
	
	return r;
}
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
	glusAllocex(map->Pixels, RGBA,  64 * 64, goto _checkboar_failed);

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
pixChromaKey(PPixMap p, unsigned char r, unsigned char g, unsigned char b)
{
	assert(p);

	long count = 0;
	//unsigned char r = fr * 255, g = fg * 255, b = fb * 255;

	for (int row = 0; row < p->nRow;row ++)
	{
		for (int col = 0; col < p->nCol;col ++)
		{
			PRGBA c = p->Pixels + count++;	

			// is color equal background
			if (c->R == r && c->G == g && c->B == b)
				c->A = 0;
			
			
		}
	}
}

void
pixSetAlpha(PPixMap p, float alpha)
{
	assert(p);

	int count = 0;
	unsigned char a = alpha * 255;

	for (int row = 0; row < p->nRow; row++)
	{
		for (int col = 0; col < p->nCol; col++)
		{
			p->Pixels[count++].A = a;
		}
	}
}


bool
pix2Rect(
_In_	PPixMap			_map,
_In_	PRectRegion		_region,
_In_	PRGBA			_color)
{
	assert(_map && _region);

	glusLinkInit(_region);	// to prevent problem

	int count = 0;
	bool start = false;
	PRectRegionRun	run = NULL;

	for (int row = 0; row < _map->nRow;_map++)
	{
		for (int col = 0; col < _map->nCol;col++)
		{
			if (memequ(_map->Pixels + count++, _color, sizeof(RGBA)))
			{
				if (!start) // start a new run
				{
					start = true;

					glusAllocex(run, RectRegionRun,1,goto _region_failed_);
					run->Row = row;
					run->Col = col;

					glusLinkInsertTail(_region, run);
				}
				else
				{
					run->Len++; // just add the length
				}
			}
			else
			{
				start = false;  // end a start or just set to no-start
			}
		}
	}

	return true;
_region_failed_:

	return false;
}

void
rect2pix(
_In_	PPixMap			_map,
_In_	PRectRegion		_region,
_In_	PRGBA			_fore,
_In_	PRGBA			_back)
{
	assert(_map && _region);

	int count = 0;
	for (int row = 0; row < _map->nRow; row++)
		for (int col = 0; col < _map->nCol; col++)
			memcpy_s(_map->Pixels + count++, sizeof(RGBA), _back, sizeof(RGBA));

	/*
	 *	now start the run
	 */
	PRectRegionRun	run = (PRectRegionRun)_region->BLink;
	while (!glusLinkIsHead(run,_region))
	{
		PRGBA p = _map->Pixels + run->Row*_map->nCol + run->Col;

		for (int i = 0; i < run->Len;i++)
			memcpy_s(p + i, sizeof(RGBA), _fore, sizeof(RGBA));
		run = (PRectRegionRun)run->Link.BLink;
	}
	
}

/*
 *	draw a chain-code 
 *	use 8-connected
 */
void	
chainDraw(PChain p)
{
	assert(p);

	int x = p->X, y = p->Y;

	

	for (int i = 0; i < p->Len;i++)
	{
		drawPoint(x, y);
		switch (p->Steps[i])
		{
		case 0: x += p->StepLen; break;
		case 1: x += p->StepLen; y += p->StepLen; break;
		case 2: y += p->StepLen; break;
		case 3: x -= p->StepLen; y += p->StepLen; break;
		case 4: x -= p->StepLen; break;
		case 5: x -= p->StepLen; y -= p->StepLen; break;
		case 6: y -= p->StepLen; break;
		case 7: x += p->StepLen; y -= p->StepLen; break;
		default:
			break;
		}
	}
}