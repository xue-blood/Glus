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
void	glusDebugEnable(_In_	bool	_is_debug);
// #define glusDebug(msg,...)	do { if (IsDebug) printf(msg,__VA_ARGS__); } while(0)
#define glusDebug(msg,...)		fprintf_s(_Glus_Std_Debug,msg,__VA_ARGS__)
//
// see init.c
// 
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
#define glusVFromPoint(PointA,PointB,Vector) glusVAdd((PointA),-1,(PointB),1,(Vector))
#define glusPAddV(p_point,p_vector,p_point_out) glusVAdd( (p_point) ,1, (p_vector),1, (p_point_out))
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
// color
//
#define glusColor(pointer,r,g,b,a)	((pointer)->R = r),((pointer)->G = g),((pointer)->B = b),((pointer)->A = a)
//
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
Glus_Intersect	glusPlaneIntersect(_In_	PGlusPlane	_pa, _In_	PGlusPlane	_pb, _Out_	PGlusLine	_l);

//
// see points.c
//
void	glusDrawPolygon(_In_	PGlusLink	_p);
void	glusDrawPolygonS(_In_	PGlusSink	_p);
void	glusDrawMulPolygonS(_In_	PGlusSink	_p);
void	glusDrawPolygons(_In_	PGlusLink	_p);
void	glusDrawPolyLine(_In_	PGlusLink	_p);
void	glusPolygonUnion(_In_	PGlusLink	_pa, _In_	PGlusLink	_pb, _Out_	PGlusLink	_po);
void	glusPointsClear_A(_In_	PGlusVector	_pointer);
void	glusPointsClear_L(_In_	PGlusLink	_head);
void	glusPointsNormal_A(_In_	PGlusVector	_p_points,_In_	Glusnum		_n_points,_Inout_	PGlusVector	_normal);
void	glusPointsNormal_L(_In_	PGlusLink	_head,_Inout_	PGlusVector	_normal);
void	glusPointsConvert_L_A(_In_	PGlusLink	_h_points,_Out_	PGlusVector	_a_points,_In_	Glusnum		_n);
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
void	glusWinToWorld(_In_ int _x, _In_ int _y,_Inout_	PGlusVector _world);
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
#define glusLinkNext(head,current,next)	((pvoid)next)=((PGlusLink)current)->BLink;if(glusLinkIsHead((next),(head))) ((pvoid)next)=((PGlusLink)head)->BLink
GLint	glusLinkLength(_In_	PGlusLink	_linkHead);
void	glusLinkInsertHead(_Inout_ pvoid LinkHead, _Inout_ pvoid LinkNode);
void	glusLinkInsertTail(_Inout_ void* _linkHead, _Inout_ void* _linkNode);
pvoid	glusLinkRemoveHead(_Inout_ pvoid _linkHead);
pvoid	glusLinkRemoveTail(_Inout_ pvoid _linkHead);
bool	glusLinkRemoveEntry(_Inout_ pvoid _linkEntry);
void	glusLinkClear(_Inout_	PGlusLink	_link_head);
#define glusLinkData(pointer)	(char*)((char*)pointer+sizeof(GlusLink))						// get the date field pointer 
#define glusEntry(link_pointer,type,member) ((type*)((char*)pointer)-&((type*)0)->member))	// get the struct pointer from a member

// singly link
#define glusSink(Name) GlusSink Name = null;
#define glusMulSink(Name) GlusMulSink Name = null;
#define glusSinkInit(LinkHead) ((PGlusSink)LinkHead)->Next = NULL;
#define glusSinkIsEmpty(LinkHead) (!LinkHead->Next)
void	glusSinkPush(_Inout_ PGlusSink _linkHead, _In_ PGlusSink _linkEntry);
PGlusSink	glusSinkPop(_Inout_ PGlusSink _linkHead);
GLint	glusSinkLength(_In_ PGlusSink _linkHead);

// transformations
void	glusInitCT();
void	glusPushCT();
void	glusPopCT();
void	glusScale(_In_	GLdouble	_sx,_In_		GLdouble	_sy,_In_	GLdouble	_sz);
void	glusTranslate(_In_ GLdouble	_dx,_In_		GLdouble	_dy,_In_	GLdouble	_dz);
void	glusRotate(_In_	GLdouble	_angle,_In_		GLdouble	_x, _In_	GLdouble	_y,_In_	GLdouble	_z);
#define glusRotateX(angle)		glusRotate(angle,1,0,0)
#define glusRotateY(angle)		glusRotate(angle,0,1,0)
#define glusRotateZ(angle)		glusRotate(angle,0,0,1)

#define glusScale2D(sx,sy)		glusScale(sx,sy,0.0)	
#define glusTranslate2D(dx,dy)	glusTranslate(dx,dy,0.0)
#define glusRotate2D(angle)		glusRotate(angle,0.0,0.0,1.0)

#define glusTranslatev(p)	glusTranslate((p)->Dx,(p)->Dy,(p)->Dz)
#define glusScalev(p)	glusScale((p)->Sx,(p)->Sy,(p)->Sz)
#define glusRotatev(p)	glusRotate((p)->Angle,(p)->Ax,(p)->Ay,(p)->Az)

void	glusTransformDefault(_In_	PGlusTransform	_trans);

//
// shape
//
void	glusTiling(_In_	PGlusTransform	_orgTrans,_In_	PGlusTransform	_eachTrans,_In_	GLdouble		_x,_In_	GLdouble		_y,_In_	GLdouble		_z,_In_	void(*callback)(void));
void	glusAxis(_In_	GLdouble	_length);
void	glusAxis3D(_In_	GLdouble	_length);
void	glusSphere(pvoid _pointer);
void	glusCube(pvoid _pointer);
void	glusShapeDefault(_Inout_ PGlusShape _shape);



