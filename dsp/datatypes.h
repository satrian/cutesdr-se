//////////////////////////////////////////////////////////////////////
// datatypes.h: Common data type declarations
//
// History:
//	2010-09-15  Initial creation MSW
//	2011-03-27  Initial release
//////////////////////////////////////////////////////////////////////
#ifndef DATATYPES_H
#define DATATYPES_H

#include <QtGui/QApplication>
#include <math.h>


//define single or double precision reals and complex types
typedef float tSReal;
typedef double tDReal;

typedef struct _sCplx
{
	tSReal re;
	tSReal im;
}tSComplex;

typedef struct _dCplx
{
	tDReal re;
	tDReal im;
}tDComplex;

typedef struct _isCplx
{
	qint16 re;
	qint16 im;
}tStereo16;


#define TYPEREAL tDReal
#define TYPECPX	tDComplex
#define TYPESTEREO16 tStereo16
#define TYPEMONO16 qint16

//#define K_2PI (8.0*atan(1))	//maybe some compilers are't too smart to optimize out
#define K_2PI (2.0 * 3.14159265358979323846)
#define K_PI (3.14159265358979323846)
#define K_PI4 (K_PI/4.0)
#define K_PI2 (K_PI/2.0)
#define K_3PI4 (3.0*K_PI4)


#endif // DATATYPES_H
