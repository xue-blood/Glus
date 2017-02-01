#include "../inc/glus.h"


void
glusFract(
_In_	PGlusVector	_a,
_In_	PGlusVector	_b,
_In_	double		_std_dev,
_In_	double		_min_len_sq,
_In_	double		_fractor,
_In_	int			_seed)
{
	assertp(_a && _b);

	/*
	 *	is fract end
	 */
	double d = glusDistanceSq(_a, _b);
	if (d< _min_len_sq)
	{
		glusLineTo(_b->X, _b->Y, _b->Z);
		return;
	}

	_std_dev *= _fractor;

	/*
	 *	use gaussian random
	 */
	double	t = 0;
	srand(_seed);
	for (int i = 0; i < 12; i++) t += rand() / 32768.0;
	t = (t - 6)*_std_dev;

	GlusVector	c;
	c.X = 0.5*(_a->X + _b->X) - t*(_b->Y - _a->Y);
	c.Y = 0.5*(_a->Y + _b->Y) + t*(_b->X - _a->X);
	c.Z = 0.5*(_a->Z + _b->Z);
	c.V = 1;

	glusFract(_a, &c, _std_dev, _min_len_sq, _fractor,_seed);
	glusFract(&c, _b, _std_dev, _min_len_sq, _fractor,_seed);
}

void
glusFractDraw(
_In_	PFractal	_p)
{
	assert(_p);

	if (glusLinkLength((PGlusLink)_p->Points) < 2)	return;

	PGlusPoints	a = (PGlusPoints)_p->Points->Link.BLink;
	glusMoveTo(a->Point.X, a->Point.Y, a->Point.Z);
	while (!glusLinkIsHead(a,_p->Points))
	{
		PGlusPoints	b = (PGlusPoints)a->Link.BLink;
		glusFract(&a->Point, &b->Point, _p->StdDev, _p->MinLenSq, _p->Fractor, _p->Seed);

		a = b;
	}
}
