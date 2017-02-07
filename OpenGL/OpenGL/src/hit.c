#include "../inc/glus.h"

/*
 *	proto type
 bool
 glusHit(PGlusShape _s, PGlusRay _r, PGlusIntersect _inter)
 {
	assert(_s && _r && _inter);

	_inter->numHits = 0;

	//get the generic ray
	
	GlusRay  ray = *_r;
	glusTransformInvVector(&_s->Transform, &ray.Point);
	glusTransformInvVector(&_s->Transform, &ray.Direction);


	#define RD(field) (ray.Direction.field)	// shortcut for ray direction
	#define RP(field) (ray.Point.field)		// shortcut for ray point

	#undef RP
	#undef RD
 }
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
	glusTransformInvVector(&_s->Transform, &ray.Point);
	glusTransformInvVector(&_s->Transform, &ray.Direction);

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
	_inter->Hits[0].hitTime = t;
	_inter->Hits[0].isEnter = true;
	_inter->Hits[0].FaceID = 0;
	glusP(hx, hy, 0, &_inter->Hits[0].HitPoint);// point
	// transform hit point to true coord
	glusTransformVector(&_s->Transform, &_inter->Hits[0].HitPoint);
	glusV(0, 0, 1, &_inter->Hits[0].HitNormal);// normal
	glusTransformVector(&_s->Transform, &_inter->Hits[0].HitNormal);
	glusNormalize(&_inter->Hits[0].HitNormal);

	_inter->HitObject = _s;
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
	glusTransformInvVector(&_s->Transform, &ray.Point);
	glusTransformInvVector(&_s->Transform, &ray.Direction);

	/*
	 *	compute discrim
	 */
	double	A, B, C;
	A = glusDotPro(&ray.Direction, &ray.Direction);
	B = glusDotPro(&ray.Point, &ray.Direction);
	C = glusDotPro(&ray.Point, &ray.Point) - 1 ; // radius = 1
	
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
		_inter->Hits[num].isEnter = true;
		_inter->Hits[num].FaceID = 0;	// for shpere , only one face 
	
		// hit point and normal
		GlusVector v;
		glusRayPos(&ray, t1, &v);
		_inter->Hits[num].HitPoint = v;
		glusTransformVector(&_s->Transform, &_inter->Hits[num].HitPoint);
		_inter->Hits[num].HitNormal = v; _inter->Hits[num].HitNormal.V = 0;
		glusTransformVector(&_s->Transform, &_inter->Hits[num].HitNormal);
		glusNormalize(&_inter->Hits[num].HitNormal);

		num = 1; // have a hit
	}

	/*
	*	t2
	*/
	double t2 = (-B + dis_root) / A;
	if (t2 > Glus_Zero) // is hit in front of eye
	{
		_inter->Hits[num].hitTime = t2;
		_inter->Hits[num].isEnter = false;
		_inter->Hits[num].FaceID = 0;	// for shpere , only one face 

		// hit point and normal
		GlusVector v;
		glusRayPos(&ray, t2, &v);
		_inter->Hits[num].HitPoint = v;
		glusTransformVector(&_s->Transform, &_inter->Hits[num].HitPoint);
		_inter->Hits[num].HitNormal = v; _inter->Hits[num].HitNormal.V = 0;
		glusTransformVector(&_s->Transform, &_inter->Hits[num].HitNormal);
		glusNormalize(&_inter->Hits[num].HitNormal);

		num++; // another hit
	}

	_inter->numHits = num;
	_inter->HitObject = _s;
	glusLog("\nSphere hit.");
	return true;
}

static real Tappered_Cylinder_S = 1;
void glusTapperedCylinderS(real s)
{
	if (s > 1 || s < 0)	return;	// check for s
	Tappered_Cylinder_S = s;
}
/*
 *	hit for tappered cylinder
 */
