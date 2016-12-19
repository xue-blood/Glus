#include "glus_pre.h"

#ifndef _GLUS_DEFINE_H
#define _GLUS_DEFINE_H
#ifndef _glus_define_h
#define _glus_define_h


//
// circle
// 
#define Glus_PI			(3.14159265)
#define Glus_2PI		(2*Glus_PI)
#define Glus_Rad		(Glus_PI/180)

#define _FI			1.618033989

#define secf(r)		(1/cosf(r))

#define sinaf(a)	sinf(a*Glus_Rad)
#define sina(a)		sin(a*Glus_Rad)
#define cosaf(a)	cosf(a*Glus_Rad)
#define cosa(a)		cos(a*Glus_Rad)
#define secaf(a)	secf(a*Glus_Rad)
#define tanaf(a)	tanf(a*Glus_Rad)
#define tana(a)		tan(a*Glus_Rad)

#define absf(f) ((f)>0?(f):-(f))

#define strlower(str) for(int i=0;str[i];i++) str[i]=tolower(str[i])

#define strequ(a,b)	(strcmp(a,b)==0)

typedef	char *		str;
typedef wchar_t *	wstring;


typedef double *	pdouble;
typedef void *		pvoid;
typedef	unsigned char	ubyte, *pubyte;

#define assertp(p)	assert(p != NULL)

#define Glus_Zero		0.00000000000001	// for double
//  [8/5/2016 Tld]: change 0.000000001 to 0.00000000000001 

#define Glus_Zerof		0.000001			// for float
//  [8/5/2016 Tld]: add

#define null		{ 0 }	

typedef void 		*pvoid;

typedef GLdouble 	*pGLdouble;
typedef	size_t		Glussize;	// change from size [8/24/2016 tld] : the size of object
typedef	long long	Glusindex;	// add [8/24/2016 tld] : the index of object
typedef size_t		Glusnum;	// add [8/24/2016 tld] : the num of object
typedef	GLenum		Glusenum;

// add [12/19/2016 xue]
#define Check(_ptr,funcHandle,funcFinally)		\
	do											\
			{											\
		if(!(_ptr))							\
						{										\
			funcHandle;							\
			funcFinally;						\
						}										\
			}while(0)

/*
 *	status
 */
// shade level
#define Glus_Shade_Wire		0
#define Glus_Shade_Solid	1



#endif // !_glus_define_h
#endif // !_GLUS_DEFINE_H
