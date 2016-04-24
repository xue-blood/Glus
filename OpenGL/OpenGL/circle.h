#pragma once



#define _PI (3.14159265)
#define _2PI (2*_PI)
#define _Rad (_PI/180)

#define _FI 1.618033989

#define secf(r) (1/cosf(r))

#define sinaf(a) sinf(a*_Rad)
#define cosaf(a) cosf(a*_Rad)
#define secaf(a) secf(a*_Rad)
#define tanaf(a) tanf(a*_Rad)



#define absf(f) ((f)>0?(f):-(f))