bool
glusHitTapperedCylinder(PGlusShape _s, PGlusRay _r, PGlusIntersect _inter)
{
	assert(_s && _r && _inter);

	_inter->numHits = 0;

	/*
	*	get the generic ray
	*/
	GlusRay  ray = *_r;
	glusTransformInvVector(&_s->Transform, &ray.Point);
	glusTransformInvVector(&_s->Transform, &ray.Direction);

	/*
	*	compute discrim
	*/
#define RD(field) (ray.Direction.field)	// shortcut for ray direction
#define RP(field) (ray.Point.field)		// shortcut for ray point
#define S			Tappered_Cylinder_S
	real	A, B, C, d, F;
	d = (S - 1)* RD(Z);
	F = 1 + (S - 1) * RP(Z);
	A = sqr(RD(X)) + sqr(RD(Y)) - d * d;
	B = sqr(RP(X)) + sqr(RP(Y)) - F * d;
	C = sqr(RP(X)) + sqr(RP(Y)) - F * F;

	int num = 0;	// number for hits

	/*
	*	hit wall
	*/
	real dis = B *B - A*C;
	if (dis > 0.0)
	{
		real dis_root = sqrt(dis);
		real t1, t2, z_hit;

		/*
		 *	earlier hit
		 */
		t1 = (-B - dis_root) / A;	
		z_hit = RP(Z) + RD(Z) * t1;
		if (t1 > Glus_Zero && z_hit <= 1.0 && z_hit >= 0.0)
		{
			_inter->Hits[num].hitTime = t1;
			_inter->Hits[num++].FaceID = 0;	// hit at wall
		}

		/*
		*	later hit
		*/
		t2 = (-B + dis_root) / A;
		z_hit = RP(Z) + RD(Z) * t2;
		if (t2 > Glus_Zero && z_hit <= 1.0 && z_hit >= 0.0)
		{
			_inter->Hits[num].hitTime = t2;
			_inter->Hits[num++].FaceID = 0;	// hit at wall
		}
	}

	/*
	 *	base, z=0
	 */
	real tb = -RP(Z) / RD(Z);
	real x = RP(X) + RD(X)*tb, y = RP(Y) + RD(Y)*tb;
	real sq = sqr(x) + sqr(y);
	if (tb > Glus_Zero && sq < 1)
	{
		_inter->Hits[num].hitTime = tb;
		_inter->Hits[num++].FaceID = 1;	// hit at base
	}

	/*
	*	cap, z=1
	*/
	real tc = (1-RP(Z)) / RD(Z);
	if (tc > Glus_Zero && sqr(RP(X) + RD(X)*tc) + sqr(RP(Y) + RD(Y)*tc) < sqr(S))
	{
		_inter->Hits[num].hitTime = tc;
		_inter->Hits[num++].FaceID = 2;	// hit at cap
	}

	if (num == 0)	return false;	// missed everything,or behind the eye


	/*
	 *	handle all hits
	 */
	if (num == 1)	// eye inside cylinder, only have the exiting hit
	{
		_inter->Hits[0].isEnter = true;
	}
	else			// two hits, first must be entering
	{
		/*
		 *	sort the two hits
		 */
		if (_inter->Hits[0].hitTime > _inter->Hits[1].hitTime)
		{
			/*
			 *	only need swap hit-time and face-id
			 */
			real tmpt = _inter->Hits[0].hitTime;
			_inter->Hits[0].hitTime = _inter->Hits[1].hitTime;
			_inter->Hits[1].hitTime = tmpt;

			int tmpf = _inter->Hits[0].FaceID;
			_inter->Hits[0].FaceID = _inter->Hits[1].FaceID;
			_inter->Hits[1].FaceID = tmpf;
		}

		_inter->Hits[0].isEnter = true; _inter->Hits[1].isEnter = false;
	}

	/*
	 *	compute the point and normal
	 */
	for (int i = 0; i < num;i++)
	{
		GlusVector v;
		int f;

		// point
		glusRayPos(&ray, _inter->Hits[i].hitTime, &v);
		_inter->Hits[i].HitPoint = v; 
		glusTransformVector(&_s->Transform, &_inter->Hits[i].HitPoint);


		/*
		 *	normal
		 */
		f = _inter->Hits[i].FaceID;
		if (f == 0)	// wall
			glusV(v.X, v.Y, -S *(1 + S*v.Z), &_inter->Hits[i].HitNormal);
		else if (f==1)	// base
			glusV(0,0,-1, &_inter->Hits[i].HitNormal);
		else
			glusV(0,0,1, &_inter->Hits[i].HitNormal);
		glusTransformVector(&_s->Transform, &_inter->Hits[i].HitNormal);
		glusNormalize(&_inter->Hits[num].HitNormal);


	}
#undef S
#undef RP
#undef RD
	_inter->HitObject = _s;
	_inter->numHits = num;

	glusLog("\nTappered cylinder hit.");

	return true;
}


