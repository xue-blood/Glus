#include "..\inc\glus.h"
#include <stdlib.h>

static	PGlusTween	Tweens = NULL;
static	GLint		Count = 0;
static	bool		Enable = false;

void
glusTween(
_In_	PGlusPolygon	_pa,
_In_	PGlusPolygon	_pb,
_In_	GLdouble		_t)
{
	assertp(_pa && _pb && _pa->Count && _pb->Count &&_pa->Data &&_pb->Data);
	assert(_pa->Count == _pb->Count);

	glBegin(GL_LINE_LOOP);
	{
		GlusVector	p;
		for (GLint i = 0; i < _pa->Count; i++)
		{
			glusVAdd(_pa->Data + i, 1 - _t, _pb->Data + i, _t, &p);

			glVertex3dv((GLdouble*)&p);
		}
	}
	glEnd();
}
void
glusTweenS(
_In_	PGlusPolygonS	_pa,
_In_	PGlusPolygonS	_pb,
_In_	GLdouble		_t)
{
	assertp(_pa && _pb);

	if (glusSinkIsEmpty(((PGlusSink)_pa)) || glusSinkIsEmpty(((PGlusSink)_pb)))
		return;

	glBegin(GL_LINE_LOOP);
	{
		PGlusPolygonS a, b;
		a = (PGlusPolygonS)((PGlusSink)_pa)->Next;
		b = (PGlusPolygonS)((PGlusSink)_pb)->Next;
		GlusVector	p;

		while(a)
		{
			glusVAdd(&a->Point, 1 - _t, &b->Point, _t, &p);

			glVertex3dv((GLdouble*)&p);
			 
			a = (PGlusPolygonS)a->Sink.Next;
			b = (PGlusPolygonS)b->Sink.Next;
		}
	}
	glEnd();
}


void
glusTweenTime(
int _id)
{

	if (!Enable)
		return;

	//assert(_id == (int)Tweens);


	
	for (GLint i = 0; i < Count; i++)
	{
		PGlusTween	p;
		p = Tweens+i;

		// is the polygon is valid
		if (!p->A || !p->B)
			continue;

		

		// draw the tween
		glusTween(p->A, p->B, p->T);

		glutPostRedisplay();

		// is the tween pause
		if (p->IsPause)
			continue;

		// is tween reverse
		if (p->IsReverse)
		{
			if (p->T > 1 || p->T < 0)
				p->Interval = -p->Interval;

			p->T += p->Interval;

			// is tween repeat
			if (!p->IsRepeat && (p->T <0))
				p->IsPause = true;

		}
		else	
		if (p->T > 1 )	if (!p->IsReverse)	p->T = 0;
						else				p->IsPause = true;
		else			p->T += p->Interval;
		
		
	}

	glutTimerFunc(30, glusTweenTime, (int)Tweens);
}
void
glusTweenTimeS(
int _id)
{

	if (!Enable)
		return;

	//assert(_id == (int)Tweens);


	for (GLint i = 0; i < Count; i++)
	{
		PGlusTween	p;
		p = Tweens + i;

		// is the polygon is valid
		if (!p->A || !p->B)
			continue;

		// draw the tween
		glusTweenS((PGlusPolygonS)p->A, (PGlusPolygonS)p->B, p->T);

		glutPostRedisplay();

		// is the tween pause
		if (p->IsPause)
			continue;

		// is tween reverse
		if (p->IsReverse)
		{
			if (p->T > 1 || p->T < 0)
				p->Interval = -p->Interval;

			p->T += p->Interval;

			// is tween repeat
			if (!p->IsRepeat && (p->T <0))
				p->IsPause = true;

		}
		else
		if (p->T > 1)	if (!p->IsReverse)	p->T = 0;
		else				p->IsPause = true;
		else			p->T += p->Interval;


	}

	glutTimerFunc(30, glusTweenTimeS, (int)Tweens);
}

GlusTweenHandle
glusTweenInit(
_In_	PGlusPolygon	_pa,
_In_	PGlusPolygon	_pb,
_In_	GLdouble		_interval,
_In_	bool			_isStart,
_In_	bool			_isReverse,
_In_	bool			_isRepeat)
{
	//
	// to find is there an invalid tween
	//
	GlusTweenHandle h;
	for (GLint i = 0; i < Count; i++)
	{
		if (Tweens[i].A)
			continue;

		//
		// find it
		// now we can store the new here
		//
		h = i;
		goto _Store_;
	}

	//
	// not find 
	// so we should add a new unit for it
	//
	PGlusTween	n;
	
	//
	// alloc memory for array buffer
	//
	if (Count == 0)
		n = malloc(sizeof(GlusTween));
	else
		n = realloc(Tweens, (Count+1)*sizeof(GlusTween));

	if (n)
		Tweens = n;
	else
		return -1;

	h = Count;
	Count += 1;

_Store_:

	Tweens[h].A = _pa;
	Tweens[h].B = _pb;
	Tweens[h].Interval	= _interval;
	Tweens[h].IsPause	= !_isStart;
	Tweens[h].IsRepeat	= _isRepeat;
	Tweens[h].IsReverse = _isReverse;
	Tweens[h].T = 0;


	// start the timer
	glutTimerFunc(30, glusTweenTime, (int)Tweens);
	Enable = true;

	return h;
}

GlusTweenHandle
glusTweenInitS(
_In_	PGlusPolygon	_pa,
_In_	PGlusPolygon	_pb,
_In_	GLdouble		_interval,
_In_	bool			_isStart,
_In_	bool			_isReverse,
_In_	bool			_isRepeat)
{
	//
	// to find is there an invalid tween
	//
	GlusTweenHandle h;
	for (GLint i = 0; i < Count; i++)
	{
		if (Tweens[i].A)
			continue;

		//
		// find it
		// now we can store the new here
		//
		h = i;
		goto _Store_;
	}

	//
	// not find 
	// so we should add a new unit for it
	//
	PGlusTween	n;

	//
	// alloc memory for array buffer
	//
	if (Count == 0)
		n = malloc(sizeof(GlusTween));
	else
		n = realloc(Tweens, (Count + 1)*sizeof(GlusTween));

	if (n)
		Tweens = n;
	else
		return -1;

	h = Count;
	Count += 1;

_Store_:

	Tweens[h].A = _pa;
	Tweens[h].B = _pb;
	Tweens[h].Interval = _interval;
	Tweens[h].IsPause = !_isStart;
	Tweens[h].IsRepeat = _isRepeat;
	Tweens[h].IsReverse = _isReverse;
	Tweens[h].T = 0;


	// start the timer
	glutTimerFunc(30, glusTweenTimeS, (int)Tweens);
	Enable = true;

	return h;

}
void
glusTweenClear(
_In_	GlusTweenHandle	_h)
{
	//
	// set the tween become invalid
	// but we shouldn't clear the memory
	//
	Tweens[_h].A = NULL;
}

void
glusTweenPause(
_In_	GlusTweenHandle _h)
{
	assert(_h < Count);
	Tweens[_h].IsPause = true;
}

void
glusTweenContinue(
_In_	GlusTweenHandle _h)
{
	assert(_h < Count);
	Tweens[_h].IsPause = false;
}

void
glusTweenReset(
_In_	GlusTweenHandle _h)
{
	assert(_h < Count);

	Tweens[_h].T = 0;
}
void
glusTweenDelete()
{
	//
	// delete all tweens and free the memory
	//
	if (Tweens)
	{

		free(Tweens);
		Tweens = 0, Count = 0;
	}

	Enable = false;

	glutPostRedisplay();
}

