#include "glus_struct.h"

//  [7/1/2016 Tld]
//	add glusTiling

#ifndef _glus_func_h
#define _glus_func_h
#ifndef _GLUS_FUNC_H
#define _GLUS_FUNC_H

//
// glus.c
//
// user should define the function by themself
void	glusInit();
// 
void	glusLogLevel(_In_	int		_level);
#define glusLogex(level,msg,...)		fprintf_s(_Glus_Logs[level],msg,__VA_ARGS__)
#define glusLog(msg,...)				fprintf_s(_Glus_Logs[Glus_Log_Normal],msg,__VA_ARGS__)


void	glusFPS(int fps);

void	glusInitWin(GLint	_left, _In_ GLint	_top, _In_ GLint	_width, _In_ GLint	_height, _In_ str _name, _In_ GLenum _mode);

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
void	glusL(_In_ GLdouble ax, _In_ GLdouble ay, _In_ GLdouble az, _In_ GLdouble bx, _In_ GLdouble by, _In_ GLdouble bz, _Inout_ PGlusLine o);
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
void	gluspd(_In_ GLdouble ax, _In_ GLdouble ay, _In_ GLdouble az, _In_ GLdouble bx, _In_ GLdouble by, _In_ GLdouble bz, _In_ GLdouble cx, _In_ GLdouble cy, _In_ GLdouble cz, _Inout_ PGlusPlane o);
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
#define drawPoint(x,y)  {glBegin(GL_POINTS); glVertex2i(x, y); glEnd();}
bool	glusPIs3PointOnLine(_In_ PGlusVector _pa, _In_ PGlusVector _pb, _In_ PGlusVector _pc);
bool	glusPIsInPolygonS(_In_	PGlusVector		_point, _In_	PGlusSink		_polygon);
bool	glusPIsInPolygon(_In_	PGlusVector		_point, _In_	PGlusLink		_polygon);

//
// see vector.c
//
void	glusAdd(_In_	GlusVector *_va, _In_	GLdouble	_pa, _In_	GlusVector *_vb, _In_	GLdouble	_pb, _Out_	GlusVector *_vo);
#define glusVFromPoint(PointA,PointB,Vector) glusAdd((PointA),-1,(PointB),1,(Vector))
#define glusPAddV(p_point,p_vector,p_point_out) glusAdd( (p_point) ,1, (p_vector),1, (p_point_out))
void	glusUnit(_Inout_ PGlusVector _v);
void	glusNormal(_In_ GlusVector *_in, _Inout_ GlusVector *_out);
void	glusNormalize(_Inout_ PGlusVector _v);
#define glusVExtern(Vector,parameter) (Vector)->X = parameter*(Vector)->X,(Vector)->Y = parameter*(Vector)->Y,(Vector)->Z = parameter*(Vector)->Z;
#define glusVOpposite(Vector)	glusVExtern(Vector,-1)
GLdouble	glusDotPro(_In_ GlusVector *_va, _In_ GlusVector *_vb);
GLdouble	glusLength(_In_ GlusVector *_v);
GLdouble	glusDistanceSq(_In_ GlusVector *_pa, _In_ GlusVector *_pb);
#define		glusDistance(a,b) sqrt(glusDistanceSq(a,b))
void	glusCroPro(_In_ PGlusVector _va, _In_ PGlusVector _vb, _Out_ PGlusVector _vo);

#define glusVLengthSq(v)

//
// color
//
#define glusColor(pointer,r,g,b,a)	((pointer)->R = r),((pointer)->G = g),((pointer)->B = b),((pointer)->A = a)
//
//
// canvas
// work on 2d-mode
//
void	glusDrawCoord();
void	glusLDraw(_In_ PGlusLine _pLine);
Glus_Intersect	glusLIntersect(_In_ PGlusVector _laa, _In_	PGlusVector	_lab, _In_ PGlusVector _lba, _In_	PGlusVector	_lbb, _Inout_ PGlusVector _p);
#define	glusLMiddle(PointA,PointB,PointM) glusAdd(PointA,0.5,PointB,0.5,PointM)
#define glusLFormPToR()
#define glusLFormPToN()
#define glusLFormRToP(linev,linep) (linep)->A = (linev)->Position,glusVAdd(&(linev)->Position,1,&(linev)->Direction,1,&(linep)->B);
#define glusLFormRToN()
#define glusLFormNToP()
#define glusLFormNToV()

