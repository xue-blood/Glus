#include "glus_struct.h"


#ifndef _glus_func_h
#define _glus_func_h
#ifndef _GLUS_FUNC_H
#define _GLUS_FUNC_H

// user should define the function by themself
void	glusInit();
// 


//
// see init.c
// 
void	glusInitWin(GLint	_left, _In_ GLint	_top, _In_ GLint	_width, _In_ GLint	_height, _In_ string _name, _In_ GLenum _mode);

void	normalize(pGLdouble data);
// point
void	glusP(_In_	GLdouble	x, _In_		GLdouble	y, _In_		GLdouble	z, _Out_	PGlusVector		o);
#define glusP3d		glusP
#define glusP3f		glusP
#define glusP3i		glusP
#define glusP2d(x,y,o)		glusP(x,y,0,o)
#define glusP2f		glusP2d
#define glusP2i		glusP2d

// vector
void	glusV(_In_	GLdouble	_x, _In_	GLdouble	_y, _In_	GLdouble	_z,_Out_	PGlusVector _o);
#define glusV3d		glusV
#define glusV3f		glusV
#define glusV3i		glusV
#define glusV2d(x,y,o)	glusV(x,y,0,o)
#define glusV2f					glusV2d		
#define glusV2i					glusV2d		


// line
void	glusL(_In_ GLdouble ax, _In_ GLdouble ay, _In_ GLdouble az, _In_ GLdouble bx, _In_ GLdouble by, _In_ GLdouble bz, _Out_ PGlusLine o);
#define glusL3f		glusL
#define glusL3i		glusL
#define glusL2d(ax,ay,bx,by,o) glusL(ax,ay,0,bx,by,0,o)
#define glusL2f		glusL2d
#define glusL2i		glusL2d
#define glusL3dv(i,o)	glusL((i)->A.X,	(i)->A.Y,	(i)->A.Z,	(i)->B.X,	(i)->B.Y,	(i)->B.Z,	o)
#define glusL3fv		glusL3dv	
#define glusL3iv		glusL3dv	
#define glusL2dv(i,o)	glusL((i)->A.X,	(i)->A.Y,	0,		(i)->B.X,	(i)->B.Y,	0,		o)
#define glusL2fv		glusL2dv
#define glusL2iv		glusL2dv

// plane
void	gluspd(_In_ GLdouble ax, _In_ GLdouble ay, _In_ GLdouble az, _In_ GLdouble bx, _In_ GLdouble by, _In_ GLdouble bz, _In_ GLdouble cx, _In_ GLdouble cy, _In_ GLdouble cz, _Out_ PGlusPlane o);
#define gluspf	gluspd
#define gluspi	gluspd

#define gluspv(i,o) gluspd((i)->A.X,	(i)->A.Y,	(i)->A.Z,	(i)->B.X,	(i)->B.Y,	(i)->B.Z,	(i)->C.X,	(i)->C.Y,	(i)->C.Z,	o)
#define gluspdv		gluspv
#define gluspfv		gluspv
#define gluspiv		gluspv


//
// point
//
#define glusPoint2f(x,y)	{.X = x,.Y = y,.Z = 0,.V=1 }
#define glusPoint3f(x,y,z)	{.X = x,.Y = y,.Z = z,.V=1 }
#define glusPoint2fv(p)		{.X = p->X, .Y=p->Y, .Z=0,	 .V=1}
#define glusPoint3fv(p)		{.X = p->X, .Y=p->Y, .Z=p->Z,.V=1}

//
// vector 
//
#define glusVector2f(ax,ay,		bx,by)		{.X = bx - ax, .Y = by - ay, .Z = 0,	.V=0 }
#define glusVector3f(ax,ay,az,	bx,by,bz)	{.X = bx - ax, .Y = by - ay, .Z = bz - az,.V=0 }
#define glusVector2fv(pa,pb) {.X = pb->X - pa->X,.Y = pb->Y - pa->Y,.Z =0,				.V=0 }	
#define glusVector3fv(pa,pb) {.X = pb->X - pa->X,.Y = pb->Y - pa->Y,.Z = pb->Z - pa->Z, .V=0 }	

#define glusVPerpende(v) {.X = v->Y, .Y = -v->X, .Z = v->Z, .Z =0}
//
// line
//
#define glusLine2f(ax,ay,	bx,by)		{.A = glusPoint2f(ax,ay),	 .B = glusPoint2f(bx,by)}
#define glusLine3f(ax,ay,az,bx,by,bz)	{.A = glusPoint3f(ax,ay,az), .B = glusPoint2f(bx,by,bz)}
#define glusLine2fv(pa,pb)	{.A=glusPoint2fv(pa), .B=glusPoint2fv(pb)}
#define glusLine3fv(pa,pb)	{.A=glusPoint3fv(pa), .B=glusPoint3fv(pb)}

