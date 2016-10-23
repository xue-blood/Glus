


typedef	struct _RGB
{
	unsigned char	R, G, B;
}RGB,*PRGB;

typedef	struct _RgbMap
{
	int		nRow, nCol;
	PRGB	Pixels;
}RgbMap, *PRgbMap;


PRgbMap	rgbCheckboard();
void	rgbSetTexture(_In_	PRgbMap		rgbmap,_In_	int			_id_texture);