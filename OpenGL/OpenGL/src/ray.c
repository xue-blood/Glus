#include "..\inc\glus.h"

/*
//  [8/5/2016 Tld]
	add glusRHitPolygonS
	add glusRHitMulPolygonS
	add glusRTrace2D
	add glusRTraces2D
*/


void	
glusRDraw(
_In_	PGlusRay	_pRay)
{
	assertp(_pRay);

	GlusVector p;
	glusAdd(&_pRay->Point, 1, &_pRay->Direction, 1, &p);
	p.V = 1;

	glBegin(GL_LINES);
	{
		glVertex3d(_pRay->Point.X,_pRay->Point.Y,_pRay->Point.Z);
		glVertex3d(p.X,p.Y,p.Z);
	}
	glEnd();
}

//
// when a ray hit a point-normal object
// compute the hit time
// both in 2d line and 3d plane
// 
GLdouble
glusRHit(
_In_	PGlusRay	_r,
_In_	PGlusVector	_n,
_In_	PGlusVector	_p)
{
	assertp(_r && _n && _p);

	GLdouble	r;
	r = glusDotPro(_n, &_r->Direction);

	// if parallel
	// then return infinity
	if (r == 0)
		return INFINITY;

	// compute the time of hit
	GlusVector	vb;
	glusVFromPoint(&_r->Point, _p, &vb);

	return	glusDotPro(_n, &vb) / r;
}

//
// compute the reflect in 2d
// 
//  [8/5/2016 Tld]: add
GLdouble
glusRHit2D(
_In_	PGlusRay	_r,
_In_	PGlusVector	_pa,
_In_	PGlusVector	_pb)
{
	assertp(_r && _pa && _pb);

	//
	//  we compute the normal vector
	// 
	GlusVector	v, vn;
	glusVFromPoint(_pa, _pb, &v);
	glusNormal(&v, &vn);


	//	get the hit time of the ray
	GLdouble	t = glusRHit(_r, &vn, _pa);
	
	//
	//  is the point is  exist or valid
	// 
	GLdouble	length = glusDistance(_pa, _pb);	//  [8/5/2016 Tld]: the length of the 
	glusAdd(&_r->Point, 1, &_r->Direction, t, &v);	//  [8/5/2016 Tld]: the hit point
	GLdouble	la, lb;	//  [8/5/2016 Tld]: the distance of the hit point between two end point
	la = glusDistance(_pa, &v);
	lb = glusDistance(_pb, &v);
	//  [8/5/2016 Tld]: if the point is valid,the distance must smaller than total length
	if (la > length || lb > length)
		return INFINITY;
	return t;
}

//
// when a ray hit a line or plane
// compute the direction after hit
// 
void
glusReflect(
_Inout_	PGlusRay	_ray,
_In_	PGlusVector	_normal,
_Out_	PGlusRay	_nray)
{
	assertp(_ray && _normal && _nray);

	GlusVector	n = *_normal;

	GLdouble	dot = glusDotPro(&n, &_ray->Direction);

	if (dot == 0)						// is parallel
		_nray->Direction = _ray->Direction;	// so the direction don't change
	if (dot <  0)			// the normal is not we want
		glusVOpposite(&n);	// we use the opposite normal

	// normalize the normal
	glusNormalize(&n);

	//
	// now we can compute the direction after reflection 
	// 
	glusAdd(	&_ray->Direction, 1, 
				&n, -2 * glusDotPro(&_ray->Direction, &n), 
				&_nray->Direction);
	glusValid(&_nray->Direction);

	// position
	glusRayPos(_ray, 1, &_nray->Point);

}


/*
 *	get the transmission ray after refract
 */
