#include <Glut\freeglut.h>
#include <assert.h>
#include "circle.h"
#include <stdbool.h>
#include <math.h>

#ifndef _GLUS_H
#ifndef _glus_h

#define _GLUS_H
#define _glus_h

//------------struct---------------
// 
// 2D 
// 

typedef struct _GlusVertex2i
{
	GLint	X, Y;
}GlusVertex2i;

typedef struct _GlusVertex2f
{
	GLfloat	X, Y;
}GlusVertex2f;

// 
// 3D 
// 

typedef struct _GlusVertex3i
{
	GLint	X, Y, Z;
}GlusVertex3i;

typedef struct _GlusVertex3f
{
	GLfloat	X, Y, Z;
}GlusVertex3f;

typedef GlusVertex2i		GlusVector2i;
typedef GlusVertex2f		GlusVector2f;
typedef GlusVertex3i		GlusVector3i;
typedef GlusVertex3f		GlusVector3f;

// 
// 2D 
// 
typedef	struct _GlusLine2i
{
	GlusVertex2i		A, B;
}GlusLine2i;


typedef	struct _GlusLine2f
{
	GlusVertex2f		A, B;
}GlusLine2f;


//
// 3D
// 
typedef	struct _GlusLine3i
{
	GlusVertex3i		A, B;
}GlusLine3i;


typedef	struct _GlusLine3f
{
	GlusVertex3f		A, B;
}GlusLine3f;

typedef	struct _GlusCircle2f
{
	GlusVertex2f		Center;
	GLfloat				Radius;
}GlusCircle2f;
//============struct===============


//------------Function---------------

// user should define the function by themself
void	glusInit();		
// 

void	glusInitWin(_In_ GLint	_left, _In_ GLint	_top, _In_ GLint	_width, _In_ GLint	_height, _In_ char	*_name, _In_ GLenum _mode);
void	glusInitVector2f(_In_ GlusVertex2f *v1, _In_ GlusVertex2f *v2, _Out_ GlusVector2f *v);

//
// vector 
//
void	glusVectorNormal2f(_In_ GlusVector2f *_org,_Out_ GlusVector2f *_v);
GLfloat glusDotProduct(_In_ GlusVector2f *_va,_In_ GlusVector2f *_vb);

//
// line
//
#define Glus_Line_Intersect				1
#define Glus_Line_Intersect_NotExsit	0
#define Glus_Line_Parallel				-1
#define Glus_Line_Parallel_Overlap		-2

#define glusLineMiddle2f(a,b,c)	\
	c->X = (a->X + b->X) / 2;	\
	c->Y = (a->Y + b->Y) / 2;
GLenum	glusIsLineIntersect2f(_In_ GlusVertex2f *_a,_In_ GlusVertex2f *_b,_In_ GlusVertex2f *_c,_In_ GlusVertex2f *_d,_Out_ GlusVertex2f *_v);
GLfloat glusLineDistance2f(_In_	GlusVertex2f	*_pa,_In_	GlusVertex2f	*_pb);
bool	glusLineHit2f(_In_ GlusLine2f	*_l,_In_ GlusVertex2f *_p,_In_	GlusVector2f	*_n,_Out_	GlusVertex2f	*_phit);
bool	glusCircleVertex2f(_In_	GlusVertex2f *_pa,_In_	GlusVertex2f *_pb,_In_	GlusVertex2f *_pc,_Out_	GlusCircle2f *_c);
void	glusDrawCircle2f(_In_	GlusCircle2f	*_c);
//============Function===============

#endif
#endif
