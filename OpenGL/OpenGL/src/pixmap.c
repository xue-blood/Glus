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

/*
*	max and min
*	refer :http://stackoverflow.com/questions/7074010/find-maximum-of-three-number-in-c-without-using-conditional-statement-and-ternar
*/
int max3(int a, int b, int c)
{
	int m = a;
	(m < b) && (m = b); //these are not conditional statements.
	(m < c) && (m = c); //these are just boolean expressions.
	return m;
}

int min3(int a, int b, int c)
{
	int m = a;
	(m > b) && (m = b); //these are not conditional statements.
	(m > c) && (m = c); //these are just boolean expressions.
	return m;
}


/*
*	rgb space to hls
*	rgb (int)
*	hls (float)
*/
void	rgb2hsl(PRGB rgb, PHSL hls)
{
	assert(rgb && hls);

	/*
	*	convent rgb from int to double
	*/
	double	r = rgb->R / (float)255,
		g = rgb->G / (float)255,
		b = rgb->B / (float)255;

	double	mx = max3(r, g, b),
		mn = min3(r, g, b);

	// compute the lightness
	hls->L = (mx + mn) / 2.0;

	/*
	*	compute  the saturation
	*/
	if (mx == mn)
		hls->L = 0.0;	// the color is gray
	else	// color is chromatic
	{
		if (hls->L <= 0.5)	hls->S = (mx - mn) / (mx + mn);
		else hls->S = (mx - mn) / (2 - mx + mn);


		/*
		*	compute the hue
		*/
		double tr, tg, tb;
		tr = (mx - r) / (mx - mn);
		tg = (mx - g) / (mx - mn);
		tb = (mx - b) / (mx - mn);

		if (r == mx)		hls->H = tb - tg;
		else if (g == mx)	hls->H = 2 + tr - tb;
		else if (b == mx)	hls->H = 4 + tg - tr;

		hls->H *= 60;
		if (hls->H < 0.0)	hls->H += 360;
	}

}


/*
 *	hls to rgb
 *	refer :http://www.rapidtables.com/convert/color/hsl-to-rgb.htm
 */
void	hsl2rgb(PHSL hls, PRGB rgb)
{
	assert(hls && rgb);

	/*
	 *	C = (1 - |2L - 1|) ¡Á S
		X = C ¡Á (1 - |(H / 60¡ã) mod 2 - 1|)
		m = L - C/2

		... 
		(R,G,B) = ((R'+m)¡Á255, (G'+m)¡Á255,(B'+m)¡Á255)
	 */
	double c, x, m;
	int t = hls->H / 60;

	c = (1 - abs(2 * hls->L - 1))* hls->S;
	x = c *(1 - abs(t % 2 - 1));
	m = hls->L - c / 2;

	double r, g, b;

	switch (t)
	{
	case 0: r = c; g = x; b = 0; break;
	case 1: r = x; g = c; b = 0; break;
	case 2: r = 0; g = c; b = x; break;
	case 3: r = 0; g = x; b = c; break;
	case 4: r = x; g = 0; b = c; break;
	case 5: r = c; g = 0; b = x; break;
	default:	break;
	}

	rgb->R = (r + m) * 255;
	rgb->G = (g + m) * 255;
	rgb->B = (b + m) * 255;
}



void	rgb2hsv(PRGB rgb, PHSV hsv)
{
	assert(rgb && hsv);

	/*
	*	convent rgb from int to double
	*/
	double	r = rgb->R / (float)255,
		g = rgb->G / (float)255,
		b = rgb->B / (float)255;

	double	mx = max3(r, g, b),
			mn = min3(r, g, b),
			md = mx-mn;

	// compute the value
	hsv->V = mx;

	/*
	*	compute  the saturation
	*/
	if (mx == mn)
		hsv->S = 0.0;	// the color is gray
	else	// color is chromatic
	{
		hsv->S = md / mx;

		/*
		*	compute the hue
		*/
		double tr, tg, tb;
		tr = (mx - r) / (mx - mn);
		tg = (mx - g) / (mx - mn);
		tb = (mx - b) / (mx - mn);

		if (r == mx)		hsv->H = tb - tg;
		else if (g == mx)	hsv->H = 2 + tr - tb;
		else if (b == mx)	hsv->H = 4 + tg - tr;

		hsv->H *= 60;
		if (hsv->H < 0.0)	hsv->H += 360;
	}
}
void	hsv2rgb(PHSV hsv, PRGB rgb)
{
	assert(rgb && hsv);

	double c, x, m;
	int t = hsv->H / 60;

	c = hsv->V / hsv->S;
	x = c *(1 - abs(t % 2 - 1));
	m = hsv->V- c;

	double r, g, b;

	switch (t)
	{
	case 0: r = c; g = x; b = 0; break;
	case 1: r = x; g = c; b = 0; break;
	case 2: r = 0; g = c; b = x; break;
	case 3: r = 0; g = x; b = c; break;
	case 4: r = x; g = 0; b = c; break;
	case 5: r = c; g = 0; b = x; break;
	default:	break;
	}

	rgb->R = (r + m) * 255;
	rgb->G = (g + m) * 255;
	rgb->B = (b + m) * 255;
}