void
glusRefract(
_Inout_	PGlusRay	_ray,
_In_	PGlusVector	_normal,
_Out_	PGlusRay	_nray,
_In_	real		_c)
{
	assertp(_ray && _normal && _nray);


	GlusVector	n = *_normal;
	glusNormalize(&n);
	GlusVector	dir = _ray->Direction;
	glusNormalize(&dir);

	GLdouble	dot = glusDotPro(&n, &dir);
	double theta = sqrt(1 - _c*_c *(1 - dot*dot));

	if (dot == 0)						// is parallel
		_nray->Direction= _ray->Direction;	// so the direction don't change
	//if (dot < 0)			// the normal is not we want
		//glusVOpposite(&n);	// we use the opposite normal

	//
	// now we can compute the direction after refraction
	// 
	glusAdd(&dir, _c,
		&n, _c * dot - theta,
		&_nray->Direction);
	glusValid(&_nray->Direction);
	
	double l = glusLength(&_ray->Direction);
	glusRayScale(_nray, sqrt(l));

	// position
	glusRayPos(_ray, 1, &_nray->Point);
	
}

GLdouble	
glusRTrace2D(
_In_	PGlusRay	_ray, 
_In_	PGlusLink	_head, 
_Out_	PGlusRay	_nRay)
{
	assertp(_ray && _head && _nRay);

	PGlusPoints	pa = (PGlusPoints)_head->BLink, pb, pHit = NULL;

	GLdouble		tHit = INFINITY;

	while (!glusLinkIsHead(pa,_head))
	{
		pb = (PGlusPoints)pa->Link.BLink;
		if (glusLinkIsHead(pb,_head))
			pb = (PGlusPoints)_head->BLink;


		//
		// we can compute the hit time
		// 
		//  [8/5/2016 Tld]: change the glusRHit to glusRHit2D
		GLdouble t = glusRHit2D(_ray, &pa->Point, &pb->Point);
		if (t > 0 && t < tHit)
		{
			tHit = t;	// update the time
			pHit = pa;	// and the part of the polygon
		}


		pa = (PGlusPoints)pa->Link.BLink;
	}

	if (!pHit)
		return tHit;

	// the new ray point
	glusAdd(&_ray->Point, 1, &_ray->Direction, tHit, &_nRay->Point);
	_nRay->Point.V = 1;

	pa = (PGlusPoints)_head->BLink;
	while (!glusLinkIsHead(pa,_head))
	{
		//  [8/5/2016 Tld]
		//
		//	if hit vector, then set the opposite direction
		// 
		if (!memcmp(&pa->Point, &_nRay->Point, sizeof(GlusVector)))
		{
			glusVOpposite(&_nRay->Direction);
			return tHit;
		}

		pa = (PGlusPoints)pa->Link.BLink;
	}

	//
	// the new ray direction
	//
	GlusVector	v, vn;
	pb = (PGlusPoints)pHit->Link.BLink;
	if (glusLinkIsHead(pb,_head))
		pb = (PGlusPoints)_head->BLink;

	glusVFromPoint(&pHit->Point, &pb->Point, &v);
	glusNormal(&v, &vn);
	glusReflect(_ray, &vn, _nRay);


	return tHit;
}


void	
glusRTraces2D(
_Inout_	PGlusRay	_ray,
_In_	PGlusLinks	_head)
{
	assertp(_ray && _head);

	PGlusLinks	pl = (PGlusLinks)_head->Link.BLink;

	GLdouble	tHit = INFINITY;
	GlusRay		rHit = null;

	while (!glusLinkIsHead(pl,_head))
	{

		GLdouble	t;
		GlusRay		rn;
		t = glusRTrace2D(_ray, &pl->Data, &rn);

		if (t > Glus_Zero && t < tHit)	//  [8/5/2016 Tld]:change 0 to Zero
		{
			tHit = t;
			rHit = rn;
		}

		pl = (PGlusLinks)pl->Link.BLink;
	}
	*_ray = rHit;
}


