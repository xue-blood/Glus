#include "../inc/glus.h"

PGlusBool
glusBoolLoad(FILE * _file, PGlusScene _scene)
{
	assert(_file);

	
	char name[64];
	PGlusShape	A, B;
	GlusBoolType t;

	/*
	 *	load object A
	 */
	// load name
	glusSkipSpace(_file);
	glusScanf(_file, "%s", name, _countof(name));
	name[_countof(name) - 1] = 0;

	// load object
	A = glusSceneGetShapeByName(_scene, name, _countof(name));
	if (!A)	return NULL;

	/*
	 *	load type
	 */
	glusSkipSpace(_file);
	glusScanf(_file, "%s", name, _countof(name));
	
	switch (name[0])
	{
	case '|':	t = Bool_Union;		break;
	case '&':	t = Bool_Intersect;	break;
	case '-':	t = Bool_Differ;	break;
	default:	return NULL;
	}

	/*
	 *	load B
	 */
	glusSkipSpace(_file);
	glusScanf(_file, "%s", name, _countof(name));
	name[_countof(name) - 1] = 0;

	// load object
	B = glusSceneGetShapeByName(_scene, name, _countof(name));
	if (!B)	return NULL;

	glusAlloc(b, GlusBool);
	b->A = A;
	b->B = B;
	b->Type = t;

	glusLog("\nBoolean load success.");

	return b;
}

void
glusBoolDraw(PGlusBool _p)
{
	// just return,
	// because it's not can draw use generic way
	return;
}

void
glusBoolClear(pvoid _p)
{
	glusFree(_p);
}


bool
glusBoolHitUnion(PGlusBool b, PGlusRay _r, PGlusIntersect _intl, PGlusIntersect _intr, PGlusIntersect _o)
{
	if (!b->A->Hit(b->A, _r, _intl) && !b->B->Hit(b->B, _r, _intr))
		return false;

	if (!_o) // no need calc hit info
		return true;

}

bool
glusBoolHitInter(PGlusBool b, PGlusRay _r, PGlusIntersect _intl, PGlusIntersect _intr, PGlusIntersect _o)
{
	if (!b->A->Hit(b->A, _r, _intl) || !b->B->Hit(b->B, _r, _intr))
		return false;

	if (!_o) // no need calc hit info
		return true;

	int num = 0;
	
	// later in
	if (_intl->Hits[num].hitTime < _intr->Hits[num].hitTime)
	{
		memcpy_s(&_o->Hits[num], sizeof(GlusHitInfo), &_intr->Hits[num], sizeof(GlusHitInfo));
	}
	else
	{
		memcpy_s(&_o->Hits[num], sizeof(GlusHitInfo), &_intl->Hits[num], sizeof(GlusHitInfo));
	}
	num++;

	// early out
	if (_intl->Hits[num].hitTime > _intr->Hits[num].hitTime)
	{
		memcpy_s(&_o->Hits[num], sizeof(GlusHitInfo), &_intr->Hits[num], sizeof(GlusHitInfo));
	}
	else
	{
		memcpy_s(&_o->Hits[num], sizeof(GlusHitInfo), &_intl->Hits[num], sizeof(GlusHitInfo));
	}
	num++;

	_o->numHits = num;

	return true;
}
bool
glusBoolHitDiffer(PGlusBool b, PGlusRay _r, PGlusIntersect _intl, PGlusIntersect _intr,PGlusIntersect _o)
{
	if (!b->A->Hit(b->A, _r, _intl))	return false;
	if (b->B->Hit(b->B, _r, _intr))	return false;

	if (!_o) // no need calc hit info
		return true;

}

bool
glusBoolHit(PGlusShape _s, PGlusRay _r, PGlusIntersect _inter)
{
	assert(_s && _r);
	
	/*
	 *	get intersect for left and right
	 */
	GlusIntersect l, r;
	PGlusBool b = _s->Extern;

	switch (b->Type)
	{
	case Bool_Union:	return glusBoolHitUnion(b, _r, &l, &r, _inter);
	case Bool_Intersect:return glusBoolHitInter(b, _r, &l, &r, _inter);
	case Bool_Differ:	return glusBoolHitDiffer(b, _r, &l, &r, _inter);
	default:			return false;	// no support type
	}
	

	glusLog("\nboolean hit.");
}