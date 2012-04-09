#ifndef _COMPLEX_H_
#define _COMPLEX_H_

#include "fftw3.h"

/* reuse the fftw?_complex type */
typedef fftwf_complex complex;

#define FLOAT_EQUAL(a, b)         ( (a<b)?( (b-a) < 1e-4 ):( (a-b) < 1e-4) )

#define COMPLEX_NEW(x, r, i)      {x[0]=(r); x[1]=(i);}
#define COMPLEX_REAL(x)           (x[0])
#define COMPLEX_IMAG(x)           (x[1])
#define COMPLEX_ABS2(x)           (x[0]*x[0]+x[1]*x[1])
#define COMPLEX_EQUAL(x, y)       ( FLOAT_EQUAL(x[0], y[0]) && FLOAT_EQUAL(x[1], y[1]) )

#define COMPLEX_CONJ(x, z)        {z[0]=x[0]; z[1]=-x[1];}
#define COMPLEX_MINUSCONJ(x, z)   {z[0]=-x[0]; z[1]=x[1];}

#define COMPLEX_ADD(x, y, z)      {z[0]=x[0]+y[0]; z[1]=x[1]+y[1];}
#define COMPLEX_SUB(x, y, z)      {z[0]=x[0]-y[0]; z[1]=x[1]-y[1];}
#define COMPLEX_MUL(x, y, z)      {z[0]=x[0]*y[0]-x[1]*y[1]; z[1]=x[0]*y[1]+x[1]*y[0];}
#define COMPLEX_MUL_CONJ(x, y, z) {z[0]=x[0]*y[0]+x[1]*y[1]; z[1]=x[1]*y[0]-x[0]*y[1];}

#define COMPLEX_SCALE(x, r, z)    {z[0]=x[0]*r; z[1]=x[1]*r;}
#define COMPLEX_ASSIGN(x, y)      {y[0]=x[0]; y[1]=x[1];}

#endif //_COMPLEX_H_