void	glusLineRel(_In_	double	_x, _In_	double	_y, _In_	double	_z);
void	glusMoveTo(_In_	double	_x, _In_	double	_y, _In_	double	_z);
void	glusLineTo(_In_	double	_x, _In_	double	_y, _In_	double	_z);

void	glusPushCS();
void	glusPopCS();

void	glusTurn(double _angle);
void	glusTurnTo(double _angle);
void	glusForward(_In_	double	_dist,_In_	bool	_is_visible);
void	glusUIEnter();
void	glusUILeave();
int		glusGetHeight();
int		glusGetWidth();
void	glusReshape(int x, int y);
void	bresenham(int ax, int ay, int bx, int by);
#define glusHideCursor() glutSetCursor(GLUT_CURSOR_NONE) // refer:https://www.gamedev.net/topic/147887-hide-mouse-with-glut/
//
// see ray.c
// 
#define glusRayPos(ray,t,p_o) glusAdd(&(ray)->Point,1,&(ray)->Direction,t,(p_o))
void	glusRDraw(_In_ PGlusRay _pRay);
GLdouble	glusRHit(_In_ PGlusRay _r, _In_ PGlusVector _n, _In_ PGlusVector _p);
void	glusRReflecte(_Inout_	PGlusRay	_ray, _In_	PGlusVector	_normal, _Inout_	PGlusVector	_direction);

GLdouble	glusRTrace2D(_In_	PGlusRay	_ray, _In_	PGlusLink	_head, _Inout_	PGlusRay	_nRay);
void	glusRTraces2D(_Inout_	PGlusRay		_ray, _In_	PGlusLinks	_head);

GLdouble	glusRHit2DS(_In_	PGlusRay	_ray,_In_	PGlusSink	_head,_Inout_	PGlusRay	_nRay);
void	glusRHitMul2DS(_Inout_	PGlusRay		_ray,_In_	PGlusMulSink	_head);

void	glusRHitS(_Inout_ PGlusRay _ray, _In_ PGlusSink _head);

//
// see circle.c
//
bool	glusCExTri(_In_	PGlusVector	_pa, _In_	PGlusVector	_pb, _In_	PGlusVector	_pc, _Out_	PGlusCircle _c);
bool	glusCInTri(_In_	PGlusVector	_pa, _In_	PGlusVector	_pb, _In_	PGlusVector	_pc, _Out_	PGlusCircle _c);
bool	glusCInTri(_In_	PGlusVector	_pa, _In_	PGlusVector	_pb, _In_	PGlusVector	_pc, _Out_	PGlusCircle _c);
bool	glusCNinePoint(_In_	PGlusVector	_pa, _In_	PGlusVector	_pb, _In_	PGlusVector	_pc, _Out_	PGlusCircle _c);
bool	glusCGetTangentPoints(_In_ PGlusVector _pa, _In_ PGlusVector	_pb, _In_ PGlusVector _pc, _Out_ PGlusVector _pr, _Out_ PGlusVector _ps, _Out_ PGlusVector _pt);
bool	glusCGetAltitudeFeet(_In_ PGlusVector _pa, _In_ PGlusVector	_pb, _In_ PGlusVector _pc, _Out_ PGlusVector _pr, _Out_ PGlusVector _ps, _Out_ PGlusVector _pt);
bool	glusCGetMiddlePoints(_In_ PGlusVector _pa, _In_ PGlusVector	_pb, _In_ PGlusVector _pc, _Out_ PGlusVector _pr, _Out_ PGlusVector _ps, _Out_ PGlusVector _pt);
void	glusDrawArc(_In_	PGlusCircle	_c,_In_	GLdouble	_angle_start,_In_	GLdouble	_angle_sweep,_In_	Glussize		_n);
#define glusDrawCircle(circle)	glusDrawArc(circle,0,360,45)

