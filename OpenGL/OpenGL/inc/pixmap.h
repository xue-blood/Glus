#include "stdbool.h"
#include "link.h"

#ifndef _PIXMAP_H
#define _PIXMAP_H
typedef	struct _RGB
{
	unsigned char	R, G, B;
}RGB,*PRGB;

typedef	struct _RGBA
{
	unsigned char	R, G, B, A;
}RGBA, *PRGBA;

typedef	struct _PixMap
{
	int		nRow, nCol;
	PRGBA	Pixels;
}PixMap, *PPixMap;

typedef	struct _HSL
{
	double H, L, S;
}HSL, *PHSL;


typedef	struct _HSV
{
	double H, S, V;
}HSV, *PHSV;

/*
*	data-struct for rectangle-defined region
*/
typedef struct _RectRegionRun
{
	GlusLink	Link;
	int			Row;
	int			Col;
	int			Len;
}RectRegionRun, *PRectRegionRun;


typedef	GlusLink	RectRegion, *PRectRegion;

typedef struct _Chain
{
	int		X,  Y;
	int		Len, StepLen;
	char*	Steps;
}Chain, *PChain;

unsigned int rgbg2uint(PRGBA p);

PPixMap	pixCheckboard();
void	pixSetTexture(PPixMap		pixmap, int			_id_texture);

#define pixSet(ptr,x,y,ppix)								\
	if(x>=0 && x<(ptr)->nRow && y >=0 && y < (ptr)->nCol)	\
		(ptr)->Pixels[(ptr)->nCol*y+x] = *(ppix)

#define pixGet(ptr,x,y,ppix)								\
	if(x>=0 && x<(ptr)->nRow && y >=0 && y < (ptr)->nCol)	\
		*(ppix)= (ptr)->Pixels[(ptr)->nCol*y+x]

#define pixDraw(ptr)	glDrawPixels((ptr)->nCol,(ptr)->nRow,GL_RGBA,GL_UNSIGNED_BYTE,(ptr)->Pixels)

#define pixCopy(x,y,wid,ht)		glCopyPixels(x,y,wid,ht,GL_COLOR)

#define pixRead(ptr,x,y,wid,ht)		(ptr)->nRow = ht;(ptr)->nCol=wid;		\
			glReadPixels(x,y,wid,ht,GL_RGBA,GL_UNSIGNED_BYTE,(ptr)->Pixels)

#define pixBlend(ptr)											\
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);	\
			glEnable(GL_BLEND);									\
			pixDraw(ptr);


bool	pixReadBmpFile(char *_file_name);
bool	pixWriteBmpFile(char *_file_name);


void	pixChromaKey(PPixMap p, unsigned char r, unsigned char g, unsigned char b);
void	pixSetAlpha(PPixMap p, float alpha);

bool	pix2Rect(PPixMap _map,PRectRegion _region,PRGBA	_color);
void	rect2pix(PPixMap _map,PRectRegion _region,PRGBA	_fore,PRGBA	_back);

void	pix2chain(PPixMap _map, PChain p, int x, int y);
void	chainDraw(PChain p);

int max3(int a, int b, int c);
int min3(int a, int b, int c);

/*
 *	color convent
 */
void	rgb2hsl(PRGB rgb, PHSL hls);
void	hsl2rgb(PHSL hls, PRGB rgb);
void	rgb2hsv(PRGB rgb, PHSV hsv);
void	hsv2rgb(PHSV hsv, PRGB rgb);
#endif