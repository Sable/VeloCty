#ifndef __MULT_H__
#define __MULT_H__
#include "cblas.h"
#include "vrbinding.hpp"
#include "helper.hpp"
//#include<complex.h>
#include<lapacke.h>

class BlasDouble{
	public :
		static VrArrayPtrF64 mmult(CBLAS_ORDER order, CBLAS_TRANSPOSE transA, CBLAS_TRANSPOSE transB, 
				  VrArrayPtrF64 A, 
				  VrArrayPtrF64 B, float alpha=1,float beta=0);
		static void mmult(CBLAS_ORDER order, CBLAS_TRANSPOSE transA, CBLAS_TRANSPOSE transB, 
				  VrArrayPtrF64 A, 
				  VrArrayPtrF64 B,double *val, float alpha=1,float beta=0);
		static void mmult(CBLAS_ORDER order, CBLAS_TRANSPOSE transA, CBLAS_TRANSPOSE transB, 
				  VrArrayPtrF64 A, 
				  VrArrayPtrF64 B,VrArrayPtrF64* val, float alpha=1,float beta=0);
		static VrArrayPtrF64 vec_mult(CBLAS_ORDER order, CBLAS_TRANSPOSE, VrArrayPtrF64 A, VrArrayPtrF64 B, float alpha=1, int incX=1 , int incY=1,float beta=0 ); 
		static VrArrayPtrF64 scal_mult(int ndims,VrArrayPtrF64 A,double);
		static VrArrayPtrF64 scal_mult(int ndims,VrArrayPtrF64 A,double,VrArrayPtrF64 *Y);
		static VrArrayPtrF64 vec_add(int ndims, VrArrayPtrF64 X,VrArrayPtrF64 Y,  const double alpha=1, const int incX=1 ,const int incY=1 );
		static void vec_add(int ndims, VrArrayPtrF64 X,VrArrayPtrF64 Y,VrArrayPtrF64 *Z,  const double alpha=1, const int incX=1 ,const int incY=1 );
		static double vec_dot(const int , VrArrayPtrF64 X, VrArrayPtrF64 Y, const int incX=1, const int incY=1 );
		static VrArrayPtrF64 vec_copy(const int, VrArrayPtrF64 X, const int incX=1 , const int incY=1 );
        
        static void vec_copy(const int ndims, VrArrayPtrF64 X,VrArrayPtrF64 *Y, const int incX=1 , const int incY=1 );
		static VrArrayPtrF64 vec_sub(const int ndims, VrArrayPtrF64 X, VrArrayPtrF64 Y ,const double alpha=1, const int incX=1 , const int incY=1 );
		static void vec_sub(const int ndims, VrArrayPtrF64 X, VrArrayPtrF64 Y,VrArrayPtrF64 *Z ,const double alpha=1, const int incX=1 , const int incY=1 );
		static VrArrayPtrF64 scal_add(const int ndims,VrArrayPtrF64 X, double scal, const int incX=1);
		static void scal_add(const int ndims,VrArrayPtrF64 X, double scal,VrArrayPtrF64 *Y, const int incX=1);
		static VrArrayPtrF64 transpose(VrArrayPtrF64 A);
		static VrArrayPtrF64 mat_ldiv(int matrix_order ,VrArrayPtrF64 A , VrArrayPtrF64 B);
		static VrArrayPtrF64 mat_rdiv(int matrix_order ,VrArrayPtrF64 A , VrArrayPtrF64 B);
		static VrArrayPtrF64 scal_div(  VrArrayPtrF64 A, double  scal);
		static void scal_div(  VrArrayPtrF64 A, double  scal,VrArrayPtrF64 *B);
		static VrArrayPtrF64 elem_mult(VrArrayPtrF64 A, VrArrayPtrF64 B);
static void elem_mult(VrArrayPtrF64 A, VrArrayPtrF64 B,VrArrayPtrF64 *C);
		static VrArrayPtrF64 elem_div(VrArrayPtrF64 A, VrArrayPtrF64 B, bool zeroCheck=false);
static void elem_div(VrArrayPtrF64 A, VrArrayPtrF64 B, VrArrayPtrF64 *C,bool zeroCheck = false);
		static VrArrayPtrF64 scal_minus(VrArrayPtrF64 A, double scal);
		static void scal_minus(VrArrayPtrF64 A, double scal,VrArrayPtrF64 *B);
		static VrArrayPtrF64 elem_mult(VrArrayPtrF64 A, double scal) ;
		static VrArrayPtrF64 elem_mult(double scal,VrArrayPtrF64 A );
		static void elem_mult(VrArrayPtrF64 A, double scal,VrArrayPtrF64 *) ;
		static void elem_mult(double scal,VrArrayPtrF64 A,VrArrayPtrF64* );
		static VrArrayPtrF64 vec_sub(const int ndims, const double alpha,VrArrayPtrF64 X);
		static void vec_sub(const int ndims, const double alpha,VrArrayPtrF64 X,VrArrayPtrF64 *Y);
		static VrArrayPtrF64 vec_sub(const int ndims, VrArrayPtrF64 X,const  double alpha);
		static void vec_sub(const int ndims, VrArrayPtrF64 X,const  double alpha,VrArrayPtrF64*);
};