//
// compute the ray reflect for a polygon
// and we return the hit time so can use it to compare with other polygon
// 
GLdouble
glusRHit2DS(
_In_	PGlusRay	_ray,
_In_	PGlusSink	_head,
_Out_	PGlusRay	_nRay)
{
	assertp(_ray && _head && _nRay);

	PGlusPointsS	pa = (PGlusPointsS)_head->Next, pb, pHit=NULL;
	
	GLdouble		tHit = INFINITY;

	while (pa)
	{
		pb = (PGlusPointsS)pa->Sink.Next;
		if (!pb)
			pb = (PGlusPointsS)_head->Next;

		
		//
		// we can compute the hit time
		// 
		//  [8/5/2016 Tld]: change the glusRHit to glusRHit2D
		GLdouble t = glusRHit2D(_ray, &pa->Point, &pb->Point);
		if (t > 0 && t < tHit)
		{
			tHit = t;	// update the time
			pHit = pa;	// and the part of the polygon
		}


		pa = (PGlusPointsS)pa->Sink.Next;
	}

	if (!pHit)
		return tHit;

	// the new ray point
	glusAdd(&_ray->Point, 1, &_ray->Direction, tHit, &_nRay->Point);
	_nRay->Point.V = 1;
	
	pa = (PGlusPointsS)_head->Next;
	while (pa)
	{
		//  [8/5/2016 Tld]
		//
		//	if hit vector, then set the opposite direction
		// 
		if (!memcmp(&pa->Point,&_nRay->Point,sizeof(GlusVector)))
		{
			glusVOpposite(&_nRay->Direction);
			return tHit;
		}

		pa = (PGlusPointsS)pa->Sink.Next;
	}

	//
	// the new ray direction
	//
	GlusVector	v, vn;
 	pb = (PGlusPointsS)pHit->Sink.Next;
	if (!pb)
		pb = (PGlusPointsS)_head->Next;

	glusVFromPoint(&pHit->Point, &pb->Point, &v);
	glusNormal(&v, &vn);
	glusReflect(_ray, &vn, _nRay);

	return tHit;
}

void	
glusRHitMul2DS(
_Inout_	PGlusRay		_ray,
_In_	PGlusMulSink	_head)
{
	assertp(_ray && _head);

	PGlusMulSink	pl = (PGlusMulSink)_head->Sink.Next;

	GLdouble	tHit = INFINITY;
	GlusRay		rHit = null;

	while (pl)
	{
		
		GLdouble	t;
		GlusRay		rn;
		t = glusRHit2DS(_ray, &pl->Data, &rn);

		if (t > Glus_Zero && t < tHit)	//  [8/5/2016 Tld]:change 0 to Zero
		{
			tHit = t;
			rHit = rn;
		}

		pl = (PGlusMulSink)pl->Sink.Next;
	}
	*_ray = rHit;
}



//
// from many point-normal objects
// compute the correct ray after reflect 
// the point-normal objects use singly link
// 
void
glusRHitS(
_Inout_	PGlusRay		_ray,
_In_	PGlusSink		_head)
{
	assertp(_ray && _head);

	PGlusPAndNS		p = (PGlusPAndNS)_head->Next;

	GLdouble		tHit = INFINITY;
	PGlusPAndNS		pHit;

	while (p)
	{

		//
		// find the positive earliest hit time 
		// 
		GLdouble	t = glusRHit(_ray, &p->Data.Normal, &p->Data.Point);
		if (t > 0 && t < tHit)
		{
			// update hit time
			tHit = t;

			// and the pillar
			pHit = p;
		}

		p = (PGlusPAndNS)p->Sink.Next;
	}

	//
	// then we can compute the new ray after reflect
	// 
	GlusRay		nRay;
	glusReflect(_ray, &p->Data.Normal, &nRay);

	// and the point
	glusAdd(&_ray->Point, 1, &_ray->Direction, tHit, &nRay.Point);
	nRay.Point.V = 1;

	*_ray = nRay;
}