//
// see plane.c
//
Glus_Intersect	glusPlaneIntersect(_In_	PGlusPlane	_pa, _In_	PGlusPlane	_pb, _Inout_	PGlusLine	_l);

//
// see points.c
//
void	glusDrawPolygon(_In_	PGlusLink	_p);
void	glusDrawPolygonS(_In_	PGlusSink	_p);
void	glusDrawMulPolygonS(_In_	PGlusSink	_p);
void	glusDrawPolygons(_In_	PGlusLink	_p);
void	glusDrawPolyLine(_In_	PGlusLink	_p);
void	glusDrawPlinesA(PGlusPointsA p);
void	glusPolygonUnion(_In_	PGlusLink	_pa, _In_	PGlusLink	_pb, _Inout_	PGlusLink	_po);
void	glusPointsClear_A(_In_	PGlusVector	_pointer);
void	glusPointsClear_L(_In_	PGlusLink	_head);
void	glusPointsNormal_A(_In_	PGlusVector	_p_points,_In_	Glusnum		_n_points,_Inout_	PGlusVector	_normal);
void	glusPointsNormal_L(_In_	PGlusLink	_head,_Inout_	PGlusVector	_normal);
void	glusPointsConvert_L_A(_In_	PGlusLink	_h_points,_Inout_	PGlusVector	_a_points,_In_	Glusnum		_n);
void	glusPointsConvert_A_L(_In_	PGlusVector	_a_points,_In_	Glusnum		_n,_Inout_	PGlusLink	_h_points);
void	glusPointsExtrude(_In_	PGlusVector		_source,_Inout_	PGlusVector		_target,_In_	Glusnum			_n,_In_	PGlusVector		_vector);

// see tween.c
void	glusTween(_In_	PGlusLink	_pa, _In_	PGlusLink	_pb, _In_	GLdouble		_t);
GlusTweenHandle	glusTweenInit(_In_	PGlusPoints _pa, _In_ PGlusPoints _pb, _In_ GLdouble interval, _In_ bool isStart, _In_ bool	isReverse, _In_ bool isRepeat);
void	glusTweenClear(GlusTweenHandle		h);
void	glusTweenPause(GlusTweenHandle		h);
void	glusTweenContinue(GlusTweenHandle	h);
void	glusTweenReset(GlusTweenHandle		h);
void	glusTweenDelete();
void	glusTweenS(_In_ PGlusPointsS _pa, _In_ PGlusPointsS _pb, _In_ GLdouble _t);
GlusTweenHandle glusTweenInitS(_In_ PGlusPoints _pa, _In_ PGlusPoints _pb, _In_ GLdouble _interval, _In_ bool _isStart, _In_ bool _isReverse, _In_ bool _isRepeat);

//
// see mouse.c
//
void	glusMouseLoop(_In_	int _button, _In_ int _state, _In_	int	_x, _In_	int	_y);
void	glusMouseInsert(_In_	pvoid	_p_function,_In_	pvoid	_p_data);
void	glusMouseRemove(_In_	pvoid	_p_function);
void glusMouseSelect(int button, int state, int x, int y, pvoid p);

//
// see link.c
//