bool
glusHitCylinder(PGlusShape _s, PGlusRay _r, PGlusIntersect _inter)
{
	assert(_s && _r && _inter);

	Tappered_Cylinder_S = 1;
	return glusHitTapperedCylinder(_s, _r, _inter);
}


bool
glusHitCone(PGlusShape _s, PGlusRay _r, PGlusIntersect _inter)
{
	assert(_s && _r && _inter);

	Tappered_Cylinder_S = 0;
	return glusHitTapperedCylinder(_s, _r, _inter);
}

void cube_normal(int i,PGlusVector n)
{
	switch (i)
	{
	case 0:glusV(0, 1, 0, n); break;
	case 1:glusV(0, -1,0, n); break;
	case 2:glusV(1, 0, 0, n); break;
	case 3:glusV(-1,0, 0, n); break;
	case 4:glusV(0, 0, 1, n); break;
	case 5:glusV(0, 0,-1, n); break;
	default:
		break;
	}
}
bool
glusHitCube(PGlusShape _s, PGlusRay _r, PGlusIntersect _inter)
{
	assert(_s && _r && _inter);

	/*
	 *	get the generic ray
	 */
	GlusRay ray = *_r;
	glusTransformInvVector(&_s->Transform, &ray.Point);
	glusTransformInvVector(&_s->Transform, &ray.Direction);

#define RD(field) (ray.Direction.field)	// shortcut for ray direction
#define RP(field) (ray.Point.field)		// shortcut for ray point

	real	denom, numer;
	real	t_hit, t_in = -100000.0, t_out = 100000;
	int		fa_in, fa_out;

	for (int i = 0; i < 6;i++)
	{
		/*
		 *	compute denom,numer
		 */
		switch (i)
		{
		case 0:numer = 1 - RP(Y); denom = RD(Y);	break;
		case 1:numer = 1 + RP(Y); denom = -RD(Y);	break;
		case 2:numer = 1 - RP(X); denom = RD(X);	break;
		case 3:numer = 1 + RP(X); denom = -RD(X);	break;
		case 4:numer = 1 - RP(Z); denom = RD(Z);	break;
		case 5:numer = 1 + RP(Z); denom = -RD(Z);	break;
		}

		/*
		 *	compute hit-time
		 */
		if (fabs(denom) < Glus_Zero)	// ray is parallel
		{
			if (numer < 0)	return false;	// ray is out
			else;							// inside, needn't change
		}
		else
		{
			t_hit = numer / denom;
			if (denom > 0)	// leave
			{
				if (t_hit < t_out)	// a new earlier leave
				{
					t_out = t_hit;	fa_out = i;
				}
			}
			else // enter
			{
				if (t_hit > t_in) // a new later enter
				{
					t_in = t_hit; fa_in = i;
				}
			}
		}

		if (t_in >= t_out) return false;	// miss
	}

	int num = 0;
	if (t_in > Glus_Zero)	// is first hit in front eye
	{
		_inter->Hits[num].hitTime = t_in;
		_inter->Hits[num].FaceID  = fa_in;
		_inter->Hits[num].isEnter = true;
		// point
		glusRayPos(&ray, t_in, &_inter->Hits[num].HitPoint);
		glusTransformVector(&_s->Transform, &_inter->Hits[num].HitPoint);
		// normal
		cube_normal(fa_in, &_inter->Hits[num].HitNormal);
		glusTransformVector(&_s->Transform, &_inter->Hits[num].HitNormal);
		glusNormalize(&_inter->Hits[num].HitNormal);

		num++;
	}
	if (t_out > Glus_Zero)	// is first hit in front eye
	{
		_inter->Hits[num].hitTime = t_out;
		_inter->Hits[num].FaceID = fa_out;
		_inter->Hits[num].isEnter = false;
		// point
		glusRayPos(&ray, t_out, &_inter->Hits[num].HitPoint);
		glusTransformVector(&_s->Transform, &_inter->Hits[num].HitPoint);
		// normal
		cube_normal(fa_out, &_inter->Hits[num].HitNormal);
		glusTransformVector(&_s->Transform, &_inter->Hits[num].HitNormal);
		glusNormalize(&_inter->Hits[num].HitNormal);

		num++;
	}
	_inter->HitObject = _s;
	_inter->numHits = num;

	glusLog("\nCube hit.");
	return (num > 0);
#undef RP
#undef RD
}