class BlasSingle{
	  public :
                static VrArrayPtrF32 mmult(CBLAS_ORDER order, CBLAS_TRANSPOSE transA, CBLAS_TRANSPOSE transB,
                                  VrArrayPtrF32 A,
                                  VrArrayPtrF32 B,float alpha =1, float beta=1);
                static VrArrayPtrF32 vec_mult(CBLAS_ORDER order, CBLAS_TRANSPOSE, VrArrayPtrF32 A, VrArrayPtrF32,float alpha=1, int incX=1,int incY=1, float beta=1);
                static VrArrayPtrF32 scal_mult(int ndims,VrArrayPtrF32 A,float);
                static VrArrayPtrF32 vec_add(int ndims, VrArrayPtrF32 X,VrArrayPtrF32 Y,  const float alpha=1, const int incX=1 ,const int incY=1 );
                static float vec_dot(const int , VrArrayPtrF32 X, VrArrayPtrF32 Y, const int incX=1 , const int incY=1);
                static VrArrayPtrF32 vec_copy(const int, VrArrayPtrF32 X, const int incX=1, const int incY=1 );
		static VrArrayPtrF32 vec_sub(const int ndims, VrArrayPtrF32 X, VrArrayPtrF32 Y ,const float  alpha=1, const int incX=1 , const int incY=1 );
		static VrArrayPtrF32 scal_add(const int ndims, VrArrayPtrF32 X,float scal, const int incX=1);
		static VrArrayPtrF32 transpose(VrArrayPtrF32 A);
		static VrArrayPtrF32 scal_div( VrArrayPtrF32 A, float  scal);
		static VrArrayPtrF32 elem_mult(VrArrayPtrF32 A, VrArrayPtrF32 B);
		static VrArrayPtrF32 elem_div(VrArrayPtrF32 A, VrArrayPtrF32 B, bool zeroCheck=false);
		static VrArrayPtrF32 scal_minus(VrArrayPtrF32 A, float scal);
		static VrArrayPtrF32 elem_mult(VrArrayPtrF32 A, float scal) ;
		static VrArrayPtrF32 elem_mult(float scal,VrArrayPtrF32 A );
		static VrArrayPtrF32 vec_sub(const int ndims, const float alpha,VrArrayPtrF32 X);
		static VrArrayPtrF32 vec_sub(const int ndims, VrArrayPtrF32 X, const float alpha);
		static VrArrayPtrF32 mat_ldiv(int matrix_order, VrArrayPtrF32 A, VrArrayPtrF32 B);
		static VrArrayPtrF32 mat_rdiv(int matrix_order, VrArrayPtrF32 A, VrArrayPtrF32 B);
		

};