// doubly link
#define glusLink(Name)	GlusLink Name={.FLink = &Name,.BLink = &Name}
#define glusLinks(Name)	GlusLinks Name={.Data.FLink = &Name.Data,.Data.BLink = &Name.Data}
void	glusLinkInit(_Inout_ void* LinkHead);
#define glusLinkIsEmpty(LinkHead) ((LinkHead)->FLink == (LinkHead))	// update [9/13/2016 blue] : fix bug for leave one
#define glusLinkIsHead(point,head) ((PGlusLink)(point) == (PGlusLink)(head))
GLint	glusLinkLength(_In_	PGlusLink	_linkHead);
void	glusLinkInsertHead(_Inout_ pvoid LinkHead, _Inout_ pvoid LinkNode);
void	glusLinkInsertTail(_Inout_ void* _linkHead, _Inout_ void* _linkNode);
pvoid	glusLinkRemoveHead(_Inout_ pvoid _linkHead);
pvoid	glusLinkRemoveTail(_Inout_ pvoid _linkHead);
bool	glusLinkRemoveEntry(_Inout_ pvoid _linkEntry);
void	glusLinkClear(_Inout_	PGlusLink	_link_head);
#define glusLinkData(pointer)	(void*)((char*)pointer+sizeof(GlusLink))						// get the date field pointer 
#define glusEntry(link_pointer,type,member) ((type*)((char*)pointer)-&((type*)0)->member))	// get the struct pointer from a member
#define glusLinkFirst(name,ptr)		PGlusLink name = (void*)((PGlusLink)ptr)->BLink
#define glusLinkNext(ptr)			ptr = (void*)((PGlusLink)ptr)->BLink

// singly link
#define glusSink(Name) GlusSink Name = null;
#define glusMulSink(Name) GlusMulSink Name = null;
#define glusSinkInit(LinkHead) ((PGlusSink)LinkHead)->Next = NULL;
#define glusSinkIsEmpty(LinkHead) (!LinkHead->Next)
void	glusSinkPush(_Inout_ PGlusSink _linkHead, _In_ PGlusSink _linkEntry);
PGlusSink	glusSinkPop(_Inout_ PGlusSink _linkHead);
GLint	glusSinkLength(_In_ PGlusSink _linkHead);
void	glusSinkClear(_Inout_	PGlusSink	_sink_head);

// transformations
void	glusTransform(_In_	PGlusTransform	_trans);
void	glusTransformVector(PGlusTransform _trans, PGlusVector _v,PGlusVector _o);
void	glusTransformInvVector(PGlusTransform _trans, PGlusVector _v, PGlusVector _o);
void	glusScale(_In_	GLdouble	_sx,_In_		GLdouble	_sy,_In_	GLdouble	_sz);
void	glusTranslate(_In_ GLdouble	_dx,_In_		GLdouble	_dy,_In_	GLdouble	_dz);
void	glusRotate(_In_	GLdouble	_angle,_In_		GLdouble	_x, _In_	GLdouble	_y,_In_	GLdouble	_z);
#define glusRotateX(angle)		glusRotate(angle,1,0,0)
#define glusRotateY(angle)		glusRotate(angle,0,1,0)
#define glusRotateZ(angle)		glusRotate(angle,0,0,1)

#define glusScale2D(sx,sy)		glusScale(sx,sy,0.0)	
#define glusTranslate2D(dx,dy)	glusTranslate(dx,dy,0.0)
#define glusRotate2D(angle)		glusRotate(angle,0.0,0.0,1.0)


void	glusTransformDefault(_In_	PGlusTransform	_trans);

//
// shape
//
void	glusTiling(_In_	PGlusTransform	_orgTrans,_In_	PGlusTransform	_eachTrans,_In_	GLdouble		_x,_In_	GLdouble		_y,_In_	GLdouble		_z,_In_	void(*callback)(void));
void	glusAxis(_In_	GLdouble	_length);
void	glusAxis3D(_In_	pvoid		_ptr);
void	glusSphere(pvoid _pointer);
void	glusCube(pvoid _pointer);
void	glusTeapot(pvoid _pointer);
void	glusGrid(pvoid _ptr);
void	glusShapeDefault(_Inout_ PGlusShape _shape);
void	glusShapeDraw(_In_	PGlusShape	_p);

void	glusSquare(pvoid p);

void	glusKoch(double dir, double len, int n);
void	glusKochSnow(pvoid ptr);

