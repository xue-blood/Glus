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
		glVertex3d(p.X, p.Y,0);
	}
	glEnd();
}