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
	while (!glusLinkIsHead(paa,pha))
	{
		pab = (PGlusPoints)paa->Link.BLink;
		if (glusLinkIsHead(pab, pha))
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
		while (!glusLinkIsHead(pta, phb))
		{
			ptb = (PGlusPoints)pta->Link.BLink;
			if (glusLinkIsHead(ptb, phb))
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
			if (glusLinkIsHead(pnext->Link.BLink,phb))
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

/*
 *	clean for array list points
 */
// create [9/1/2016 blue]
void
glusPointsClear_A(
_In_ PGlusVector _pointer)
{
	assertp(_pointer);

	// just free it
	glusFree(_pointer);
}

/*
 *	free for link list points
 */
// add [9/1/2016 blue]
void
glusPointsClear_L(
_In_ PGlusLink _head)
{
	assertp(_head);

	// just call the function
	glusLinkClear(_head);
	
	// but we should free head
	glusFree(_head);
}

/*
 *	compute the normal for array point list
 */
// create [9/2/2016 blue]
void
glusPointsNormal_A(
_In_	PGlusVector	_p_points,
_In_	Glusnum		_n_points,
_Inout_	PGlusVector	_normal)
{
	assertp(_n_points && _p_points && _normal);

	ZeroMemory(_normal, sizeof(GlusVector));

	// use macro 
#define Express(m,o)	(_p_points[i].m o _p_points[(i+1)%_n_points].m)

	for (Glusnum i = 0; i < _n_points;i++)
	{
		_normal->X += Express(Y, -)*Express(Z, +);
		_normal->Y += Express(Z, -)*Express(X, +);
		_normal->Z += Express(X, -)*Express(Y, +);
	}
	// the V of normal is zero

#undef Express
	// normalize the normal
	glusVNormalize(_normal);	
}

/*
 *	compute normal of link list points
 */
// create [9/2/2016 blue]
void
glusPointsNormal_L(
_In_	PGlusLink	_head,
_Inout_	PGlusVector	_normal)
{
	assertp(_head && _normal);

	ZeroMemory(_normal, sizeof(GlusVector));

	// use macro too
#define Express(m,o)	(p->Point.m o p_n->Point.m) 
	
	PGlusPoints	p = (PGlusPoints)_head->BLink, p_n;

	while (!glusLinkIsHead(p,_head))
	{
		// get the pointer of next
		glusLinkNext(_head, p, p_n);

		_normal->X += Express(Y, -)*Express(Z, +);
		_normal->Y += Express(Z, -)*Express(X, +);
		_normal->Z += Express(X, -)*Express(Y, +);

		p = (PGlusPoints)p->Link.BLink;
	}
#undef Express

	glusVNormalize(_normal);
}

/*
 *	convert points data_struct from link list to array
 *		the caller must care of the memory
 */
// create [9/19/2016 blue]
void
glusPointsConvert_L_A(
_In_	PGlusLink	_h_points,
_Out_	PGlusVector	_a_points,
_In_	Glusnum		_n)
{
	assertp(_h_points && _a_points);
	assert(_n > 0);

	PGlusPoints p = (PGlusPoints)_h_points->BLink;
	for (Glusindex i = 0; 
		i < _n,!glusLinkIsHead(p,_h_points);
		i++,p=(PGlusPoints)p->Link.BLink)
	{
		_a_points[i] = p->Point;
	}
}

/*
 *	convert points data_struct from array to link list
 *		and we will allocate memory,
 *		but the link head should be initialized
 */
// create [9/19/2016 blue]
void
glusPointsConvert_A_L(
_In_	PGlusVector	_a_points,
_In_	Glusnum		_n,
_Inout_	PGlusLink	_h_points)
{
	assertp(_a_points && _h_points);
	assert(_n >0);

	PGlusPoints p;

	for (Glusindex i = 0; i < _n; i++)
	{
		
		glusAllocex(p, GlusPoints, 1, goto _failed_);

		p->Point = _a_points[i];

		glusLinkInsertTail(_h_points, p);
	}
	return;

_failed_:
	glusLinkClear(_h_points);
}

/*
 *	extrude the points array with the specify vector
 */
// create [9/19/2016 blue]
void
glusPointsExtrude(
_In_	PGlusVector		_source,
_Inout_	PGlusVector		_target,
_In_	Glusnum			_n,
_In_	PGlusVector		_vector)
{
	assertp(_source && _target);
	assert(_n > 0);


	for (Glusindex i = 0; i < _n; i++)
	{
		glusPAddV(_source + i, _vector,
			_target + i);
	}
}