// scene
PGlusScene	glusSceneNew(_In_	str	_fileName);
void	glusSceneLoad(_In_	PGlusScene	_scene,_In_	str			_file_name);
void	glusSceneClear(_In_	PGlusScene	_scene);
PGlusScene	glusSceneDefault();
void	glusSceneDraw(_In_	PGlusScene	_scene);
void	glusSceneLight(_In_	PGlusScene	_scene);

void	glusSceneReshape(_In_ PGlusScene	_scene, int w, int h);
void	glusSceneHit(PGlusScene _scene, PGlusRay _ray, PGlusIntersect _inter);
void	glusSceneShade(PGlusScene _scene, PGlusRay _ray, PGlusColor _clr);
void	glusSceneRayTrace(PGlusScene _scene, int _block_size);

#define glusSceneGetLastShape(p_scene)	(PGlusShape)glusLinkData((p_scene)->Shapes.FLink)
PGlusShape	glusSceneCreateNewShape(_In_ PGlusScene _scene);
PGlusShape	glusSceneGetShapeByName(_In_	PGlusScene		_scene,_In_	str				_name);
// sdl
void	glusSDL(_Inout_ PGlusScene	_scene, _In_	FILE*		_file);
void	glusSDLex(_Inout_ PGlusScene	_scene, _In_	FILE*		_file);
void	glusSDLClear();
//
// file
//
#define		glusScanf(file,sz_format,...)		fscanf_s(file,sz_format,__VA_ARGS__)
#define		glusSkipSpace(file)					fscanf_s(file,"%*[ \t\r\n]")

#define		glusPointLoad(file,sz_format,ptr) \
					glusScanf(file,sz_format, &(ptr)->X, &(ptr)->Y, &(ptr)->Z);(ptr)->V = 1

#define		glusVectorLoad(file,sz_format,ptr) \
					glusScanf(file,sz_format, &(ptr)->X, &(ptr)->Y, &(ptr)->Z);(ptr)->V = 0

Glussize	glusPointsLoad_L(_In_	FILE *		_file,_In_	PGlusLink	_head,_In_	Glussize	_max_size);
Glussize	glusPointsLoad_A(_In_	FILE *		_file, _Inout_	PGlusVector	_buffer, _In_	Glussize	_max_size);

Glussize	glusVectorsLoad_L(_In_	FILE *		_file, _In_	PGlusLink	_head, _In_	Glussize	_max_size);
Glussize	glusVectorsLoad_A(_In_	FILE *		_file, _Inout_	PGlusVector	_buffer, _In_	Glussize	_max_size);

#if WIN32
FILE*	fmemopen(_In_	char *	buffer,_In_	size_t	size,_In_	char *	mode);
#endif
void	fskipcomment(_In_	FILE **	_file,_In_	char *	line_comment,_In_	char *	mul_start_comment,_In_	char *	mul_end_comment);
FILE *fgetstdin();
//
// mesh
//
void	glusMeshDraw(_In_	PGlusMesh	_mesh);
bool	glusMeshLoad(_In_	FILE *		_file,_Inout_	PGlusMesh	*_mesh);
void	glusMeshClear(_In_	PGlusMesh	_mesh);
bool	glusMeshAddToScene(_In_	PGlusMesh	_mesh, _Inout_	PGlusScene	_scene);

void	glusMeshFaceNormal(_Inout_		PGlusMesh	_mesh);
PGlusMesh	glusMeshMakePrism(_In_	PGlusLink	_point, _In_	PGlusVector	_vector);
void	glusMeshsDraw(_In_	PGlusLink	_head);
void	glusMeshsClear(_In_	PGlusLink	_head);
PGlusLink	glusMakePrismArray(_In_	PGlusLink	_polygons,_In_	PGlusVector	_vector);
PGlusMesh	glusMeshExtrudeQuadStrip(_In_	PGlusLink	_quad_strip,_In_	PGlusVector	_vector);
PGlusMesh	glusMeshRevolution(_In_	PGlusLink	_points,_In_	Glusnum		_n,_In_	GLdouble	_ang_start,_In_	GLdouble	_ang_sweep);

