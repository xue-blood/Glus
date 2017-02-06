#include "../inc/glus.h"

void
chaos_affine(PChaosGame	_p,PGlusVector _point)
{
	assert(_p);

	/*
	 *	find a random affine
	 */
	uint r = rand() % 100;
	
	PChaosGameAffine affine = (PChaosGameAffine)_p->Affines.BLink;
	while (!glusLinkIsHead(affine, &_p->Affines))
	{
		if (r <= affine->Pr) break; //! there must use '<='
		affine = (PChaosGameAffine)affine->Link.BLink;
	}

	/*
	 *	transform point
	 */
	double x = _point->X, y = _point->Y;
#define P(n) _point->n
#define A(n) affine->n
	P(X) = A(M11)* x + A(M12)* y + A(M13); normalize(&_point->X);
	P(Y) = A(M21)* x + A(M22)* y + A(M23); normalize(&_point->Y);
#undef A
#undef P

}

void
glusChaosGame(
_In_	PChaosGame	_p)
{
	assert(_p);

	GlusVector	point = { 0 };

	glPointSize(1);
	glEnable(GL_LIGHTING);
	glBegin(GL_POINTS);
	for (uint i = 0; i < _p->MaxLoop;i++)
	{
		chaos_affine(_p,&point);
		glVertex2d(point.X, point.Y); 
	}
	glEnd();
}

PChaosGame
glusChaosGameLoad(
_In_	FILE *	_file)
{
	assert(_file);

	glusAlloc(p, ChaosGame);
	glusLinkInit(&p->Affines);

	glusScanf(_file,"%d", &p->MaxLoop);	// load max-loop-count

	int n;
	glusScanf(_file, "%d", &n);	// the count of transform

	PChaosGameAffine l;
	float	pr, tpr = 0;
	for (int i = 0; i < n;i++)
	{
		glusAllocex(l, ChaosGameAffine, 1, goto _chaos_game_load_failed_);

		
		glusScanf(_file, "%f %f %f %f %f %f %f", &l->M11, &l->M12, &l->M21, &l->M22, &l->M13, &l->M23, &pr);
		tpr += pr;
		l->Pr = tpr * 100;  // convert float to interge

		glusLinkInsertTail(&p->Affines, l);
	}

	return p;

_chaos_game_load_failed_:
	glusLinkClear(&p->Affines);
	glusFree(p);
	return NULL;
}