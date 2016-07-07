#include "..\inc\glus.h"

/*
[2016.8.5]
	glusDrawMulPolygonS add 
	glusDrawPolygon		change, from array to doubly link
	glusDrawPolygons	add
	glusPolygonUnion	add
*/
void
glusDrawPolygon(
_In_	PGlusLink	_p)
{
	assertp(_p );

	
	if (glusLinkLength(_p) < 3)
		return;

	PGlusPolygon	p = (PGlusPolygon)_p->BLink;
	glBegin(GL_LINE_LOOP);
	{
		while (p != (PGlusPolygon)_p)
		{
			glVertex3dv((GLdouble*)&p->Point);

			p = (PGlusPolygon)p->Link.BLink;

		}		
	}
	glEnd();
}
void
glusDrawPolygons(
_In_	PGlusLink	_p)
{
	assertp(_p);

	if (glusLinkIsEmpty((PGlusLink)_p))
		return;

	PGlusLinks	p = (PGlusLinks)_p->BLink;
	while (p != (PGlusLinks)_p)
	{
		glusDrawPolygon(&p->Data);
		p = (PGlusLinks)p->Link.BLink;
	}
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

//
// union two polygon
// just can handle the same direction of points
// 
void
glusPolygonUnion(
_In_	PGlusLink	_pa,
_In_	PGlusLink	_pb,
_Out_	PGlusLink	_po)
{
	assertp(_pa && _pb && _po);

	// check the polygons
	if (glusLinkLength((PGlusLink)_pa) < 3 || glusLinkLength((PGlusLink)_pb) < 3)
		return;

	PGlusPolygon	paa, pab,pha,phb, p;
	paa = (PGlusPolygon)_pa->BLink;

	// start from polygon a
	pha = (PGlusPolygon)_pa;
	phb = (PGlusPolygon)_pb;
	while (!glusLinkIsEnd(paa,pha))
	{
		pab = (PGlusPolygon)paa->Link.BLink;
		if (glusLinkIsEnd(pab, pha))
			pab = (PGlusPolygon)pha->Link.BLink;

		//
		// add point to result
		// 
		p = (PGlusPolygon)malloc(sizeof(GlusPolygon));
		*p = *paa;
		glusLinkInsertTail(_po, p);

		//
		// is there a intersection
		// 
		bool	isIntsect = false;
		GlusVector	pi;
		PGlusPolygon	pta = (PGlusPolygon)phb->Link.BLink, ptb, pnext=NULL;
		while (!glusLinkIsEnd(pta, phb))
		{
			ptb = (PGlusPolygon)pta->Link.BLink;
			if (glusLinkIsEnd(ptb, phb))
				ptb = (PGlusPolygon)phb->Link.BLink;

			GlusVector pt;
			if (Intersect_Exsit == glusLIntersect(
				&paa->Point, &pab->Point, &pta->Point, &ptb->Point, &pt))
			{
				if (!isIntsect)
					pi = pt, pnext = pta;
				isIntsect = true;

				//
				// get the least point distance from paa
				// 
				if (glusPDistance(&paa->Point, &pi) >
					glusPDistance(&paa->Point, &pt))
					pi = pt, pnext = pta;
			}
			pta = (PGlusPolygon)pta->Link.BLink;
		}
		
		if (isIntsect)
		{
			//
			// add the intersection
			// 
			p = (PGlusPolygon)malloc(sizeof(GlusPolygon));
			p->Point = pi;
			glusLinkInsertTail(_po, p);

			//
			// convert the pointer
			// 
			if (glusLinkIsEnd(pnext->Link.BLink,phb))
				pnext = (PGlusPolygon)pnext->Link.BLink;
			PGlusPolygon	pt;
			pt = pha, pha = phb, phb = pt;
			paa = pnext;
		}

		//
		// next point
		// 
		paa = (PGlusPolygon)paa->Link.BLink;


	}

}