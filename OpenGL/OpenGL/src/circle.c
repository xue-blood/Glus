#include "..\inc\glus.h"

bool
glusCircleVertex2f(
_In_	GlusVertex2f *_pa,
_In_	GlusVertex2f *_pb,
_In_	GlusVertex2f *_pc,
_Out_	GlusCircle2f *_c)
{
	assert(_pa != NULL);
	assert(_pb != NULL);
	assert(_pc != NULL);
	assert(_c != NULL);

	//
	// compute vectors
	//
	GlusVector2f	va, vb, vc, vbn;
	GLfloat			t;

	glusInitVector2f(_pa, _pb, &vb);
	glusInitVector2f(_pc, _pa, &va);
	glusInitVector2f(_pb, _pc, &vc);
	glusVectorNormal2f(&vb, &vbn);

	//
	// check is the three point at one line
	//
	if (glusDotProduct(&va, &vbn) == 0)
		return false;
	//
	// compute the parameter
	//b	
	t = glusDotProduct(&vc, &va) /
		glusDotProduct(&va, &vbn) / 2;

	_c->Center.X = _pa->X + vb.X / 2 + vbn.X*t;
	_c->Center.Y = _pa->Y + vb.Y / 2 + vbn.Y*t;
	_c->Radius = glusLineDistance2f(&_c->Center, _pa);

	return true;

}
void 
glusDrawCircle2f(
_In_	GlusCircle2f	*_c)
{
	glBegin(GL_LINE_LOOP);
	{
		GLfloat	x, y;
		for (GLfloat f = 0; f < _2PI; f+=0.1)
		{
			//
			// compute x and y
			//
			x = _c->Center.X + _c->Radius*cosf(f);
			y = _c->Center.Y + _c->Radius*sinf(f);

			glVertex2f(x, y);
		}
	}
	glEnd();
}