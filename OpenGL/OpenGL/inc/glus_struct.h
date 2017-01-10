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
	GLfloat	R, G, B, A;
}GlusColor, *PGlusColor;


// define a function for draw 
// now it should use at the first
typedef	struct _GlusShape
{
	//void(*Draw)(); // change to no paramete. [7/9/2016 tld]
	void(*Draw)(pvoid);
	void(*Clear)(pvoid);	// add [8/31/2016 blue]
	pvoid Extern;	// a pointer to a struct

	GlusColor		Diffuse, Specular, Ambient;

	GlusTransform	Transform;

	char	Name[64];	// add [12/26/2016 xue]
	bool	IsHide;		// add [12/26/2016 xue]
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

/*
 *	projection type 
 */
// create [10/17/2016 blue]
typedef enum _GlusProjectionType
{
	ProjectPers,
	ProjectOrtho,
	ProjectOblique
}GlusProjectionType;

//  [7/9/2016 tld] add
//
//	for projection matrix
//
typedef struct _GlusProjection
{
	GLenum	Type;
	union {
		struct 
		{
			GLdouble	Left, Right, Bottom, Top;
			GLdouble	Dx, Dy, Dz;			// for oblique projection
		}Ortho; 
		struct 
		{
			GLdouble	AngleView, AspectRation;
		}Persp; 
	};
	GLdouble	Near, Far;
	
}GlusProjection, *PGlusProjection;

//  [7/9/2016 tld] add
//
//  for set camera
//
typedef struct _GlusCamera
{
	GlusVector	Eye, U, V, N;
}GlusCamera, *PGlusCamera;

typedef	struct _GlusTexture
{
	GlusLink	Link;
	PPixMap		Pixmap;
	Glusindex	ID;
}GlusTexture, *PGlusTexture;

typedef	struct _GlusScene
{
	GlusColor	Background;
	GlusColor	GlobalAmbient;
	GlusCamera	Camera;		//  [7/9/2016 tld] add
	GlusProjection	Projection;	// add [7/9/2016 tld]
	GlusLink	Shapes;
	GlusLink	Lights;
	GlusLink	Textures;
	// add [7/9/2016 tld]
	bool		EnableAxis;
	bool		IsLight;
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
	Glusindex	TextureID;
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
	PGlusVector	Points, Normals, Textures;
	PGlusFace	Faces;
	Glusnum		PointNum, NormalNum, TextureNum,FaceNum;
	Glusindex	TextureID;
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

typedef struct _Peano
{
	/*
	 *	main 
	 */
	char	Atom[64], FString[64], XString[64], YString[64];
	float	F, A;

	/*
	 *	other
	 */
	int		Level;
	float	Radom, StartAngle, Ratio;
}Peano, *PPeano;


typedef	enum _Array_Type
{
	Array_Rect,
	Array_Circle,
	Array_Curve
}Array_Type;

typedef	struct _Array
{
	Array_Type	Type;

	PGlusShape	Shape;

	union 
	{
		struct  
		{
			ubyte	n_X, n_Y, n_Z;
			float	d_X, d_Y, d_Z;
		}Rect;
	}Data;
}Array,*PArray;


typedef	struct _Fractal
{
	PGlusPoints		Points;
	float			MinLenSq, StdDev, Fractor;
	int				Seed;
}Fractal,*PFractal;


typedef	struct _ChaosGame
{
	uint		MaxLoop;
	GlusLink	Affines;
}ChaosGame, *PChaosGame;
typedef	struct _ChaosGameAffine
{
	GlusLink	Link;
	float		M11, M12, M21, M22, M13, M23;
	uint		Pr;
}ChaosGameAffine, *PChaosGameAffine;
#endif // !_GLUS_STRUCT_H
#endif // !_glus_struct_h
