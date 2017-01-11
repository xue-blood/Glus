#include "../inc/glus.h"

float _Alpha, _Step=0.01;

void 
glusDissolve(PPixMap a, PPixMap b)
{
	assert(a && b);

	pixSetAlpha(a, 1 - _Alpha);
	pixChromaKey(a, 0, 0, 0);
	pixBlend(a);

	pixSetAlpha(b, _Alpha);
	pixChromaKey(b, 0,248,248);
	pixDraw(b);

	_Alpha += _Step;

	if (_Alpha > 1 && _Step > 0)	_Step = -0.01, _Alpha = 1;
	if (_Alpha < 0 && _Step < 0)	_Step = 0.01, _Alpha = 0;

}