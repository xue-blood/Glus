#include "..\inc\glus.h"


void 
glusInitVector2f(
	GlusVertex2f *_v1,
	GlusVertex2f *_v2,
	GlusVector2f *_v)
{
	assert(_v1	!= NULL);
	assert(_v2	!= NULL);
	assert(_v	!= NULL);

	_v->X	=	_v2->X	-	_v1->X;
	_v->Y	=	_v2->Y	-	_v1->Y;

}
void
glusVectorNormal2f(
_In_ GlusVector2f *_org,
_Out_ GlusVector2f *_v)
{
	assert(_org != NULL);
	assert(_v	!= NULL);

	_v->X	=	-_org->Y;
	_v->Y	=	_org->X;
}

GLfloat
glusDotProduct(
_In_ GlusVector2f *_va,
_In_ GlusVector2f *_vb)
{
	assert(_va != NULL);
	assert(_vb != NULL);

	return _va->X * _vb->X + _va->Y*_vb->Y;
}
