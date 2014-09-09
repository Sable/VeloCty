#include"matrix_ops.hpp"
#include<vrbinding.hpp>
#include <iostream>

VrArrayPtrF32 BlasSingle::mmult(CBLAS_ORDER order, CBLAS_TRANSPOSE transA, CBLAS_TRANSPOSE transB, VrArrayPtrF32 A, VrArrayPtrF32 B, float alpha ,float beta){
	int dims[2];
	if (isScalar<VrArrayF32>(A) && !isScalar<VrArrayF32>(B)) {
	  return scal_mult(VR_GET_NDIMS_F32(B),B,*VR_GET_DATA_F32(A));
	}
	if (!isScalar<VrArrayF32>(A) && isScalar<VrArrayF32>(B)) {
	  return scal_mult(VR_GET_NDIMS_F32(A), A,*VR_GET_DATA_F32(B));
	}
	if(!checkdim(VR_GET_DIMS_F32(A)[1],VR_GET_DIMS_F32(B)[0])) { 
	  std::cout<<"Error matrix dimensions must agree"<<std::endl;
	}
	dims[0]=VR_GET_DIMS_F32(A)[0];
	dims[1]=VR_GET_DIMS_F32(B)[1];
	VrArrayPtrF32 C=vrAllocArrayF32RM(2,0,dims);
        cblas_sgemm(order, transA, transB,VR_GET_DIMS_F32(A)[0],VR_GET_DIMS_F32(B)[1],VR_GET_DIMS_F32(A)[1],alpha,VR_GET_DATA_F32(A),VR_GET_DIMS_F32(A)[0],VR_GET_DATA_F32(B),VR_GET_DIMS_F32(B)[0],beta,VR_GET_DATA_F32(C),VR_GET_DIMS_F32(C)[0]);
	//return NULL;
	return C;
}

VrArrayPtrF32 BlasSingle::vec_mult(CBLAS_ORDER order, CBLAS_TRANSPOSE transA, VrArrayPtrF32 A, VrArrayPtrF32 X,float alpha, int incX, int incY, float beta){
	int dims[2];
	dims[0]=VR_GET_DIMS_F32(A)[0];
	dims[1]=1;
	VrArrayPtrF32 Y=vrAllocArrayF32RM(2,0,(int*)dims);
	
        cblas_sgemv(order, transA , VR_GET_DIMS_F32(A)[0],VR_GET_DIMS_F32(A)[1],1, (float*)VR_GET_DATA_F32(A),1,(float*)VR_GET_DATA_F32(X), 1,1,(float*)VR_GET_DATA_F32(Y),1 );
	return Y;
}
VrArrayPtrF32 BlasSingle::scal_mult(int ndims,VrArrayPtrF32 X,float alpha){
        int N=1;
        for(int i=0;i<ndims;i++){
                N*=VR_GET_DIMS_F32(X)[i];
        }
	VrArrayPtrF32 Y;//=(VrArrayPtrF32) mxMalloc(sizeof(VrArrayF32)); //mxDuplicateArray(X);
        Y = vec_copy(ndims,X);
	cblas_sscal(N,alpha,(float*)VR_GET_DATA_F32(Y),1);
	return Y;
}
VrArrayPtrF32 BlasSingle::vec_add(int ndims, VrArrayPtrF32 X,VrArrayPtrF32 Y, const float alpha, const int incX,const int incY ){
        int N=1;
        for(int i=0;i<ndims;i++){
                N*=VR_GET_DIMS_F32(X)[i];
        }
	VrArrayPtrF32 Y1=vec_copy(ndims,Y);
        cblas_saxpy(N,alpha,(float*)VR_GET_DATA_F32(X),incX,(float*)VR_GET_DATA_F32(Y1),incY);
	return Y1;


}
float  BlasSingle::vec_dot(const int ndims , VrArrayPtrF32 X, VrArrayPtrF32 Y, const int incX, const int incY ){
         int N=1;
        for(int i=0;i<ndims;i++){
                N*=VR_GET_DIMS_F32(X)[i];
        }
	
        return cblas_sdot (N,(float*)VR_GET_DATA_F32(X),incX,(float*)VR_GET_DATA_F32(Y),incY);
}

