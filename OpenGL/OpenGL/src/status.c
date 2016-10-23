#include "../inc/glus.h"


/*
 *	shade level
 */
// create [10/23/2016 blue]
static	Glusenum	_ShadeLevel;

void
glusSetShadeLevel(
_In_	Glusenum	_param)
{
	_ShadeLevel = _param;
}

Glusenum
glusGetShadeLevel()
{
	return _ShadeLevel;
}