bool
glusHitMesh(PGlusShape _s, PGlusRay _r, PGlusIntersect _inter)
{
	assert(_s && _r && _inter);

	_inter->numHits = 0;

	//get the generic ray

	GlusRay  ray = *_r;
	glusTransformInvVector(&_s->Transform, &ray.Point);
	glusTransformInvVector(&_s->Transform, &ray.Direction);


#define RD(field) (ray.Direction.field)	// shortcut for ray direction
#define RP(field) (ray.Point.field)		// shortcut for ray point

	real	denom, numer;
	real	t_hit, t_in = -100000.0, t_out = 100000;
	int		fa_in, fa_out;

	PGlusMesh mesh = _s->Extern;

	for (int f = 0; f < (int)mesh->FaceNum;f++)
	{
		/*
		 *	compute normal, point
		 */
		GlusVector diff, *normal, *point;
		normal = &mesh->Normals[mesh->Faces[f].FaceIDs->NormalID];
		point = &mesh->Points[mesh->Faces[f].FaceIDs->PointID];
		glusAdd(point, 1, &ray.Point, -1, &diff);

		/*
		*	compute denom,numer
		*/
		numer = glusDotPro(normal, &diff);
		denom = glusDotPro(normal, &ray.Direction);

		/*
		*	compute hit-time
		*/
		if (fabs(denom) < Glus_Zero)	// ray is parallel
		{
			if (numer < 0)	return false;	// ray is out
			else;							// inside, needn't change
		}
		else
		{
			t_hit = numer / denom;
			if (denom > 0)	// leave
			{
				if (t_hit < t_out)	// a new earlier leave
				{
					t_out = t_hit;	fa_out = f;
				}
			}
			else // enter
			{
				if (t_hit > t_in) // a new later enter
				{
					t_in = t_hit; fa_in = f;
				}
			}
		}

		if (t_in >= t_out) return false;	// miss
	}

	int num = 0;
	if (t_in > Glus_Zero)	// is first hit in front eye
	{
		_inter->Hits[num].hitTime = t_in;
		_inter->Hits[num].FaceID = fa_in;
		_inter->Hits[num].isEnter = true;
		// point
		glusRayPos(&ray, t_in, &_inter->Hits[num].HitPoint);
		glusTransformVector(&_s->Transform, &_inter->Hits[num].HitPoint);
		// normal
		_inter->Hits[num].HitNormal = mesh->Normals[mesh->Faces[fa_in].FaceIDs->NormalID];
		glusTransformVector(&_s->Transform, &_inter->Hits[num].HitNormal);
		glusNormalize(&_inter->Hits[num].HitNormal);

		num++;
	}
	if (t_out > Glus_Zero && t_out <50000.0)	// is first hit in front eye
	{
		_inter->Hits[num].hitTime = t_out;
		_inter->Hits[num].FaceID = fa_out;
		_inter->Hits[num].isEnter = false;
		// point
		glusRayPos(&ray, t_out, &_inter->Hits[num].HitPoint);
		glusTransformVector(&_s->Transform, &_inter->Hits[num].HitPoint);
		// normal
		_inter->Hits[num].HitNormal = mesh->Normals[mesh->Faces[fa_in].FaceIDs->NormalID];
		glusTransformVector(&_s->Transform, &_inter->Hits[num].HitNormal);
		glusNormalize(&_inter->Hits[num].HitNormal);

		num++;
	}

	/*
	 *	check for no closed mesh
	 */
	_inter->HitObject = _s;
	_inter->numHits = num;

	glusLog("\nMesh hit.");
	return (num > 0);
#undef RP
#undef RD
}