VrArrayPtrF32 BlasSingle::vec_copy(const int ndims, VrArrayPtrF32 X,  const int incX , const int incY){
         int N=1;
        for(int i=0;i<ndims;i++){
                N*=VR_GET_DIMS_F32(X)[i];
        }
	VrArrayPtrF32 Y=vrAllocArrayF32RM(ndims,0,(int*)VR_GET_DIMS_F32(X));
        cblas_scopy (N, (float*)VR_GET_DATA_F32(X),incX, (float*)VR_GET_DATA_F32(Y),incY);
	return Y;
}

VrArrayPtrF32 BlasSingle::vec_sub(int ndims, VrArrayPtrF32 X, VrArrayPtrF32 Y , const float alpha, const int incX, const int incY){
        VrArrayPtrF32 X1=X;
        if(alpha!=1){
                VrArrayPtrF32 X1=vrAllocArrayF32RM(ndims,0,(int*)VR_GET_DIMS_F32(X));
                X1=BlasSingle::scal_mult(ndims,X,alpha);

        }
        return vec_add(ndims, Y,X1, -1);


}
 VrArrayPtrF32 BlasSingle::vec_sub(const int ndims, const float alpha,VrArrayPtrF32 X) {

    return BlasSingle::scal_minus(X,alpha);
}

 VrArrayPtrF32 BlasSingle::vec_sub(const int ndims, VrArrayPtrF32 X, const float alpha) { 

  return BlasSingle::scal_minus(X,alpha);
}
VrArrayPtrF32 BlasSingle::scal_add(const int ndims, VrArrayPtrF32 X,float scal, int incX ){
	int N=1;
	for(int i=0;i<ndims;i++){
		N*=VR_GET_DIMS_F32(X)[i];
	}
	VrArrayPtrF32 Y= vec_copy(ndims, X);
	VrArrayPtrF32 Z=vrAllocArrayF32RM(VR_GET_NDIMS_F32(X),1,VR_GET_DIMS_F32(X));
	cblas_saxpy(N,scal,VR_GET_DATA_F32(Z),1,VR_GET_DATA_F32(Y),incX);	
	
	return Y;
}	
 VrArrayPtrF32 BlasSingle::transpose(VrArrayPtrF32 A){
	 int dims[2];
	 dims[1]= VR_GET_DIMS_F32(A)[0];
	 dims[0]= VR_GET_DIMS_F32(A)[1];
	 VrArrayPtrF32 B;
	  if(VR_GET_DIMS_F64(A)[0]==1 || VR_GET_DIMS_F32(A)[1]==1) {
	 
	      B=vec_copy(VR_GET_NDIMS_F32(A),A);
	      dim_type temp= VR_GET_DIMS_F32(B)[0];
	      VR_GET_DIMS_F32(B)[0] =VR_GET_DIMS_F32(B)[1];
	      VR_GET_DIMS_F32(B)[1] = temp;
	      return B;
	 }
	 B =vrAllocArrayF32RM(VR_GET_NDIMS_F32(A),0,dims);
	 int row= VR_GET_DIMS_F32(A)[0];
	 int col= VR_GET_DIMS_F32(A)[1];
	 float *out =VR_GET_DATA_F32(B);
	 float *in =VR_GET_DATA_F32(A);
	 for(int i=0;i<row;i++){
	 
	   for(int j=0;j<col;j++){
	    
	       out[i*col+j] =  in[j*row+i];
	  }
	}
	return B;
}

VrArrayPtrF32 BlasSingle::scal_minus(VrArrayPtrF32 A, float scal) {
  return scal_add(VR_GET_NDIMS_F32(A), A,-scal);
}

