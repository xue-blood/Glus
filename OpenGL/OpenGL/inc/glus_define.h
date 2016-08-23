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
#define sina(a)		sin(a*_Rad)
#define cosaf(a)	cosf(a*_Rad)
#define cosa(a)		cos(a*_Rad)
#define secaf(a)	secf(a*_Rad)
#define tanaf(a)	tanf(a*_Rad)
#define tana(a)		tan(a*_Rad)

#define absf(f) ((f)>0?(f):-(f))

#define strlower(str) for(int i=0;str[i];i++) str[i]=tolower(str[i])

typedef	char *		str;
typedef wchar_t *	wstring;


typedef double *	pdouble;
typedef void *		pvoid;

#define assertp(p)	assert(p != NULL)

#define Zero		0.00000000000001
//  [8/5/2016 Tld]: change 0.000000001 to 0.00000000000001

#define Zerof		0.000001
//  [8/5/2016 Tld]: add

#define null		{ 0 }	

typedef void 		*pvoid;

typedef GLdouble 	*pGLdouble;
typedef	size_t		size;


#define glusEntry(link_pointer,type,member) ((type*)((char*)pointer)-&((type*)0)->member))	// get the struct pointer from a member

#define glusAlloc(type)		(type*)malloc(sizeof(type))
#define glusFree(pointer)	free(pointer),pointer = NULL;


#endif // !_glus_define_h
#endif // !_GLUS_DEFINE_H