//
// plane
//
#define glusPlanef(ax,ay,az,bx,by,bz,cx,cy,cz)	{.A = glusPoint3f(ax,ay,az), .B = glusPoint2f(bx,by,bz), .C=glusPoint2f(cx,cy,cz)}
#define glusPlanefv(pa,pb,pc)	{.A=glusPoint3fv(pa), .B=glusPoint3fv(pb),.C=glusPoint3fv(pc)}

// 
// see point.c
// 
bool	glusPIs3PointOnLine(_In_ PGlusVector _pa, _In_ PGlusVector _pb, _In_ PGlusVector _pc);
bool	glusPIsInPolygonS(_In_	PGlusVector		_point, _In_	PGlusSink		_polygon);
bool	glusPIsInPolygon(_In_	PGlusVector		_point, _In_	PGlusLink		_polygon);

//
// see vector.c
//
void	glusVAdd(_In_	GlusVector *_va, _In_	GLdouble	_pa, _In_	GlusVector *_vb, _In_	GLdouble	_pb, _Out_	GlusVector *_vo);
#define glusVFromPoint(PointA,PointB,Vector) glusVAdd(PointA,-1,PointB,1,Vector)
void	glusVUnit(_Inout_ PGlusVector _v);
void	glusVNormal(_In_ GlusVector *_in, _Out_ GlusVector *_out);
void	glusVNormalize(_Inout_ PGlusVector _v);
#define glusVExtern(Vector,parameter) (Vector)->X = parameter*(Vector)->X,(Vector)->Y = parameter*(Vector)->Y,(Vector)->Z = parameter*(Vector)->Z;
#define glusVOpposite(Vector)	glusVExtern(Vector,-1)
GLdouble	glusVDotPro(_In_ GlusVector *_va, _In_ GlusVector *_vb);
GLdouble	glusVLength(_In_ GlusVector *_v);
GLdouble	glusPDistance(_In_ GlusVector *_pa, _In_ GlusVector *_pb);
void	glusCroPro(_In_ PGlusVector _va, _In_ PGlusVector _vb, _Out_ PGlusVector _vo);

//
// see line.c
//
void	glusDrawCoord();
void	glusLDraw(_In_ PGlusLine _pLine);
Glus_Intersect	glusLIntersect(_In_ PGlusVector _laa, _In_	PGlusVector	_lab, _In_ PGlusVector _lba, _In_	PGlusVector	_lbb, _Out_ PGlusVector _p);
#define	glusLMiddle(PointA,PointB,PointM) glusVAdd(PointA,0.5,PointB,0.5,PointM)
#define glusLFormPToR()
#define glusLFormPToN()
#define glusLFormRToP(linev,linep) (linep)->A = (linev)->Position,glusVAdd(&(linev)->Position,1,&(linev)->Direction,1,&(linep)->B);
#define glusLFormRToN()
#define glusLFormNToP()
#define glusLFormNToV()

//
// see ray.c
// 
void	glusRDraw(_In_ PGlusRay _pRay);
GLdouble	glusRHit(_In_ PGlusRay _r, _In_ PGlusVector _n, _In_ PGlusVector _p);
void	glusRReflecte(_Inout_	PGlusRay	_ray, _In_	PGlusVector	_normal, _Out_	PGlusVector	_direction);

GLdouble	glusRTrace2D(_In_	PGlusRay	_ray, _In_	PGlusLink	_head, _Out_	PGlusRay	_nRay);
void	glusRTraces2D(_Inout_	PGlusRay		_ray, _In_	PGlusLinks	_head);

GLdouble	glusRHit2DS(_In_	PGlusRay	_ray,_In_	PGlusSink	_head,_Out_	PGlusRay	_nRay);
void	glusRHitMul2DS(_Inout_	PGlusRay		_ray,_In_	PGlusMulSink	_head);

void	glusRHitS(_Inout_ PGlusRay _ray, _In_ PGlusSink _head);

