#include "../inc/glus.h"

GlusVector
bezier(
PGlusVector points,
int			n,
double		t)
{
	assert(points && n>=2);


	if (n-- == 2)
	{
		/*
		 *	compute interpolation between two point
		 */
		GlusVector	p;
		glusVAdd(points, 1 - t, points + 1, t, &p);
		return p;
	}

	/*
	 *	compute the new point
	 */
	GlusVector a[2];
	a[0] = bezier(points, n, t);
	a[1] = bezier(points + 1, n, t);

	// the final interpolation
	return bezier(a, 2, t);
}

void 
glusBezier(
_In_ PGlusPointsA	points, 
_In_ int			num)
{
	assert(points);

	double	step = 1.0 / num;

	glBegin(GL_LINE_STRIP);
	for (double t = 0; t < 1; t +=step)
	{
		GlusVector p = bezier(points->Points, points->Count, t);
		glVertex3d(p.X, p.Y,p.Z);
	}
	glEnd();
}


/*
 *	compute the N(k,m) for b-spline with knot vector at time t
 */
float 
bSpline(int k, int m, float t, float *knots)
{
	assert(knots);

	float denom1, denom2, sum = 0.0;

	if (m == 1)
		return (t >= knots[k] && t < knots[k + 1]); // 1 or 0

	denom1 = knots[k + m - 1] - knots[k];
	if (denom1 != 0.0)
		sum = (t - knots[k])* bSpline(k, m - 1, t, knots);

	denom2 = knots[k + m] - knots[k + 1];
	if (denom2 != 0.0)
		sum += (knots[k + m] - t)*bSpline(k + 1, m - 1, t,knots);


	return sum;
}

/*
 *	build standard knot vector for b-spline
 */
void
buildKnots(int m, int l, float *knots)
{
	assert(knots);

	if (m >l+1) return;	// too few control points

	for (int i = 0; i < l + m;i++)
	{
		if (i < m) knots[i] = 0.0;
		else if (i <= l) knots[i] = i - m + 1;
		else knots[i] = l - m + 2;
	}
}

/*
 *	compute the point for b-spline at t
 */
GlusVector 
curve(PGlusVector	points, int n,int m, double t,float *knots)
{
	GlusVector p = { 0, .V=1 };

	for (int i = 0; i < n;i++)
	{
		double nt = bSpline(i, m, t,knots);

		p.X += points[i].X*nt;
		p.Y += points[i].Y*nt;
		p.Z += points[i].Z*nt;
	}

	return p;
}

/*
 *	draw b-spline
 */
void 
glusBSpline(PGlusPointsA pa, int m, int num)
{
	/*
	 *	prepare knot vector
	 */
	float*knots;
	glusAllocex(knots, float, pa->Count + m,return);
	buildKnots(m, pa->Count, knots);

	GlusVector p;
	double t = 0;
	glBegin(GL_LINE_STRIP);
	while (t < 1)
	{
		p = curve(pa->Points, pa->Count, m, t, knots);
		glVertex3d(p.X, p.Y, p.Z);

		t += ((float)pa->Count + m - 1) / num;
	}
	glEnd();

	glusFree(knots);
}

void 
glusBSplineFunc(PGlusPointsA pa, int m, int num)
{
	float*knots;
	glusAllocex(knots, float, pa->Count + m, return);
	buildKnots(m, pa->Count, knots);

	
	for (int k = 0; k < (int)pa->Count;k++)
	{
		double t = 0;
		glBegin(GL_LINE_STRIP);
		for (int n = 0; n < num;n++)
		{
			double nt = bSpline(k, m, t, knots);
			glVertex2d(t, nt);
			
			t += ((double)pa->Count + m -1 ) / num;
		}
		glEnd();
	}

	glusFree(knots);
}