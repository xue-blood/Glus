#include "glus_pre.h"
#include "glus_define.h"

/*
[2016.8.5]
add GlusMulSink
*/


#ifndef _glus_struct_h
#define _glus_struct_h
#ifndef _GLUS_STRUCT_H
#define _GLUS_STRUCT_H


typedef void 		*pvoid;

typedef GLdouble 	*pGLdouble;


// doubly link list
typedef struct _GlusLink
{
	struct _GlusLink*	FLink;
	struct _GlusLink*	BLink;
}GlusLink, *PGlusLink;

// singly link list
typedef struct _GlusSink
{
	struct _GlusSink*	Next;
}GlusSink, *PGlusSink;


typedef struct _GlusMulSink
{
	GlusSink	Sink;
	GlusSink	Data;
}GlusMulSink, *PGlusMulSink;

//
// use unity four diemous homogeneous form
// how to use the value is by youself
//
typedef struct _GlusVector
{
	GLdouble	X, Y, Z, V;
}GlusVector, *PGlusVector;


typedef	struct _GlusLine
{
	GlusVector	A, B;
}GlusLine, *PGlusLine;


typedef	struct _GlusRect
{
	GlusVector Left, Top, Right, Bottom;
}GlusRect, *PGlusRect;


typedef struct _GlusCircle
{
	GlusVector	Center;
	GLdouble	Radius;
}GlusCircle, *PGlusCircle;


typedef struct _GlusPlane
{
	GlusVector	A, B, C;
}GlusPlane, *PGlusPlane;

typedef struct _GlusPolygon
{
	GLint		Count;
	PGlusVector	Data;
}GlusPolygon, *PGlusPolygon;

typedef struct _GlusPolygonS
{
	GlusSink	Sink;
	GlusVector	Point;
}GlusPolygonS, *PGlusPolygonS;


typedef	char *		string;
typedef wchar_t *	wstring;

typedef enum _Glus_Intersect
{
	Intersect_Exsit = 1,
	Intersect_Not_Exsit = 0,
	Intersect_Parallel = -1,
	Intersect_Overlap = -2
}Glus_Intersect;

typedef struct _GlusTween
{
	PGlusPolygon	A, B;
	GLdouble		T, Interval;
	bool			IsPause,
		IsRepeat,
		IsReverse;
}GlusTween, *PGlusTween;
typedef	GLint	GlusTweenHandle;

typedef struct _GlusRay
{
	GlusVector	 Point;
	GlusVector	Direction;
}GlusRay, *PGlusRay;

typedef struct _GlusPAndN
{
	GlusVector	Point;
	GlusVector	Normal;
}GlusPAndN, *PGlusPAndN;

typedef struct _GlusPAndNS
{
	GlusSink	Sink;
	GlusPAndN	Data;
}GlusPAndNS, *PGlusPAndNS;

#endif // !_GLUS_STRUCT_H
#endif // !_glus_struct_h
