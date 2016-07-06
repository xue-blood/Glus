#include "..\inc\glus.h"


bool
glusMousePolygonS(
_In_	int		_x,
_In_	int		_y,
_Out_	pvoid	_po)
{
	assertp(_po);

	PGlusPolygonS	p = (PGlusPolygonS)malloc(sizeof(GlusPolygonS));
	if (!p)	return	-1;

	glusP2i(_x, _y, &p->Point);
	glusSinkPush(_po,(PGlusSink)p);
	return 1;
}