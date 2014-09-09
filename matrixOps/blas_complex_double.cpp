#include "matrix_ops.hpp"
#include <cblas.h>
#include <iostream>
#include <complex.h>
#include <string.h>
#include <lapacke.h>
//#define DEBUG

VrArrayPtrCF64 BlasComplexDouble::mmult(CBLAS_ORDER order, CBLAS_TRANSPOSE transA, CBLAS_TRANSPOSE transB, VrArrayPtrCF64 A, VrArrayPtrCF64 B,double complex alpha,double complex beta){
	int dims[2];
	if(!checkdim(VR_GET_DIMS_CF64(A)[1],VR_GET_DIMS_CF64(B)[0])) { 
	  std::cout<<"Error matrix dimensions must agree"<<std::endl;
	}
	dims[0]=VR_GET_DIMS_CF64(A)[0];
	dims[1]=VR_GET_DIMS_CF64(B)[1];
	const double alph[]={1,0},bet[]={0,0};
	VrArrayPtrCF64 C=vrAllocArrayF64CM(2,0,(int*)dims);
#ifdef DEBUG
	std::cout<<"dimensions"<<VR_GET_DIMS_CF64(C)[0]<<" "<<VR_GET_DIMS_CF64(C)[1]<<std::endl;
#endif
        cblas_zgemm(order, transA, transB,(int)VR_GET_DIMS_CF64(A)[0],(int)VR_GET_DIMS_CF64(B)[1],(int)VR_GET_DIMS_CF64(A)[1],(alph),(double*)VR_GET_DATA_CF64(A),(int)VR_GET_DIMS_CF64(A)[0],(double*)VR_GET_DATA_CF64(B),(int)VR_GET_DIMS_CF64(B)[0],(bet),reinterpret_cast<double*>(VR_GET_DATA_CF64(C)),(int)VR_GET_DIMS_CF64(C)[0]);
#ifdef DEBUG
//	for(int i=0;i<C->dims[0]*dims[1];i++){
//		std::cout<<C->data[2*i]<<" "<<C->data[2*i+1]<<std::endl;
//	}
#endif
	return C;
}

VrArrayPtrCF64 BlasComplexDouble::vec_mult(CBLAS_ORDER order, CBLAS_TRANSPOSE transA, VrArrayPtrCF64 A, VrArrayPtrCF64 X,double  complex alpha , int incX,int incY,double complex beta){
	int dims[2];
	dims[0]=VR_GET_DIMS_CF64(A)[0];
	dims[1]=1;
	double alph[] = {1,0};
	double bet[] = {0,0};
	VrArrayPtrCF64 Y=vrAllocArrayF64CM(2,0,(int*)dims);
        cblas_zgemv(order, transA , VR_GET_DIMS_CF64(A)[0],VR_GET_DIMS_CF64(A)[1],alph, (double*)VR_GET_DATA_CF64(A),VR_GET_DIMS_CF64(A)[0],(double*)VR_GET_DATA_CF64(X), incX,bet,(double*)VR_GET_DATA_CF64(Y),incY);
	return Y;
}

VrArrayPtrCF64 BlasComplexDouble::scal_mult(int ndims,VrArrayPtrCF64 X,double complex alpha){
        int N=1;
        for(int i=0;i<ndims;i++){
                N*=VR_GET_DIMS_CF64(X)[i];
        }
	//mexPrintf("%d",N);
	double alph[] = {1,0};
	VrArrayPtrCF64 Y=vec_copy(ndims,X);
    cblas_zscal(N,(alph),(double*)VR_GET_DATA_CF64(Y),1);
	return Y;
}

//TODO: the parameter alpha is not used. 
VrArrayPtrCF64 BlasComplexDouble::vec_add(int ndims, VrArrayPtrCF64 X,VrArrayPtrCF64 Y, const double  complex alpha, const int incX,const int incY ){
        int N=1;
        for(int i=0;i<ndims;i++){
                N*=VR_GET_DIMS_CF64(X)[i];
        }
	double alph[]={1,0};
		
	VrArrayPtrCF64 Y1=vec_copy(ndims,Y);
        cblas_zaxpy(N,alph,reinterpret_cast<double*>(VR_GET_DATA_CF64(X)),1,reinterpret_cast<double*>(VR_GET_DATA_CF64(Y1)),1);
	return Y1;
	

}

VrArrayPtrCF64 BlasComplexDouble::scal_minus(VrArrayPtrCF64 A, double complex scal) {
 // scal[0]=-scal[0];
 //scal[1]=-scal[1];
 scal = -creal(scal)-(cimag(scal)*I);
  return scal_add(VR_GET_NDIMS_CF64(A) , A,scal);
}