PGlusMesh	glusMeshSurface(_In_	Glusnum		_n_piece,_In_	GLdouble	_u_start,_In_	GLdouble	_u_sweep,_In_	Glusnum		_n_stack,	_In_	GLdouble	_v_start,_In_	GLdouble	_v_sweep,_In_    void(*_f_point)(GLdouble u, GLdouble v, PGlusVector o));
PGlusMesh	glusMeshSurfaceBilinear(_In_	Glusnum		_n_piece,_In_	GLdouble	_u_start,_In_	GLdouble	_u_sweep,_In_	Glusnum		_n_stack,_In_	GLdouble	_v_start,_In_	GLdouble	_v_sweep,_In_	void(*_f_a)(GLdouble u, PGlusVector o),_In_	void(*_f_b)(GLdouble u, PGlusVector o));
bool	glusIsFaceBack(_In_	PGlusVector	_eye,_In_	PGlusVector	_p,_In_	PGlusVector	_v);
/*
 *	memory
 */
#define glusCheckex(pointer,do_when_fail) do{if(!pointer) {glusLog(__FILE__ ## "-" ## __FUNCTION__ ## "Allocate memory failed");do_when_fail;}}while(0)

// allocate memory ,zero the memory and  check it
#define glusAllocex(pointer,type,num,do_when_fail)		(pointer) = (type*)malloc((num)*sizeof(type));ZeroMemory((pointer),(num)*sizeof(type));glusCheckex(pointer,do_when_fail)
#define glusFree(pointer)		if(pointer) free(pointer),pointer = NULL

#define glusCheck(pointer)		glusCheckex(pointer,return 0)
#define glusAlloc(pointer,type)			type* pointer = (type*)malloc(sizeof(type));	ZeroMemory((pointer),sizeof(type));glusCheck(pointer)
#define glusAllocN(pointer,type,num)	type* pointer = malloc((num)*sizeof(type));ZeroMemory((pointer),(num)*sizeof(type));glusCheck(pointer)

/*
 *	status
 */
#define glusSuccess(status)	(status == true)
#define glusFail(status)	(status != true)


/*
 *	see surface.c
 */
void	glusSurfaceSphere(_In_	GLdouble	_u,_In_	GLdouble	_v,_Inout_	PGlusVector	_o);
bool	glusSurfaceBuildFace(_In_	PGlusMesh	_mesh,_In_	Glusnum		_n_piece,_In_	Glusnum		_n_stack);
void	glusSurfaceBilinear(_In_	GLdouble	_u, _In_	GLdouble	_v, _In_	void(*_f_a)(GLdouble u, PGlusVector o), _In_	void(*_f_b)(GLdouble u, PGlusVector o), _Inout_	PGlusVector	_o);

/*
 *	see camera.c
 */
void	glusCamera(_In_	PGlusCamera		_camera);
void	glusCameraSet(_In_	GLdouble	_eyex, GLdouble	_eyey, GLdouble		_eyez,_In_	GLdouble	_lookx, GLdouble	_looky, GLdouble		_lookz,_In_	GLdouble	_upx, GLdouble	_upy, GLdouble		_upz,_Inout_	PGlusCamera	_camera);

void	glusCameraStereo(_In_	GLdouble	_d,_Inout_	PGlusCamera	_camera);
void	glusCameraPop(_In_	PGlusCamera	_camera);
void	glusCameraPush(_In_	PGlusCamera	_camera);

void	glusCameraSlide(_In_	GLdouble	_u_del,_In_	GLdouble	_v_del,_In_	GLdouble	_n_del,_Inout_	PGlusCamera	_camera);
void	glusCameraRoll(_In_	GLdouble	_ang,_Inout_	PGlusCamera	_camera);
void	glusCameraYaw(_In_	GLdouble	_ang, _Inout_	PGlusCamera	_camera);
void	glusCameraPitch(_In_	GLdouble	_ang, _Inout_	PGlusCamera	_camera);