VrArrayPtrF32 BlasSingle::scal_div( VrArrayPtrF32 A, float  scal){
  
  float * data, * out;
  VrArrayPtrF32 B = vrAllocArrayF32RM(VR_GET_NDIMS_F32(A), 0,VR_GET_DIMS_F32(A));
  data=VR_GET_DATA_F32(A);
  out= VR_GET_DATA_F32(B);
  for (int i=0;i<getNumElem(VR_GET_DIMS_F32(A),VR_GET_NDIMS_F32(A));i++) {
    out[i]=data[i]/scal;
  }
  return B;
}

VrArrayPtrF32 BlasSingle::elem_mult(VrArrayPtrF32 A, VrArrayPtrF32 B) {
  if ( !checkdims<VrArrayF32>(A,B) ) {
    std::cout<<"dimensions do not match. \n Exiting. "<<std::endl;
    exit(0);
  }
  VrArrayPtrF32 C = vec_copy(VR_GET_NDIMS_F32(A),A);
  for (int i = 0; i < getNumElem(VR_GET_DIMS_F32(A),VR_GET_NDIMS_F32(A)); i++) {
    VR_GET_DATA_F32(C)[i] *= VR_GET_DATA_F32(B)[i];
  }
  return C;
}

VrArrayPtrF32 BlasSingle::elem_mult(float scal, VrArrayPtrF32 A) {

 return scal_mult(VR_GET_NDIMS_F32(A), A, scal); 
  
}

VrArrayPtrF32 BlasSingle::elem_mult(VrArrayPtrF32 A, float scal) {

 return scal_mult(VR_GET_NDIMS_F32(A), A, scal); 
  
}
VrArrayPtrF32 BlasSingle::elem_div(VrArrayPtrF32 A, VrArrayPtrF32 B, bool zeroCheck) {
  if ( !checkdims<VrArrayF32>(A,B) ) {
    std::cout<<"dimensions do not match. \n Exiting. "<<std::endl;
    exit(0);
  }
  VrArrayPtrF32 C = vec_copy(VR_GET_NDIMS_F32(A),A);
  if(!zeroCheck){
   for (int i = 0; i < getNumElem(VR_GET_DIMS_F32(A),VR_GET_NDIMS_F32(A)); i++) {
     VR_GET_DATA_F32(C)[i] /= VR_GET_DATA_F32(B)[i];
   }
  }else {
    for (int i = 0; i < getNumElem(VR_GET_DIMS_F32(A),VR_GET_NDIMS_F32(A)); i++) {
     if (VR_GET_DATA_F32(B)[i]==0) {
       std::cout<<"divide by zero. \n Exiting"<<std::endl;
       exit(0);
     }
     VR_GET_DATA_F32(C)[i] /= VR_GET_DATA_F32(B)[i];
   }
  }
  return C;
}

VrArrayPtrF32 BlasSingle::mat_ldiv(int matrix_order, VrArrayPtrF32 A, VrArrayPtrF32 B) {
VrArrayPtrF32 C = vec_copy(VR_GET_NDIMS_F32(B),B);
   VrArrayPtrF32 D = vec_copy(VR_GET_NDIMS_F32(A),A);
   float * data= VR_GET_DATA_F32(D);
   float * out_data=VR_GET_DATA_F32(C);
   long int lda=(long int)VR_GET_DIMS_F32(D)[0];
   long int ldb= (long int)VR_GET_DIMS_F32(C)[0];
   long int n=(long int )VR_GET_DIMS_F32(D)[1];
   long int nrhs= (long int )VR_GET_DIMS_F32(C)[1];
  int  *IPIV=(int*)VR_MALLOC(sizeof(int)*n);
  long int info=0;
  

    info= LAPACKE_sgesv(LAPACK_COL_MAJOR,n,nrhs,data,lda,IPIV,out_data,ldb);
    
    
  
 return C;
  
}
VrArrayPtrF32 BlasSingle::mat_rdiv(int matrix_order, VrArrayPtrF32 A, VrArrayPtrF32 B) {  

  return transpose(mat_ldiv(LAPACK_COL_MAJOR,transpose(B),transpose(A)));
}