//TODO: Implement . 
double  BlasComplexDouble::vec_dot(const int ndims,VrArrayPtrCF64 X, VrArrayPtrCF64 Y, const int incX, const int incY ){
//          int N=1;
//         for(int i=0;i<ndims;i++){
//                 N*=VR_GET_DIMS_CF64(X)[i];
//         }
// 	
//         double complex retVal= 0;//cblas_zdotu(N,(double*)VR_GET_DATA_CF64(X),incX,(double*)VR_GET_DATA_CF64(Y),incY);
// 	return cimag(retVal);
}
VrArrayPtrCF64 BlasComplexDouble::vec_copy(const int ndims, VrArrayPtrCF64 X,  const int incX , const int incY ){
         int N=1;
        for(int i=0;i<ndims;i++){
                N*=VR_GET_DIMS_CF64(X)[i];
        }
	VrArrayPtrCF64 Y=vrAllocArrayF64CM(ndims,0,(int*)VR_GET_DIMS_CF64(X));
        cblas_zcopy (N, (double*)VR_GET_DATA_CF64(X),incX, (double*)VR_GET_DATA_CF64(Y),incY);
	
	return Y;
}

VrArrayPtrCF64 BlasComplexDouble::vec_sub(int ndims, VrArrayPtrCF64 X, VrArrayPtrCF64 Y ,const double complex alpha, const int incX, const int incY){
        VrArrayPtrCF64 X1=X;
        /*if(alpha[0]!=1){
                VrArrayPtrCF64 X1=vrAllocArrayF64CM(ndims,0,(int*)VR_GET_DIMS_CF64(X));
                X1=BlasComplexDouble::scal_mult(ndims,X,(double*)alpha);

        }*/
	const double arr[]={-1,0};
	//double complex arr = -1 ;
	int N=1;
        for(int i=0;i<ndims;i++){
                N*=VR_GET_DIMS_CF64(X)[i];
        }
        //std::cout<<"N="<<N<<std::endl;
        //double alph[]={1,0};

        VrArrayPtrCF64 Y1=vec_copy(ndims,X);
        cblas_zaxpy(N,(arr),reinterpret_cast<double*>(VR_GET_DATA_CF64(Y)),1,reinterpret_cast<double*>(VR_GET_DATA_CF64(Y1)),1);
	//double complex alph = 1+0*I;	
        //VrArrayPtrCF64 Y1= scal_mult(Y->ndims,Y,arr);// vec_copy(ndims,X);
       // cblas_zaxpy(N,&alph,VR_GET_DATA_CF64(X),1,VR_GET_DATA_CF64(Y1),1);
        return Y1;//BlasComplexDouble::vec_add(ndims, Y,X1, arr);
}

VrArrayPtrCF64 BlasComplexDouble::scal_add(int ndims, VrArrayPtrCF64 X, const double complex scal, int incX){
   int N=1;
    for(int i=0;i<ndims;i++){
                N*=VR_GET_DIMS_CF64(X)[i];
    }
  VrArrayPtrCF64 X1=vec_copy(ndims, X);
 // VrArrayPtrCF64 Y= vrAllocArrayF64CM(ndims,1,X->dims);
  double complex * out=(double complex *)VR_GET_DATA_CF64(X1);  
  double complex * in =(double complex *)VR_GET_DATA_CF64(X);
  for(int i=0;i<VR_GET_DIMS_CF64(X1)[0]*VR_GET_DIMS_CF64(X1)[0];i++){
	//out[2*i]=in[2*i]+scal[0];
	//out[2*i+1]=in[2*i+1]+scal[1];
	out[i]=in[i]+scal;
  }
  return X1;
}

VrArrayPtrCF64 BlasComplexDouble::transpose(VrArrayPtrCF64 A){
	 VrArrayPtrCF64 B;
	 int dims[2];
	 if(VR_GET_DIMS_F64(A)[0]==1 || VR_GET_DIMS_F64(A)[1]==1) {
	 
	      B=vec_copy(VR_GET_NDIMS_CF64(A),A);
	      dim_type temp= VR_GET_DIMS_CF64(B)[0];
	      VR_GET_DIMS_CF64(B)[0] =VR_GET_DIMS_CF64(B)[1];
	      VR_GET_DIMS_CF64(B)[1] = temp;
	      return B;
	 }
	 dims[1]=VR_GET_DIMS_CF64(A)[0];
	 dims[0]=VR_GET_DIMS_CF64(A)[1];
	 B= vrAllocArrayF64CM(2,0,dims);
	 int row= VR_GET_DIMS_CF64(A)[0];
	 int col= VR_GET_DIMS_CF64(A)[1];
	 double complex *out =VR_GET_DATA_CF64(B);
	 double complex *in =VR_GET_DATA_CF64(A);
	 for(int i=0;i<row;i++){
	   for(int j=0;j<col;j++) {
	   
	     out[i*col+j] = conj(in[j*row +i]);
	  }
	  
	 }
	return B;
  
}

