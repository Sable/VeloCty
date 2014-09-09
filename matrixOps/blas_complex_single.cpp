#include "matrix_ops.hpp"
//#include<complex.h>
#include<iostream>

VrArrayPtrCF32 BlasComplexSingle::mmult(CBLAS_ORDER order, CBLAS_TRANSPOSE transA, CBLAS_TRANSPOSE transB, VrArrayPtrCF32 A, VrArrayPtrCF32 B,float complex  alpha,float complex beta) {
	int dims[2];
	dims[0]=VR_GET_DIMS_CF32(A)[0];
	dims[1]=VR_GET_DIMS_CF32(B)[1];
	VrArrayPtrCF32 C=vrAllocArrayF32CM(2,0,(int*)dims);
	//float alph[]={1,0};
	//float bet[]={0,0};
        cblas_cgemm(order, transA, transB,VR_GET_DIMS_CF32(A)[0],VR_GET_DIMS_CF32(B)[1],VR_GET_DIMS_CF32(A)[1],reinterpret_cast<float*>(&alpha),(float*)VR_GET_DATA_CF32(A),VR_GET_DIMS_CF32(A)[0],
        (float*)VR_GET_DATA_CF32(B),VR_GET_DIMS_CF32(B)[0],reinterpret_cast<float*>(&beta),(float*)VR_GET_DATA_CF32(C),VR_GET_DIMS_CF32(C)[0]);
	return C;
}

VrArrayPtrCF32 BlasComplexSingle::vec_mult(CBLAS_ORDER order, CBLAS_TRANSPOSE transA, VrArrayPtrCF32 A, VrArrayPtrCF32 X, float complex  alpha, int incX,int incY,float complex beta) {
	int dims[2];
	dims[0]=VR_GET_DIMS_CF32(A)[0];
	dims[1]=1;
	VrArrayPtrCF32 Y=vrAllocArrayF32CM(2,0,(int*)dims);
	
	const float alph[] = {1,0};
	const float bet[] = {0,0};
        cblas_cgemv(order, transA , VR_GET_DIMS_CF32(A)[0],VR_GET_DIMS_CF32(A)[1],(alph), (float*)VR_GET_DATA_CF32(A),VR_GET_DIMS_CF32(A)[0],(float*)VR_GET_DATA_CF32(X), incX,bet,(float*)VR_GET_DATA_CF32(Y),incY );
	return Y;
}

VrArrayPtrCF32 BlasComplexSingle::scal_mult(int ndims,VrArrayPtrCF32 X,float complex alpha) {
        int N=1;
        for(int i=0;i<ndims;i++){
                N*=VR_GET_DIMS_CF32(X)[i];
        }
	VrArrayPtrCF32 Y;//=(VrArrayPtrCF32)mxMalloc(sizeof(VrArrayPtrCF32));
	Y=vec_copy(ndims,X);
	//mxDuplicateArray(X);
        cblas_cscal(N,reinterpret_cast<float*>(&alpha),(float*)VR_GET_DATA_CF32(Y),1);
	return Y;
}

VrArrayPtrCF32 BlasComplexSingle::vec_add(int ndims, VrArrayPtrCF32 X,VrArrayPtrCF32 Y, const float complex alpha, const int incX,const int incY ) {
        int N=1;
	VrArrayPtrCF32 Y1=vec_copy(VR_GET_NDIMS_CF32(Y),Y);
        for(int i=0;i<ndims;i++){
                N*=VR_GET_DIMS_CF32(X)[i];
        }
		const float alph[] = {1,0};
        cblas_caxpy(N,alph,(float*)VR_GET_DATA_CF32(X),incX,(float*)VR_GET_DATA_CF32(Y1),incY);
	return Y1;


}
float  BlasComplexSingle::vec_dot(const int ndims , VrArrayPtrCF32 X, VrArrayPtrCF32 Y, const int incX, const int incY ){
//          int N=1;
//         for(int i=0;i<ndims;i++){
//                 N*=VR_GET_DIMS_CF32(X)[i];
//         }
// 	//TODO: Dot product implementation may not be right. Have to check . 	
//         float complex retVal=0;//cblas_cdotu (N,(float*)VR_GET_DATA_CF32(X),incX,(float*)VR_GET_DATA_CF32(Y),incY);
// 	return cimag(retVal);
}
VrArrayPtrCF32 BlasComplexSingle::vec_copy(const int ndims, VrArrayPtrCF32 X,  const int incX , const int incY ){
         int N=1;
        for(int i=0;i<ndims;i++){
                N*=VR_GET_DIMS_CF32(X)[i];
        }
	VrArrayPtrCF32 Y=vrAllocArrayF32CM(ndims,0,(int*)VR_GET_DIMS_CF32(X));
        cblas_ccopy (N, (float*)VR_GET_DATA_CF32(X),incX, (float*)VR_GET_DATA_CF32(Y),incY);
	return Y;
}

