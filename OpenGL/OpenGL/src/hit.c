#include "../inc/glus.h"

/*
 *	type
 bool
 glusHit(PGlusShape _s, PGlusRay _r, PGlusIntersect _inter)
 */

/*
 *	square line in z =0 ,extent from -1 to 1 both in x and  y
 */
bool
glusHitSquare(PGlusShape _s, PGlusRay _r, PGlusIntersect _inter)
{
	assert(_s && _r && _inter);

	/*
	*	get the generic ray
	*/
	GlusRay  ray = *_r;
	glusTransformInvVector(&_s->Transform, &_r->Point, &ray.Point);
	glusTransformInvVector(&_s->Transform, &_r->Direction, &ray.Direction);

	/*
	 *	is ray parallel to plane
	 */ 
	double denom = ray.Direction.Z;
	if (fabs(denom) < Glus_Zero) return false; // miss

	double t = -ray.Point.Z / denom;
	if (t <= 0.0) return false;	// behind the eye

	double hx = ray.Point.X + ray.Direction.X * t;	// hit x
	if (hx > 1.0 || hx < -1.0)	return false;		// miss
	double hy = ray.Point.Y + ray.Direction.Y * t;	// hit y
	if (hy > 1.0 || hy < -1.0)	return false;		// miss

	_inter->numHits = 1;	// one hit 
	_inter->Hits[0].HitObject = _s;
	_inter->Hits[0].hitTime = t;
	_inter->Hits[0].isEnter = true;
	_inter->Hits[0].FaceID = 0;
	glusP(hx, hy, 0, &_inter->Hits[0].HitPoint);// point
	// transform hit point to true coord
	glusTransformVector(&_s->Transform, &_inter->Hits[0].HitPoint, &_inter->Hits[0].HitPoint);
	glusV(0, 0, 1, &_inter->Hits[0].HitNormal);// normal
	glusTransformVector(&_s->Transform, &_inter->Hits[0].HitNormal, &_inter->Hits[0].HitNormal);

	glusLog("\nSquare hit.");
	return true; // ray hit

}

/*
 *	hit for sphere
 */
bool
glusHitSphere(PGlusShape _s, PGlusRay _r, PGlusIntersect _inter)
{
	assert(_s && _r && _inter);

	/*
	 *	get the generic ray
	 */
	GlusRay  ray = *_r;
	glusTransformInvVector(&_s->Transform, &_r->Point, &ray.Point);
	glusTransformInvVector(&_s->Transform, &_r->Direction, &ray.Direction);

	/*
	 *	compute discrim
	 */
	double	A, B, C;
	A = glusDotPro(&ray.Direction, &ray.Direction);
	B = glusDotPro(&ray.Point, &ray.Direction);
	C = glusDotPro(&ray.Point, &ray.Point) - 0.25 ; // radius = 0.5
	
	double dis = B *B - A*C;
	if (dis < 0.0)	return false;	// ray miss

	int num = 0;	// number for hits
	double dis_root = sqrt(dis);

	/*
	 *	t1 
	 */
	double t1 = (-B - dis_root) / A;
	if (t1 > Glus_Zero) // is hit in front of eye
	{
		_inter->Hits[num].hitTime = t1;
		_inter->Hits[num].HitObject = _s;
		_inter->Hits[num].isEnter = true;
		_inter->Hits[num].FaceID = 0;	// for shpere , only one face 
	
		// hit point and normal
		GlusVector v;
		glusRayPos(&ray, t1, &v);
		_inter->Hits[num].HitPoint = v;
		glusTransformVector(&_s->Transform, &_inter->Hits[num].HitPoint, &_inter->Hits[num].HitPoint);
		_inter->Hits[num].HitNormal = v; _inter->Hits[num].HitNormal.V = 0;
		
		num = 1; // have a hit
	}

	/*
	*	t2
	*/
	double t2 = (-B + dis_root) / A;
	if (t2 > Glus_Zero) // is hit in front of eye
	{
		_inter->Hits[num].hitTime = t2;
		_inter->Hits[num].HitObject = _s;
		_inter->Hits[num].isEnter = false;
		_inter->Hits[num].FaceID = 0;	// for shpere , only one face 

		// hit point and normal
		GlusVector v;
		glusRayPos(&ray, t2, &v);
		_inter->Hits[num].HitPoint = v;
		glusTransformVector(&_s->Transform, &_inter->Hits[num].HitPoint, &_inter->Hits[num].HitPoint);
		_inter->Hits[num].HitNormal = v; _inter->Hits[num].HitNormal.V = 0;

		num++; // another hit
	}

	_inter->numHits = num;

	glusLog("\nSphere hit.");
	return true;
}

