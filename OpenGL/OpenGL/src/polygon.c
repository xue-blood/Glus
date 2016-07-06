#include "..\inc\glus.h"

/*
[2016.8.5]
	add glusDrawMulPolygonS
*/
void
glusDrawPolygon(
_In_	PGlusPolygon	_p)
{
	assertp(_p && _p->Count && _p->Data);

	glBegin(GL_LINE_LOOP);
	{
		for (GLint i = 0; i < _p->Count; i++)
		{
			glVertex3dv((GLdouble*)_p->Data+i);
		}
	}
	glEnd();
}


void	
glusDrawPolygonS(
_In_	PGlusSink	_p)
{
	assertp(_p);

	PGlusPolygonS p = (PGlusPolygonS)_p->Next;
	if (!p)
		return;

	glBegin(GL_LINE_LOOP);
	{
		while (p)
		{
			glVertex3dv((GLdouble*)&p->Point);
			p = (PGlusPolygonS)p->Sink.Next;
		}
	}
	glEnd();
}

void
glusDrawMulPolygonS(
_In_	PGlusSink	_p)
{
	assertp(_p);

	// get the polygons list head
	PGlusMulSink	pl = (PGlusMulSink)_p->Next;
	
	while (pl)
	{
		// get the polygon head
		PGlusPolygonS	pd = (PGlusPolygonS)pl->Data.Next;

		//
		// draw the polygon
		// 
		glBegin(GL_LINE_LOOP);
		while (pd)
		{
			glVertex3dv((pGLdouble)&pd->Point);
			
			pd = (PGlusPolygonS)pd->Sink.Next;
		}
		glEnd();

		pl = (PGlusMulSink)pl->Sink.Next;
	}
}