//
// see circle.c
//
void	glusDrawCircle(_In_	GlusCircle	*_c);
bool	glusCExTri(_In_	PGlusVector	_pa, _In_	PGlusVector	_pb, _In_	PGlusVector	_pc, _Out_	PGlusCircle _c);
bool	glusCInTri(_In_	PGlusVector	_pa, _In_	PGlusVector	_pb, _In_	PGlusVector	_pc, _Out_	PGlusCircle _c);
bool	glusCInTri(_In_	PGlusVector	_pa, _In_	PGlusVector	_pb, _In_	PGlusVector	_pc, _Out_	PGlusCircle _c);
bool	glusCNinePoint(_In_	PGlusVector	_pa, _In_	PGlusVector	_pb, _In_	PGlusVector	_pc, _Out_	PGlusCircle _c);
bool	glusCGetTangentPoints(_In_ PGlusVector _pa, _In_ PGlusVector	_pb, _In_ PGlusVector _pc, _Out_ PGlusVector _pr, _Out_ PGlusVector _ps, _Out_ PGlusVector _pt);
bool	glusCGetAltitudeFeet(_In_ PGlusVector _pa, _In_ PGlusVector	_pb, _In_ PGlusVector _pc, _Out_ PGlusVector _pr, _Out_ PGlusVector _ps, _Out_ PGlusVector _pt);
bool	glusCGetMiddlePoints(_In_ PGlusVector _pa, _In_ PGlusVector	_pb, _In_ PGlusVector _pc, _Out_ PGlusVector _pr, _Out_ PGlusVector _ps, _Out_ PGlusVector _pt);

//
// see plane.c
//
Glus_Intersect	glusPlaneIntersect(_In_	PGlusPlane	_pa, _In_	PGlusPlane	_pb, _Out_	PGlusLine	_l);

//
// see polygon.c
//
void	glusDrawPolygon(_In_	PGlusLink	_p);
void	glusDrawPolygonS(_In_	PGlusSink	_p);
void	glusDrawMulPolygonS(_In_	PGlusSink	_p);
void	glusDrawPolygons(_In_	PGlusLink	_p);

void	glusPolygonUnion(_In_	PGlusLink	_pa, _In_	PGlusLink	_pb, _Out_	PGlusLink	_po);

// see tween.c
void	glusTween(_In_	PGlusLink	_pa, _In_	PGlusLink	_pb, _In_	GLdouble		_t);
GlusTweenHandle	glusTweenInit(_In_	PGlusPolygon _pa, _In_ PGlusPolygon _pb, _In_ GLdouble interval, _In_ bool isStart, _In_ bool	isReverse, _In_ bool isRepeat);
void	glusTweenClear(GlusTweenHandle		h);
void	glusTweenPause(GlusTweenHandle		h);
void	glusTweenContinue(GlusTweenHandle	h);
void	glusTweenReset(GlusTweenHandle		h);
void	glusTweenDelete();
void	glusTweenS(_In_ PGlusPolygonS _pa, _In_ PGlusPolygonS _pb, _In_ GLdouble _t);
GlusTweenHandle glusTweenInitS(_In_ PGlusPolygon _pa, _In_ PGlusPolygon _pb, _In_ GLdouble _interval, _In_ bool _isStart, _In_ bool _isReverse, _In_ bool _isRepeat);

//
// see mouse.c
//
bool	glusMousePolygonS(_In_ int _x, _In_ int _y, _Out_ pvoid _po);


//
// see link.c
//

// doubly link
#define glusLink(Name)	GlusLink Name={.FLink = &Name,.BLink = &Name}
#define glusLinks(Name)	GlusLinks Name={.Data.FLink = &Name.Data,.Data.BLink = &Name.Data}
void	glusLinkInit(_Inout_ void* LinkHead);
#define glusLinkIsEmpty(LinkHead) ((LinkHead)->FLink == (LinkHead)->BLink)
#define glusLinkIsEnd(point,head) ((PGlusLink)(point) == (PGlusLink)(head))
GLint	glusLinkLength(_In_	PGlusLink	_linkHead);
void	glusLinkInsertHead(_Inout_ pvoid LinkHead, _Inout_ pvoid LinkNode);
void	glusLinkInsertTail(_Inout_ void* _linkHead, _Inout_ void* _linkNode);
pvoid	glusLinkRemoveHead(_Inout_ pvoid _linkHead);
pvoid	glusLinkRemoveTail(_Inout_ pvoid _linkHead);
bool	glusLinkRemoveEntry(_Inout_ pvoid _linkEntry);

// singly link
#define glusSink(Name) GlusSink Name = null;
#define glusMulSink(Name) GlusMulSink Name = null;
#define glusSinkInit(LinkHead) ((PGlusSink)LinkHead)->Next = NULL;
#define glusSinkIsEmpty(LinkHead) (!LinkHead->Next)
void	glusSinkPush(_Inout_ PGlusSink _linkHead, _In_ PGlusSink _linkEntry);
PGlusSink	glusSinkPop(_Inout_ PGlusSink _linkHead);
GLint	glusSinkLength(_In_ PGlusSink _linkHead);


#endif // !_GLUS_FUNC_H

#endif // !_glus_func_h