void	glusProjection(_In_	PGlusProjection		_proj);
void	glusObli(_In_	GLdouble	_dx,_In_	GLdouble	_dy,_In_	GLdouble	_dz);
void	glusPerspective(_In_	GLdouble	_angle,_In_	GLdouble	_ration,_In_	GLdouble	_near,_In_	GLdouble	_far,_Inout_	PGlusProjection	_projection);
void	glusOrtho(_In_	GLdouble	_left,_In_	GLdouble	_right,_In_	GLdouble	_top,_In_	GLdouble	_bottom,_In_	GLdouble	_near,_In_	GLdouble	_far,_Inout_	PGlusProjection	_projection);
void	glusOblique(_In_	GLdouble	_dx,_In_	GLdouble	_dy,_In_	GLdouble	_dz,_Inout_	PGlusProjection	_projection);

void	glusCameraRay(int _x, int _y, PGlusRay _ray,PGlusCamera _cam,PGlusProjection _proj);

/*
 *	status
 */
void	glusSetShadeLevel(_In_	Glusenum	_param);
Glusenum	glusGetShadeLevel();

PGlusVector	glusGetEye();


/*
 *	texture
 */
bool	glusTextureLoad(_In_	FILE*		file,_Inout_	PGlusScene	_scene);
bool	glusTextureIDLoad(_In_	FILE *		_file,_Inout_	PGlusMesh	_mesh);


/*
 *	peano curve
 */
void	glusPeanoLoad(_In_	FILE *	_file,_In_	PPeano	_pea);
void	glusPeano(_In_	PPeano		_peano, _In_	str			_as, _In_	double		_len, _In_	int			_level);
void	glusPeanoDraw(_In_	PPeano	_peano);


/*
 *	array
 */
void	glusArrayLoad(_In_	FILE *		_file,_Out_	PArray		_array,_In_	PGlusShape	_target);
void	glusArrayDraw(_In_	PArray	_p);
void	glusArrayRect(_In_	PArray	_p);

/*
 *	fract 
 */
void	glusFract(_In_	PGlusVector	_a,_In_	PGlusVector	_b,_In_	double		_std_dev,_In_	double		_min_len_sq,_In_	double		_fractor,_In_	int			_seed);
void	glusFractDraw(_In_	PFractal	_p);

/*
 *	chaos game
 */
PChaosGame	glusChaosGameLoad(_In_	FILE *	_file);
void	glusChaosGame(_In_	PChaosGame	_p);
PPixMap glusMandelbrotSet(_In_	double	px,_In_	double	py,_In_	double	w);


/*
 *	anime
 */
void	glusDissolve(PPixMap a, PPixMap b);


/*
 *	curve
 */
GlusVector	bezier(PGlusVector points, int n, double t);
void	glusBezier(PGlusPointsA points, int num);
void	glusBSpline(PGlusPointsA pa, int m, int num);
void	glusBSplineFunc(PGlusPointsA pa, int m, int num);

/*
 *	function handle window
 */
void	glusWinToWorld(_In_ int _x, _In_ int _y, _Inout_	PGlusVector _world);
void	glusWinToWorldex(_In_ int _x, _In_ int _y, int _z,_Inout_	PGlusVector _world);
int		glusWinTitleHeight();

/*
 *	matrix
 */
// identity matrix 4 x 4
#define glusMatrixIdentity(m) m[0][0] = 1; m[1][1]=1; m[2][2]=1; m[3][3]=1

// identity matrix 3 x 3
#define glusMatrix3Identity(m) m[0][0] = 1; m[1][1]=1; m[2][2]=1; 

// identity matrix 2 x 2
#define glusMatrix2Identity(m) m[0][0] = 1; m[1][1]=1; 

/*
 *	hit object
 */

bool	glusHitSphere(PGlusShape _s, PGlusRay _r, PGlusIntersect _inter);

bool	glusHitSquare(PGlusShape _s, PGlusRay _r, PGlusIntersect _inter);
#endif // !_GLUS_FUNC_H
#endif // !_glus_func_h