VrArrayPtrCF32 BlasComplexSingle::vec_sub(int ndims, VrArrayPtrCF32 X, VrArrayPtrCF32 Y , const float complex alpha, const int incX, const int incY) {
        VrArrayPtrCF32 X1 =vec_copy(VR_GET_NDIMS_CF32(X),X);
        /*if(cimag(alpha)!=1){
                VrArrayPtrCF32 X1=vrAllocArrayF32CM(ndims,0,(int*)VR_GET_DIMS_CF32(X));
                X1=BlasComplexSingle::scal_mult(ndims,X,alpha);

        }*/
	//float arr[]={-1,0};  
	float complex arr = -1 ; 
        return vec_add(ndims, Y,X1, arr);
}

VrArrayPtrCF32 BlasComplexSingle::scal_add(int ndims, VrArrayPtrCF32 X, float complex scal, const int incX ) {
	int N = getNumElem(VR_GET_DIMS_CF32(X), ndims);
	VrArrayPtrCF32 Y=vec_copy(ndims, X);
	float complex * out=VR_GET_DATA_CF32(Y);
	
	for(int i=0;i<N;i++){
		//out[2*i]+=scal[0];
		//out[2*i+1]=scal[1];
		out[i]+=scal;
	}	
	return Y;
}

VrArrayPtrCF32 BlasComplexSingle::scal_div( VrArrayPtrCF32 A, float complex  scal){
  
  float complex * data, * out;
  VrArrayPtrCF32 B= vec_copy(VR_GET_NDIMS_CF32(A), A);
  data=(float complex *)VR_GET_DATA_CF32(A);
  out= (float complex *)VR_GET_DATA_CF32(B);
  for(int i=0;i<getNumElem(VR_GET_DIMS_CF32(A),VR_GET_NDIMS_CF32(A));i++){
  
  //  out[2*i]=data[i]/scal[0];
   // out[2*i+1]=data[2*i+1]/scal[1];
	VR_GET_DATA_CF32(B)[i]= VR_GET_DATA_CF32(A)[i]/scal;
	//mexPrintf("%f %f",creal(out[i]),cimag(out[i]));
  }
  return B;
}
VrArrayPtrCF32 BlasComplexSingle::transpose(VrArrayPtrCF32 A) {
	 VrArrayPtrCF32 B;
	 int dims[2];
	 if(VR_GET_DIMS_CF32(A)[0]==1 || VR_GET_DIMS_CF32(A)[1]==1) {
	 
	      B=vec_copy(VR_GET_NDIMS_CF32(A),A);
	      dim_type temp= VR_GET_DIMS_CF32(B)[0];
	      VR_GET_DIMS_CF32(B)[0] =VR_GET_DIMS_CF32(B)[1];
	      VR_GET_DIMS_CF32(B)[1] = temp;
	      return B;
	 }
	 dims[1]=VR_GET_DIMS_CF32(A)[0];
	 dims[0]=VR_GET_DIMS_CF32(A)[1];
	 B= vrAllocArrayF32CM(2,0,dims);
	 int row= VR_GET_DIMS_CF32(A)[0];
	 int col= VR_GET_DIMS_CF32(A)[1];
	 float complex *out =VR_GET_DATA_CF32(B);
	 float complex *in =VR_GET_DATA_CF32(A);
	 for(int i=0;i<row;i++){
	   for(int j=0;j<col;j++) {
	   
	     out[i*col+j] = conj(in[j*row +i]);
	  }
	  
	 }
	return B;
}
VrArrayPtrCF32 BlasComplexSingle::elem_mult(VrArrayPtrCF32 A, VrArrayPtrCF32 B) {
  if ( !checkdims<VrArrayCF32>(A,B) ) {
    std::cout<<"dimensions do not match. \n Exiting. "<<std::endl;
    exit(0);
  }
  VrArrayPtrCF32 C = vec_copy(VR_GET_NDIMS_CF32(A),A);
  for (int i = 0; i < getNumElem(VR_GET_DIMS_CF32(A),VR_GET_NDIMS_CF32(A)); i++) {
    VR_GET_DATA_CF32(C)[i] *= VR_GET_DATA_CF32(B)[i];
  }
  return C;
}

