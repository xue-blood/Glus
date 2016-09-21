#include "glus_pre.h"
#include "glus_define.h"

/*
//	[2016.8.5]
	add GlusMulSink

//  [8/14/2016 Tld]
	change GlusPolygon to GlusPoints
	add	GlusTransform	
//  [8/19/2016 xcv]
	add GlusColor
	add GlusLight,	GlusLights
	add GlusShape,	GlusShapes
	add GlusScene
*/


#ifndef _glus_struct_h
#define _glus_struct_h
#ifndef _GLUS_STRUCT_H
#define _GLUS_STRUCT_H

typedef enum _Glus_Status
{
	Glus_Status_Memory_Allocate_Fail	= -1,
	Glus_Status_Success = 0
}Glus_Status;

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

typedef struct _GlusLinks
{
	GlusLink	Link;
	GlusLink	Data;
}GlusLinks, *PGlusLinks;
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

typedef struct _GlusPoints
{
	GlusLink	Link;
	GlusVector	Point;
}GlusPoints, *PGlusPoints;

typedef struct _GlusPointsS
{
	GlusSink	Sink;
	GlusVector	Point;
}GlusPointsS, *PGlusPointsS;



typedef enum _Glus_Intersect
{
	Intersect_Exsit = 1,
	Intersect_Not_Exsit = 0,
	Intersect_Parallel = -1,
	Intersect_Overlap = -2
}Glus_Intersect;

typedef struct _GlusTween
{
	PGlusPoints	A, B;
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

typedef struct _GlusTransform
{
	GLdouble	Sx, Sy, Sz;		// scale
	GLdouble	Dx, Dy, Dz;		// translate
	GLdouble	Angle;			// rotate
	GLdouble	Ax, Ay, Az;		
}GlusTransform, *PGlusTransform;

typedef struct _GlusColor
{
	GLclampd	R, G, B, A;
}GlusColor, *PGlusColor;


// define a function for draw 
// now it should use at the first
typedef	struct _GlusShape
{
	//void(*Draw)(); // change to no paramete. [7/9/2016 tld]
	void(*Draw)(pvoid);
	void(*Clear)(pvoid);	// add [8/31/2016 blue]
	pvoid Extern;	// a pointer to a struct

	GlusColor		Diffuse;
	GlusTransform	Transform;
}GlusShape, *PGlusShape;

typedef struct _GlusShapes
{
	GlusLink	Link;
	GlusShape	Shape;		//  [7/9/2016 tld]	: change from GlusShape to PGlusShape
}GlusShapes,*PGlusShapes;

typedef struct _GlusLight
{
	GlusVector	Position;
	GlusColor	Diffuse;
	GLenum		Type;
}GlusLight, *PGlusLight;

typedef struct _GlusLights
{
	GlusLink	Link;
	GlusLight	Light;
}GlusLights, *PGlusLights;

//  [7/9/2016 tld] add
//
//	for projection matrix
//
typedef struct _GlusProjection
{
	GLdouble	Left,	Right,
				Bottom, Top,
				Near,	Far;
}GlusProjection, *PGlusProjection;

//  [7/9/2016 tld] add
//
//  for set camera
//
typedef struct _GlusCamera
{
	GLdouble	EyeX,		EyeY,		EyeZ,
				CenterX,	CenterY,	CenterZ,
				UpX,		UpY,		UpZ;
}GlusCamera, *PGlusCamera;
typedef	struct _GlusScene
{
	GlusColor	Background;
	GlusCamera	Camera;		//  [7/9/2016 tld] add
	GlusProjection	Projection;	// add [7/9/2016 tld]
	GlusLink	Shapes;
	GlusLink	Lights;
	// add [7/9/2016 tld]
	bool		EnableAxis;	
	GLdouble	AxisLength;
}GlusScene, *PGlusScene;

//
// structure for mesh
//

// add [8/24/2016 tld]
typedef struct _GlusFaceIndex
{
	Glusindex	PointID;
	Glusindex	NormalID;
}GlusFaceIndex, *PGlusFaceIndex;

// Face work for Mesh
// add [8/24/2016 tld]
typedef struct _GlusFace
{
	PGlusFaceIndex	FaceIDs;
	Glusnum			FaceIDNum;
}GlusFace, *PGlusFace;

// Mesh
// add [8/24/2016 tld] : point and normal use array
// change [8/25/2016 tld] : face use array too 
typedef struct _GlusMesh
{	
	PGlusVector	Points,		Normals;
	PGlusFace	Faces;
	Glusnum		PointNum, NormalNum, FaceNum;
}GlusMesh, *PGlusMesh;
typedef struct _GlusMeshs
{
	GlusLink	Link;
	PGlusMesh	Mesh;
}GlusMeshs, *PGlusMeshs;

/*
 *	for mouse input
 */
typedef	struct _GlusMouse
{
	GlusLink	Link;
	pvoid	Custom;			// pointer for custom
	void(*glusMouse)(int _button, int _state, GLint x, GLint y, pvoid custom);	// function 
}GlusMouse, *PGlusMouse;
#endif // !_GLUS_STRUCT_H
#endif // !_glus_struct_h