VrArrayPtrCF64 BlasComplexDouble::scal_div( VrArrayPtrCF64 A, double complex  scal){
  
  double complex  * data, * out;
  VrArrayPtrCF64 B= vec_copy(VR_GET_NDIMS_CF64(A), A);
  data=(double complex*)VR_GET_DATA_CF64(A);
  out= (double complex*)VR_GET_DATA_CF64(B);
  for(int i=0;i<getNumElem(VR_GET_DIMS_CF64(A),VR_GET_NDIMS_CF64(A));i++){
  
    out[i]=data[i]/scal;
    //out[2*i+1]=data[2*i+1]/scal[1];
  }
  return B;
}

VrArrayPtrCF64 BlasComplexDouble::elem_mult(VrArrayPtrCF64 A, VrArrayPtrCF64 B) {
  if ( !checkdims<VrArrayCF64>(A,B) ) {
    std::cout<<"dimensions do not match. \n Exiting. "<<std::endl;
    exit(0);
  }
  VrArrayPtrCF64 C = vec_copy(VR_GET_NDIMS_CF64(A),A);
  for (int i = 0; i < getNumElem(VR_GET_DIMS_CF64(A),VR_GET_NDIMS_CF64(A)); i++) {
    VR_GET_DATA_CF64(C)[i] *= VR_GET_DATA_CF64(B)[i];
   
  }
  return C;
}

VrArrayPtrCF64 BlasComplexDouble::elem_div(VrArrayPtrCF64 A, VrArrayPtrCF64 B, bool zeroCheck) {
  if ( !checkdims<VrArrayCF64>(A,B) ) {
    std::cout<<"dimensions do not match. \n Exiting. "<<std::endl;
    exit(0);
  }
  VrArrayPtrCF64 C = vec_copy(VR_GET_NDIMS_CF64(A),A);
  if(!zeroCheck){
   for (int i = 0; i < getNumElem(VR_GET_DIMS_CF64(A),VR_GET_NDIMS_CF64(A)); i++) {
     VR_GET_DATA_CF64(C)[i] /= VR_GET_DATA_CF64(B)[i];
     
   }
  }else {
    for (int i = 0; i < getNumElem(VR_GET_DIMS_CF64(A),VR_GET_NDIMS_CF64(A)); i++) {
     if (VR_GET_DATA_CF64(B)[i]==0) {
       std::cout<<"divide by zero. \n Exiting"<<std::endl;
       exit(0);
     }
     VR_GET_DATA_CF64(C)[i] /= VR_GET_DATA_CF64(B)[i];
    
   }
  }
  return C;
}
VrArrayPtrCF64 BlasComplexDouble::mat_ldiv(int matrix_order ,VrArrayPtrCF64 A, VrArrayPtrCF64 B) {

    VrArrayPtrCF64 C = vec_copy(VR_GET_NDIMS_CF64(B),B);
   VrArrayPtrCF64 D = vec_copy(VR_GET_NDIMS_CF64(A),A);
   double complex* data= VR_GET_DATA_CF64(D);
   double complex * out_data=VR_GET_DATA_CF64(C);
   long int lda=(long int)VR_GET_DIMS_CF64(D)[0];
   long int ldb= (long int)VR_GET_DIMS_CF64(C)[0];
   long int n=(long int )VR_GET_DIMS_CF64(D)[1];
   long int nrhs= (long int )VR_GET_DIMS_CF64(C)[1];
  int  *IPIV=(int*)VR_MALLOC(sizeof(int)*n);
  long int info=0;
  
 // dgesv_(&n,&nrhs,data,&lda,IPIV,out_data,&ldb,&info);
    info= LAPACKE_zgesv(LAPACK_COL_MAJOR,n,nrhs,data,lda,IPIV,out_data,ldb);

  
 return C; 
}
VrArrayPtrCF64 BlasComplexDouble::mat_rdiv(int matrix_order, VrArrayPtrCF64 A, VrArrayPtrCF64 B) {

  return transpose(mat_ldiv(LAPACK_COL_MAJOR,transpose(B),transpose(A)));
}

