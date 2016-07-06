#include "glus_pre.h"

#ifndef _GLUS_DEFINE_H
#define _GLUS_DEFINE_H
#ifndef _glus_define_h
#define _glus_define_h


//
// circle
// 
#define _PI			(3.14159265)
#define _2PI		(2*_PI)
#define _Rad		(_PI/180)

#define _FI			1.618033989

#define secf(r)		(1/cosf(r))
	
#define sinaf(a)	sinf(a*_Rad)
#define cosaf(a)	cosf(a*_Rad)
#define secaf(a)	secf(a*_Rad)
#define tanaf(a)	tanf(a*_Rad)

#define absf(f) ((f)>0?(f):-(f))




#define assertp(p)	assert(p != NULL)

#define Zero		0.00000000000001
//  [8/5/2016 Tld]: change 0.000000001 to 0.00000000000001

#define Zerof		0.000001
//  [8/5/2016 Tld]: add

#define null		{ 0 }	


/*
#define normalize(data)												\
	if( (data) > 0)													\
	{ if((data +Zero)-(GLint)(data +Zero) <Zero)					\
			(data) = (GLint)(data + Zero); }						\
	else															\
	{ if((GLint)(data -Zero)-(data -Zero) <Zero)					\
		(data) = (GLint)(data - Zero);}
*/

#endif // !_glus_define_h
#endif // !_GLUS_DEFINE_H
