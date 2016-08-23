#include "..\inc\glus.h"

/*
[2016.8.5]
	glusDrawMulPolygonS add 
	glusDrawPolygon		change, from array to doubly link
	glusDrawPolygons	add
	glusPolygonUnion	add
//  [8/13/2016 Tld]
	glusDrawPolyLine	add

//  [8/19/2016 xcv]
	change glusTiling to object.c
*/
void
glusDrawPolygon(
_In_	PGlusLink	_p)
{
	assertp(_p );

	
	if (glusLinkLength(_p) < 3)
		return;

	PGlusPoints	p = (PGlusPoints)_p->BLink;
	glBegin(GL_LINE_LOOP);
	{
		while (p != (PGlusPoints)_p)
		{
			glVertex3dv((GLdouble*)&p->Point);

			p = (PGlusPoints)p->Link.BLink;

		}		
	}
	glEnd();
}
void
glusDrawPolyLine(
_In_	PGlusLink	_p)
{
	assertp(_p);


	if (glusLinkLength(_p) < 3)
		return;

	PGlusPoints	p = (PGlusPoints)_p->BLink;
	glBegin(GL_LINE_STRIP);
	{
		while (p != (PGlusPoints)_p)
		{
			glVertex3dv((GLdouble*)&p->Point);

			p = (PGlusPoints)p->Link.BLink;

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

	PGlusPointsS p = (PGlusPointsS)_p->Next;
	if (!p)
		return;

	glBegin(GL_LINE_LOOP);
	{
		while (p)
		{
			glVertex3dv((GLdouble*)&p->Point);
			p = (PGlusPointsS)p->Sink.Next;
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
		PGlusPointsS	pd = (PGlusPointsS)pl->Data.Next;

		//
		// draw the polygon
		// 
		glBegin(GL_LINE_LOOP);
		while (pd)
		{
			glVertex3dv((pGLdouble)&pd->Point);
			
			pd = (PGlusPointsS)pd->Sink.Next;
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

	PGlusPoints	paa, pab,pha,phb, p;
	paa = (PGlusPoints)_pa->BLink;

	// start from polygon a
	pha = (PGlusPoints)_pa;
	phb = (PGlusPoints)_pb;
	while (!glusLinkIsEnd(paa,pha))
	{
		pab = (PGlusPoints)paa->Link.BLink;
		if (glusLinkIsEnd(pab, pha))
			pab = (PGlusPoints)pha->Link.BLink;

		//
		// add point to result
		// 
		p = (PGlusPoints)malloc(sizeof(GlusPoints));
		*p = *paa;
		glusLinkInsertTail(_po, p);

		//
		// is there a intersection
		// 
		bool	isIntsect = false;
		GlusVector	pi;
		PGlusPoints	pta = (PGlusPoints)phb->Link.BLink, ptb, pnext=NULL;
		while (!glusLinkIsEnd(pta, phb))
		{
			ptb = (PGlusPoints)pta->Link.BLink;
			if (glusLinkIsEnd(ptb, phb))
				ptb = (PGlusPoints)phb->Link.BLink;

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
			pta = (PGlusPoints)pta->Link.BLink;
		}
		
		if (isIntsect)
		{
			//
			// add the intersection
			// 
			p = (PGlusPoints)malloc(sizeof(GlusPoints));
			p->Point = pi;
			glusLinkInsertTail(_po, p);

			//
			// convert the pointer
			// 
			if (glusLinkIsEnd(pnext->Link.BLink,phb))
				pnext = (PGlusPoints)pnext->Link.BLink;
			PGlusPoints	pt;
			pt = pha, pha = phb, phb = pt;
			paa = pnext;
		}

		//
		// next point
		// 
		paa = (PGlusPoints)paa->Link.BLink;


	}

}