VrArrayPtrCF32 BlasComplexSingle::scal_minus(VrArrayPtrCF32 A, float complex scal) {
  //scal[0]=-scal[0];
  //scal[1]=-scal[1];
  scal = -creal(scal) - cimag(scal)*I;
  return scal_add(VR_GET_NDIMS_CF32(A), A,scal);
}

VrArrayPtrCF32 BlasComplexSingle::elem_div(VrArrayPtrCF32 A, VrArrayPtrCF32 B, bool zeroCheck) {
  if ( !checkdims<VrArrayCF32>(A,B) ) {
    std::cout<<"dimensions do not match. \n Exiting. "<<std::endl;
    exit(0);
  }
  VrArrayPtrCF32 C = vec_copy(VR_GET_NDIMS_CF32(A),A);
  if(!zeroCheck){
   for (int i = 0; i < getNumElem(VR_GET_DIMS_CF32(A),VR_GET_NDIMS_CF32(A)); i++) {
     VR_GET_DATA_CF32(C)[i] /= VR_GET_DATA_CF32(B)[i];
   }
  }else {
    for (int i = 0; i < getNumElem(VR_GET_DIMS_CF32(A),VR_GET_NDIMS_CF32(A)); i++) {
     if (VR_GET_DATA_CF32(B)[i]==0) {
       std::cout<<"divide by zero. \n Exiting"<<std::endl;
       exit(0);
     }
     VR_GET_DATA_CF32(C)[i] /= VR_GET_DATA_CF32(B)[i];
   }
  }
  return C;
}

VrArrayPtrCF32 BlasComplexSingle::mat_ldiv(int matrix_order, VrArrayPtrCF32 A, VrArrayPtrCF32 B) {
VrArrayPtrCF32 C = vec_copy(VR_GET_NDIMS_CF32(B),B);
   VrArrayPtrCF32 D = vec_copy(VR_GET_NDIMS_CF32(A),A);
   float complex * data= VR_GET_DATA_CF32(D);
   float complex * out_data=VR_GET_DATA_CF32(C);
   long int lda=(long int)VR_GET_DIMS_CF32(D)[0];
   long int ldb= (long int)VR_GET_DIMS_CF32(C)[0];
   long int n=(long int )VR_GET_DIMS_CF32(D)[1];
   long int nrhs= (long int )VR_GET_DIMS_CF32(C)[1];
  int  *IPIV=(int*)VR_MALLOC(sizeof(int)*n);
  long int info=0;
  

    info= LAPACKE_cgesv(LAPACK_COL_MAJOR,n,nrhs,data,lda,IPIV,out_data,ldb);
    
    
  
 return C;
  
}
VrArrayPtrCF32 BlasComplexSingle::mat_rdiv(int matrix_order, VrArrayPtrCF32 A, VrArrayPtrCF32 B) {  

  return transpose(mat_ldiv(LAPACK_COL_MAJOR,transpose(B),transpose(A)));
}