class BlasComplexSingle{
    public:
	    static VrArrayPtrCF32 mmult(CBLAS_ORDER,CBLAS_TRANSPOSE,CBLAS_TRANSPOSE,VrArrayPtrCF32 A, VrArrayPtrCF32 B,float complex  alpha=1 ,float complex beta=0 );
	    static VrArrayPtrCF32 vec_mult(CBLAS_ORDER, CBLAS_TRANSPOSE,VrArrayPtrCF32 A, VrArrayPtrCF32 X,float  complex alpha=1,int incx=1,int incy=1,float complex beta=0 );
	    static VrArrayPtrCF32 scal_mult(int ndims, VrArrayPtrCF32 X,float complex alpha);
	    static VrArrayPtrCF32 vec_add(int ndims, VrArrayPtrCF32 X, VrArrayPtrCF32 Y, const float complex  alpha=1,const int incx=1, int incY=1 );
	    static float vec_dot(const int ndims, VrArrayPtrCF32 X, VrArrayPtrCF32 Y, const int incX=1, const int incY=1);
	    static VrArrayPtrCF32 vec_copy(const int ndims, VrArrayPtrCF32 X, const int incX=1, const int incY=1);
	    static VrArrayPtrCF32 vec_sub(const int ndims, VrArrayPtrCF32 X, VrArrayPtrCF32 Y ,const float  complex alpha=NULL, const int incX=1 , const int  incY=1);
	    static VrArrayPtrCF32 scal_add(const int ndims, VrArrayPtrCF32 X, float complex scal, const int incX=1);
	    static VrArrayPtrCF32 scal_div( VrArrayPtrCF32 A, float complex  scal);
	    static VrArrayPtrCF32 elem_mult(VrArrayPtrCF32 A, VrArrayPtrCF32 B);
	    static VrArrayPtrCF32 elem_div(VrArrayPtrCF32 A, VrArrayPtrCF32 B, bool zeroCheck=false);
	    static VrArrayPtrCF32 scal_minus(VrArrayPtrCF32 A, float complex scal);
 	    static VrArrayPtrF32 elem_mult(VrArrayPtrF32 A, float complex  scal);
 	    static VrArrayPtrF32 elem_mult( float complex  scal, VrArrayPtrF64  A);
	    static VrArrayPtrCF32 transpose(VrArrayPtrCF32 A);
	    static VrArrayPtrCF32 mat_ldiv(int matrix_order, VrArrayPtrCF32 A, VrArrayPtrCF32 B);
	    static VrArrayPtrCF32 mat_rdiv(int matrix_order, VrArrayPtrCF32 A, VrArrayPtrCF32 B);
 };

 class BlasComplexDouble{
    public:
	    static VrArrayPtrCF64 mmult(CBLAS_ORDER,CBLAS_TRANSPOSE,CBLAS_TRANSPOSE,VrArrayPtrCF64 A, VrArrayPtrCF64 B,double complex alpha=1 ,double complex beta=0 );
	    static VrArrayPtrCF64 vec_mult(CBLAS_ORDER, CBLAS_TRANSPOSE,VrArrayPtrCF64 A, VrArrayPtrCF64 X,double complex alpha=1,int incx=1,int incy=1 ,double complex beta=0);
	    static VrArrayPtrCF64 scal_mult(int ndims, VrArrayPtrCF64 X,double complex alpha);
	    static VrArrayPtrCF64 vec_add(int ndims, VrArrayPtrCF64 X, VrArrayPtrCF64 Y, const double complex alpha=1,const int incx=1, int incy=1 );
	    static double vec_dot(const int ndims, VrArrayPtrCF64 X, VrArrayPtrCF64 Y, const int incX=1, const int incY=1);
	    static VrArrayPtrCF64 vec_copy(const int ndims, VrArrayPtrCF64 X, const int incX=1, const int incY=1);
	    static VrArrayPtrCF64 vec_sub(const int ndims, VrArrayPtrCF64 X, VrArrayPtrCF64 Y ,const double complex , const int incX=1 , const int incY=1 );
	    static VrArrayPtrCF64 scal_add(int ndims, VrArrayPtrCF64 X, const double complex scal, int incX=1);
	     static VrArrayPtrCF64 transpose(VrArrayPtrCF64 A);
	    static VrArrayPtrCF64 scal_div( VrArrayPtrCF64 A, double complex  scal);
	    static VrArrayPtrCF64 elem_mult(VrArrayPtrCF64 A, VrArrayPtrCF64 B);
	    static VrArrayPtrCF64 elem_div(VrArrayPtrCF64 A, VrArrayPtrCF64 B, bool zeroCheck=false);
	    static VrArrayPtrCF64 scal_minus(VrArrayPtrCF64 A, double  complex scal);
	    static VrArrayPtrCF64 mat_ldiv(int, VrArrayPtrCF64 A, VrArrayPtrCF64 B);
	    static VrArrayPtrCF64 mat_rdiv(int matrix_order, VrArrayPtrCF64 A, VrArrayPtrCF64 B); 
	    
 };
#endif