/*
 *	rgb space color cube
 *	refer :http://www.swiftless.com/tutorials/opengl/vertex_colors.html
 */
void	rgbSpace()
{
#define S(x,y,z) glColor3f(x, y, z); glVertex3f(x, y, z);
	
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);

	glBegin(GL_QUADS);

	/*
	*	z = 0
	*/
	S(0, 0, 0);		S(0, 1, 0);
	S(1, 1, 0);		S(1, 0, 0); 

	/*
	 *	z = 1
	 */
	S(0, 0, 1);		S(0, 1, 1); 
	S(1, 1, 1);		S(1, 0, 1); 

	/*
	*	x = 0
	*/
	S(0, 0, 0);		S(0, 0, 1);		
	S(0, 1, 1);		S(0, 1, 0); 

	/*
	*	x = 1
	*/
	S(1, 0, 0);		S(1, 0, 1); 
	S(1, 1, 1);		S(1, 1, 0); 

	/*
	*	y = 0
	*/
	S(0, 0, 0);		S(1, 0, 0); 
	S(1, 0, 1);		S(0, 0, 1); 

	/*
	*	y = 1
	*/
	S(0, 1, 0);		S(1, 1, 0); 
	S(1, 1, 1);		S(0, 1, 1);


	glEnd();
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

#undef S
}

/*
 *	refer:http://stackoverflow.com/questions/8043923/gl-triangle-fan-explanation
 */
void	hslSpace()
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glBegin(GL_TRIANGLE_FAN);
	glDisable(GL_CULL_FACE);

#define SQRT32 0.866
	/*
	 *	top
	 */
	glColor3f(1, 1, 1); glVertex3f(0, 1, 0);			// white
	
	glColor3f(1, 0, 0); glVertex3f(1,		0, 0);			// red
	glColor3f(1, 1, 0);	glVertex3f(0.5,		0, SQRT32);		// yellow
	glColor3f(0, 1, 0); glVertex3f(-0.5,	0, SQRT32);	// green
	glColor3f(0, 1, 1); glVertex3f(-1,		0, 0);			// cyan
	glColor3f(0, 0, 1); glVertex3f(-0.5,	0, -SQRT32);	// blue
	glColor3f(1, 0, 1); glVertex3f(0.5,		0, -SQRT32);	// magenta
	glColor3f(1, 0, 0); glVertex3f(1,		0, 0);			// red

	/*
	*	bottom
	*/
	glEnd();

	glBegin(GL_TRIANGLE_FAN);

	glColor3f(0, 0, 0); glVertex3f(0, -1, 0);			// black

	glColor3f(1, 0, 0); glVertex3f(1,		0, 0);			// red
	glColor3f(1, 1, 0);	glVertex3f(0.5,		0, SQRT32);		// yellow
	glColor3f(0, 1, 0); glVertex3f(-0.5,	0, SQRT32);	// green
	glColor3f(0, 1, 1); glVertex3f(-1,		0, 0);			// cyan
	glColor3f(0, 0, 1); glVertex3f(-0.5,	0, -SQRT32);	// blue
	glColor3f(1, 0, 1); glVertex3f(0.5,		0, -SQRT32);	// magenta
	glColor3f(1, 0, 0); glVertex3f(1,		0, 0);			// red

	glEnd();
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
#undef SQRT32
}
void	hsvSpace()
{

#define SQRT32 0.866

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);

	glBegin(GL_TRIANGLE_FAN);
	/*
	*	top
	*/
	glColor3f(1, 1, 1); glVertex3f(0, 1, 0);			// white

	glColor3f(1, 0, 0); glVertex3f(1,		1, 0);			// red
	glColor3f(1, 1, 0);	glVertex3f(0.5,		1, SQRT32);		// yellow
	glColor3f(0, 1, 0); glVertex3f(-0.5,	1, SQRT32);		// green
	glColor3f(0, 1, 1); glVertex3f(-1,		1, 0);			// cyan
	glColor3f(0, 0, 1); glVertex3f(-0.5,	1, -SQRT32);	// blue
	glColor3f(1, 0, 1); glVertex3f(0.5,		1, -SQRT32);	// magenta
	glColor3f(1, 0, 0); glVertex3f(1,		1, 0);			// red
	glEnd();

	/*
	*	bottom
	*/
	glBegin(GL_TRIANGLE_FAN);

	glColor3f(0, 0, 0); glVertex3f(0, 0, 0);			// black

	glColor3f(1, 0, 0); glVertex3f(1,		1, 0);			// red
	glColor3f(1, 1, 0);	glVertex3f(0.5,		1, SQRT32);		// yellow
	glColor3f(0, 1, 0); glVertex3f(-0.5,	1, SQRT32);		// green
	glColor3f(0, 1, 1); glVertex3f(-1,		1, 0);			// cyan
	glColor3f(0, 0, 1); glVertex3f(-0.5,	1, -SQRT32);	// blue
	glColor3f(1, 0, 1); glVertex3f(0.5,		1, -SQRT32);	// magenta
	glColor3f(1, 0, 0); glVertex3f(1,		1, 0);			// red

	glEnd();
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}