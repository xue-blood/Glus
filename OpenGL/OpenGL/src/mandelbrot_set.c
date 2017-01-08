#include "../inc/glus.h"
#define NUM 100

int dwell(double cx, double cy)
{


	double tmp, dx = cx, dy = cy, fsq = cx*cx + cy*cy;
	int i = 0;

	for (; i <= NUM&& fsq <= 4;i++)
	{
		tmp = dx;  // store old real part

		dx = dx*dx - dy*dy + cx;	// new real part
		dy = 2.0 *tmp *dy + cy;		// new image part
		fsq = dx*dx + dy*dy;		
	}

	return i;

}


PPixMap glusMandelbrotSet(
_In_	double	px,
_In_	double	py,
_In_	double	w)
{
	glusAlloc(map, PixMap);
	map->nCol = px, map->nRow = py;
	glusAllocex(map->Pixels, RGBA, px*py, goto _manset_failed_);

	for (int j = 0; j < map->nCol; j++)
	{
		for (int i = 0; i < map->nRow;i++)
		{
			/*
			 *	find the corresponding c-val for current point
			 */
			double cx = px + ((i + 0.5) / map->nCol)*w;
			double cy = py + ((j + 0.5) / map->nCol)*w;

			/*
			 *	estimate the dwell for the orbit
			 */
			int d = dwell(cx, cy);
			float v = d / (float)NUM;

			/*
			 *	store the color
			 */
			map->Pixels[j*map->nRow + i].R = v*v *255;
			map->Pixels[j*map->nRow + i].G = v*v * 255;
			map->Pixels[j*map->nRow + i].B = 0.2 * 255;
			map->Pixels[j*map->nRow + i].A = 255;
		}
	}

	return map;

_manset_failed_:
	glusFree(map->Pixels);
	glusFree(map);

	return NULL;
}

#undef NUM