// scene
PGlusScene	glusSceneLoad(_In_	str	_fileName);
void	glusSceneUnload(_In_	PGlusScene	_scene);
PGlusScene	glusSceneDefault();
void	glusSceneDraw(_In_	PGlusScene	_scene);
void	glusSceneLight(_In_	PGlusScene	_scene);

#define glusSceneGetLastShape(p_scene)	(PGlusShape)glusLinkData((p_scene)->Shapes.FLink)
PGlusShape	glusSceneCreateNewShape(_In_ PGlusScene _scene);

//
// file
//
#define		glusFileSkipSpace(file)		fscanf_s(file,"%*[ \r\t\n]")
#define		glusFileScanf(file,sz_format,...)	do{\
				glusFileSkipSpace(file);		\
				fscanf_s(file,sz_format, __VA_ARGS__ );}while(0)	// skip all kind of space

void		glusFileLoadPoint(_In_	FILE * _file, _In_	str _format, _Outptr_ PGlusVector	_p_point);
void		glusFileLoadVector(_In_	FILE * _file, _In_	str _format, _Outptr_ PGlusVector	_p_vector);

Glussize	glusFileLoadPoints_L(_In_	FILE *		_file,_In_	PGlusLink	_head,_In_	Glussize	_max_size);
Glussize	glusFileLoadPoints_A(_In_	FILE *		_file, _Inout_	PGlusVector	_buffer, _In_	Glussize	_max_size);

Glussize	glusFileLoadVectors_L(_In_	FILE *		_file, _In_	PGlusLink	_head, _In_	Glussize	_max_size);
Glussize	glusFileLoadVectors_A(_In_	FILE *		_file, _Inout_	PGlusVector	_buffer, _In_	Glussize	_max_size);


//
// mesh
//
void	glusMeshDraw(_In_	PGlusMesh	_mesh);
Glus_Status	glusMeshLoad(_In_	FILE *		_file,_Inout_	PGlusMesh	*_mesh);
void	glusMeshClear(_In_	PGlusMesh	_mesh);
Glus_Status	glusMeshAddToScene(_In_	PGlusMesh	_mesh, _Inout_	PGlusScene	_scene);

void	glusMeshFaceNormal(_Inout_		PGlusMesh	_mesh);
PGlusMesh	glusMeshMakePrism(_In_	PGlusLink	_point, _In_	PGlusVector	_vector);
void	glusMeshsDraw(_In_	PGlusLink	_head);
void	glusMeshsClear(_In_	PGlusLink	_head);
PGlusLink	glusMakePrismArray(_In_	PGlusLink	_polygons,_In_	PGlusVector	_vector);
PGlusMesh	glusMeshExtrudeQuadStrip(_In_	PGlusLink	_quad_strip,_In_	PGlusVector	_vector);
PGlusMesh	glusMeshRevolution(_In_	PGlusLink	_points,_In_	Glusnum		_n,_In_	GLdouble	_ang_start,_In_	GLdouble	_ang_sweep);

PGlusMesh	glusMeshSurface(_In_	Glusnum		_n_piece,_In_	GLdouble	_u_start,_In_	GLdouble	_u_sweep,_In_	Glusnum		_n_stack,	_In_	GLdouble	_v_start,_In_	GLdouble	_v_sweep,_In_    void(*_f_point)(GLdouble u, GLdouble v, PGlusVector o));
PGlusMesh	glusMeshSurfaceBilinear(_In_	Glusnum		_n_piece,_In_	GLdouble	_u_start,_In_	GLdouble	_u_sweep,_In_	Glusnum		_n_stack,_In_	GLdouble	_v_start,_In_	GLdouble	_v_sweep,_In_	void(*_f_a)(GLdouble u, PGlusVector o),_In_	void(*_f_b)(GLdouble u, PGlusVector o));
/*
 *	memory
 */
#define glusCheckex(pointer,do_when_fail) do{if(!pointer) {glusDebug(__FILE__ ## "-" ## __FUNCTION__ ## "Allocate memory failed");do_when_fail;}}while(0)

// allocate memory ,zero the memory and  check it
#define glusAllocex(pointer,type,num,do_when_fail)		(pointer) = (type*)malloc((num)*sizeof(type));ZeroMemory((pointer),(num)*sizeof(type));glusCheckex(pointer,do_when_fail)
#define glusFree(pointer)		if(pointer) free(pointer),pointer = NULL

#define glusCheck(pointer)		glusCheckex(pointer,return Glus_Status_Memory_Allocate_Fail)
#define glusAlloc(pointer,type)			(pointer) = (type*)malloc(sizeof(type));	ZeroMemory((pointer),sizeof(type));glusCheck(pointer)
#define glusAllocN(pointer,type,num)	(pointer) = (type*)malloc((num)*sizeof(type));ZeroMemory((pointer),(num)*sizeof(type));glusCheck(pointer)

/*
 *	status
 */
#define glusSuccess(status)	(status == Glus_Status_Success)
#define glusFail(status)	(status != Glus_Status_Success)


/*
 *	see surface.c
 */
void	glusSurfaceSphere(_In_	GLdouble	_u,_In_	GLdouble	_v,_Out_	PGlusVector	_o);
Glus_Status	glusSurfaceBuildFace(_In_	PGlusMesh	_mesh,_In_	Glusnum		_n_piece,_In_	Glusnum		_n_stack);
void	glusSurfaceBilinear(_In_	GLdouble	_u,_In_	GLdouble	_v,_In_	void(*_f_a)(GLdouble u, PGlusVector o),_In_	void(*_f_b)(GLdouble u, PGlusVector o),_Out_	PGlusVector	_o);




#endif // !_GLUS_FUNC_H
#endif // !_glus_func_h



