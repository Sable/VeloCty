#include "matrix_ops.hpp"
#include <string.h>
#include <iostream>
#include<lapacke.h>

VrArrayPtrF64 BlasDouble::mmult(CBLAS_ORDER order, CBLAS_TRANSPOSE transA, CBLAS_TRANSPOSE transB, VrArrayPtrF64 A, VrArrayPtrF64 B,float alpha ,float beta){
	int dims[2];
	if (isScalar<VrArrayF64>(A) && !isScalar<VrArrayF64>(B)) {
	  return scal_mult(VR_GET_NDIMS_F64(B),B,*VR_GET_DATA_F64(A));
	}
	if (!isScalar<VrArrayF64>(A) && isScalar<VrArrayF64>(B)) {
	  return scal_mult(VR_GET_NDIMS_F64(A), A,*VR_GET_DATA_F64(B));
	}
	if(!checkdim(VR_GET_DIMS_F64(A)[1],VR_GET_DIMS_F64(B)[0])) { 
	  VR_PRINT_ERR("Error matrix dimensions must agree");
	}
	dims[0]=VR_GET_DIMS_F64(A)[0];
	dims[1]=VR_GET_DIMS_F64(B)[1];
	VrArrayPtrF64 C=vrAllocArrayF64RM(2,0,(int*)dims);	
	cblas_dgemm(order, transA, transB,VR_GET_DIMS_F64(A)[0],VR_GET_DIMS_F64(B)[1],VR_GET_DIMS_F64(A)[1],alpha,VR_GET_DATA_F64(A),VR_GET_DIMS_F64(A)[0],
	VR_GET_DATA_F64(B),VR_GET_DIMS_F64(B)[0],beta,VR_GET_DATA_F64(C),VR_GET_DIMS_F64(C)[0]);
	return C;
}

void BlasDouble::mmult(CBLAS_ORDER order, CBLAS_TRANSPOSE transA, CBLAS_TRANSPOSE transB, VrArrayPtrF64 A, VrArrayPtrF64 B,VrArrayPtrF64 *C, float alpha ,float beta){
	if (isScalar<VrArrayF64>(A) && !isScalar<VrArrayF64>(B)) {
	   scal_mult(VR_GET_NDIMS_F64(B),B,*VR_GET_DATA_F64(A),C);
	}
	if (!isScalar<VrArrayF64>(A) && isScalar<VrArrayF64>(B)) {
	   scal_mult(VR_GET_NDIMS_F64(A), A,*VR_GET_DATA_F64(B),C);
	}
	if(!checkdim(VR_GET_DIMS_F64(A)[1],VR_GET_DIMS_F64(B)[0])) { 
	  VR_PRINT_ERR("Error matrix dimensions must agree");
	}
    if(C->data == NULL || C->dims[0] < A.dims[0] || C->dims[1] < B.dims[1]){
        int dims[2];
        dims[0]=VR_GET_DIMS_F64(A)[0];
        dims[1]=VR_GET_DIMS_F64(B)[1] ;
        C->data = static_cast<double*>(VR_MALLOC(sizeof(double) * dims[0] * dims[1]));
        C->dims = static_cast<dim_type*>(VR_MALLOC(sizeof(dim_type) * 2));
        C->ndims = 2;
        memcpy(C->dims,dims,sizeof(dim_type)*2);
    }
	cblas_dgemm(order, transA, transB,VR_GET_DIMS_F64(A)[0],VR_GET_DIMS_F64(B)[1],VR_GET_DIMS_F64(A)[1],alpha,VR_GET_DATA_F64(A),VR_GET_DIMS_F64(A)[0],
	VR_GET_DATA_F64(B),VR_GET_DIMS_F64(B)[0],beta,VR_GET_DATA_F64((*C)),VR_GET_DIMS_F64((*C))[0]);
}
void BlasDouble::mmult(CBLAS_ORDER order, CBLAS_TRANSPOSE transA, CBLAS_TRANSPOSE transB, VrArrayPtrF64 A, VrArrayPtrF64 B,double *val,float alpha ,float beta){
	int dims[2];
	if(!checkdim(VR_GET_DIMS_F64(A)[1],VR_GET_DIMS_F64(B)[0])) { 
	  std::cout<<"Error matrix dimensions must agree"<<std::endl;
	}
	cblas_dgemm(order, transA, transB,VR_GET_DIMS_F64(A)[0],VR_GET_DIMS_F64(B)[1],VR_GET_DIMS_F64(A)[1],alpha,VR_GET_DATA_F64(A),VR_GET_DIMS_F64(A)[0],
	VR_GET_DATA_F64(B),VR_GET_DIMS_F64(B)[0],beta,val,1);
}

VrArrayPtrF64 BlasDouble::vec_mult(CBLAS_ORDER order, CBLAS_TRANSPOSE transA, VrArrayPtrF64 A, VrArrayPtrF64 X,float alpha, int incX , int incY, float beta){
	int dims[2];
	dims[0]=VR_GET_DIMS_F64(A)[0];
	dims[1]=1;
	VrArrayPtrF64 Y=vrAllocArrayF64RM(2,0,(int*)dims);
	cblas_dgemv(order, transA , VR_GET_DIMS_F64(A)[0],VR_GET_DIMS_F64(A)[1],1, VR_GET_DATA_F64(A),1,VR_GET_DATA_F64(X), 1,1,VR_GET_DATA_F64(Y),1 );
	return Y;
}

VrArrayPtrF64 BlasDouble::scal_mult(int ndims,VrArrayPtrF64 X,double alpha,VrArrayPtrF64 *Y) {
	int N = 1;
	for(int i = 0; i < X.ndims; i++){
		N *= VR_GET_DIMS_F64(X)[i];
	}
	BlasDouble::vec_copy(ndims,X,Y);
	cblas_dscal(N,alpha,VR_GET_DATA_F64((*Y)),1);
	return Y;
}

VrArrayPtrF64 BlasDouble::scal_mult(int ndims,VrArrayPtrI64 X,double alpha,VrArrayPtrF64 *Y) {
	int N = 1;
	for(int i = 0; i < X.ndims; i++){
		N *= VR_GET_DIMS_F64(X)[i];
	}
	BlasDouble::vec_copy(ndims,X,Y);
	cblas_dscal(N,alpha,VR_GET_DATA_F64((*Y)),1);
	return Y;
}

VrArrayPtrF64 BlasDouble::scal_mult(int ndims,VrArrayPtrF64 X,double alpha) {
	int N = 1;
	for(int i = 0; i < X.ndims; i++){
		N *= VR_GET_DIMS_F64(X)[i];
	}
	VrArrayPtrF64 Y; 
     vec_copy(ndims,X,&Y);
	cblas_dscal(N,alpha,VR_GET_DATA_F64(Y),1);
	return Y;
}

VrArrayPtrF64 BlasDouble::vec_add(int ndims, VrArrayPtrF64 X,VrArrayPtrF64 Y, const double alpha, const int incX,const int incY){
    int N=1;
    for(int i=0;i<X.ndims;i++){
        N*=VR_GET_DIMS_F64(X)[i];	
    }
    VrArrayPtrF64 Y1 = vec_copy(Y.ndims,Y);
    cblas_daxpy(N,alpha,VR_GET_DATA_F64(X),incX,VR_GET_DATA_F64(Y1),incY);
    return Y1;
}

void BlasDouble::vec_add(int ndims, VrArrayPtrF64 X,VrArrayPtrF64 Y,VrArrayPtrF64 *Z,  const double alpha, const int incX ,const int incY ){
    int N=1;
    for(int i=0;i<X.ndims;i++){
        N*=VR_GET_DIMS_F64(X)[i];	
    }
    BlasDouble::vec_copy(Y.ndims,Y,Z);
    cblas_daxpy(N,alpha,VR_GET_DATA_F64(X),incX,VR_GET_DATA_F64((*Z)),incY);
}

double  BlasDouble::vec_dot(const int ndims , VrArrayPtrF64 X, VrArrayPtrF64 Y, const int incX, const int incY){
    int N=1;
    for(int i=0;i<ndims;i++){
        N*=VR_GET_DIMS_F64(X)[i];
    }
    return cblas_ddot (N,VR_GET_DATA_F64(X),incX,VR_GET_DATA_F64(Y),incY);	
}

VrArrayPtrF64 BlasDouble::vec_copy(const int ndims, VrArrayPtrF64 X, const int incX , const int incY ){
    int N=1;
    for(int i=0;i<X.ndims;i++){
        N*=VR_GET_DIMS_F64(X)[i];
    }
    VrArrayPtrF64 Y=vrAllocArrayF64RM(X.ndims,0,VR_GET_DIMS_F64(X));
    cblas_dcopy (N, VR_GET_DATA_F64(X),incX, VR_GET_DATA_F64(Y),incY);
    return Y;
}

void BlasDouble::vec_copy(const int ndims, VrArrayPtrF64 X,VrArrayPtrF64 *Y, const int incX , const int incY ){
    int N=1;
    int N_y =1;
    for(int i=0;i<X.ndims;i++){
        N*=VR_GET_DIMS_F64(X)[i];
    }
    for(int i=0;i<Y->ndims;i++){
        N_y*=VR_GET_DIMS_F64((*Y))[i];
    }
    if(Y->data ==NULL || Y->ndims < X.ndims || N_y < N){
        *Y=vrAllocArrayF64RM(X.ndims,0,VR_GET_DIMS_F64(X));
    }
    cblas_dcopy (N, VR_GET_DATA_F64(X),incX, VR_GET_DATA_F64((*Y)),incY);
}

void BlasDouble::vec_copy(const int ndims, VrArrayPtrI64 X,VrArrayPtrF64 *Y, const int incX , const int incY ){
    int N=1;
    int N_y =1;
    for(int i=0;i<X.ndims;i++){
        N*=VR_GET_DIMS_F64(X)[i];
    }
    for(int i=0;i<Y->ndims;i++){
        N_y*=VR_GET_DIMS_F64((*Y))[i];
    }
    if(Y->data ==NULL || Y->ndims < X.ndims || N_y < N){
        *Y=vrAllocArrayF64RM(X.ndims,0,VR_GET_DIMS_F64(X));
    } else {
        memcpy(Y->dims, X.dims,sizeof(dim_type)*X.ndims);
    }
    for(int i = 0; i < N; i++) {
        Y->data[i] = static_cast<double>(X.data[i]);
    }
}

VrArrayPtrF64 BlasDouble::vec_sub(int ndims, VrArrayPtrF64 X, VrArrayPtrF64 Y , const double alpha, const int incX, const int incY){
	VrArrayPtrF64 X1=X;
	if(alpha!=1){
		X1=BlasDouble::scal_mult(ndims,X,alpha);
	}
	return vec_add(ndims, Y,X1, -1);
}

void BlasDouble::vec_sub(int ndims, VrArrayPtrF64 X, VrArrayPtrF64 Y,VrArrayPtrF64 *Z , const double alpha, const int incX, const int incY){
	VrArrayPtrF64 X1=X;
	if(alpha!=1){
		BlasDouble::scal_mult(ndims,X,alpha,Z);
	}
	BlasDouble::vec_add(ndims, Y,X1,Z, -1);
}
 VrArrayPtrF64 BlasDouble::vec_sub(const int ndims, const double alpha,VrArrayPtrF64 X) {

    return BlasDouble::scal_minus(X,alpha);
}
 void BlasDouble::vec_sub(const int ndims, const double alpha,VrArrayPtrF64 X,VrArrayPtrF64 *Y) {
     BlasDouble::scal_minus(X,alpha,Y);
}

 VrArrayPtrF64 BlasDouble::vec_sub(const int ndims, VrArrayPtrF64 X, const double alpha) { 
  return BlasDouble::scal_minus(X,alpha);
}

void BlasDouble::vec_sub(const int ndims, VrArrayPtrF64 X, const double alpha,VrArrayPtrF64 *Y) { 
     BlasDouble::scal_minus(X,alpha,Y);
}

VrArrayPtrF64 BlasDouble::scal_add(const int ndims, VrArrayPtrF64 X,double scal, int incX ){
	int N=1;
	for(int i=0;i<ndims;i++){
		N*=VR_GET_DIMS_F64(X)[i];
	}
	VrArrayPtrF64 Y= vec_copy(ndims, X);
    for( int  i = 0; i < N; i++){
        Y.data[i] += scal; 
    } 
	return Y;
}	

void BlasDouble::scal_add(const int ndims, VrArrayPtrF64 X,double scal, VrArrayPtrF64 *Y, int incX){
    int N=1;
    for(int i=0;i<ndims;i++){
        N*=VR_GET_DIMS_F64(X)[i];
    }
    vec_copy(ndims, X,Y);
    for( int  i = 0; i < N; i++){
        Y->data[i] += scal; 
    } 
}	

void BlasDouble::transpose(VrArrayPtrF64 A,VrArrayPtrF64 *B) {
    int dims[2];
    if(VR_GET_DIMS_F64(A)[0]==1 || VR_GET_DIMS_F64(A)[1]==1) {
	      vec_copy(VR_GET_NDIMS_F64(A),A,B);
	      dim_type temp= VR_GET_DIMS_F64((*B))[0];
	      VR_GET_DIMS_F64((*B))[0] = VR_GET_DIMS_F64((*B))[1];
	      VR_GET_DIMS_F64((*B))[1] = temp;
	 }
     if(B->data == NULL || B->dims[0] < A.dims[1] ||  B->dims[1] < A.dims[0]){
         dims[1]= VR_GET_DIMS_F64(A)[0];
         dims[0]= VR_GET_DIMS_F64(A)[1];
         *B =vrAllocArrayF64RM(VR_GET_NDIMS_F64(A),0,dims);
     } else {
        B->dims[0] = A.dims[1];
        B->dims[1] = A.dims[0];
        B->ndims = A.ndims;
     }
	 int row= VR_GET_DIMS_F64(A)[0];
	 int col= VR_GET_DIMS_F64(A)[1];
	 double *out =VR_GET_DATA_F64((*B));
	 double *in =VR_GET_DATA_F64(A);
	 for(int i=0;i<row;i++) {
	 	for(int j=0;j<col;j++) {	    
	      		 out[i*col+j] =  in[j*row+i];        
		}
	 }
}
VrArrayPtrF64 BlasDouble::transpose(VrArrayPtrF64 A) {
    int dims[2];
    VrArrayPtrF64 B;
    if(VR_GET_DIMS_F64(A)[0]==1 || VR_GET_DIMS_F64(A)[1]==1) {

	      B=vec_copy(VR_GET_NDIMS_F64(A),A);
	      dim_type temp= VR_GET_DIMS_F64(B)[0];
	      VR_GET_DIMS_F64(B)[0] = VR_GET_DIMS_F64(B)[1];
	      VR_GET_DIMS_F64(B)[1] = temp;
	      return B;
	 }
	 dims[1]= VR_GET_DIMS_F64(A)[0];
	 dims[0]= VR_GET_DIMS_F64(A)[1];
	 B =vrAllocArrayF64RM(VR_GET_NDIMS_F64(A),0,dims);
	 int row= VR_GET_DIMS_F64(A)[0];
	 int col= VR_GET_DIMS_F64(A)[1];
	 double *out =VR_GET_DATA_F64(B);
	 double *in =VR_GET_DATA_F64(A);
	 for(int i=0;i<row;i++) {
	 	for(int j=0;j<col;j++) {	    
	      		 out[i*col+j] =  in[j*row+i];        
		}
	}
	return B; 
}
VrArrayPtrF64 BlasDouble::mat_rdiv(int matrix_order,VrArrayPtrF64 A, VrArrayPtrF64 B ) {
 	return transpose(mat_ldiv(LAPACK_COL_MAJOR,transpose(B),transpose(A)));
}

VrArrayPtrF64 BlasDouble::mat_ldiv(int matrix_order, VrArrayPtrF64 A, VrArrayPtrF64 B ){
   VrArrayPtrF64 C = vec_copy(VR_GET_NDIMS_F64(B),B);
   VrArrayPtrF64 D = vec_copy(VR_GET_NDIMS_F64(A),A);
  lapack_int m = VR_GET_DATA_F64(D)[0]; 
   lapack_int rank;
	double rcond = 0.001f;
  long int info=0;
   double * data= VR_GET_DATA_F64(D);
   double * out_data=VR_GET_DATA_F64(C);
   long int lda=(long int)VR_GET_DIMS_F64(D)[0];
   long int ldb= (long int)VR_GET_DIMS_F64(C)[0];
   long int n=(long int )VR_GET_DIMS_F64(D)[1];
   long int nrhs= (long int )VR_GET_DIMS_F64(C)[1];
	lapack_int s_size = m<n?m:n;
	double* s = static_cast<double*>(VR_MALLOC(sizeof(double)*s_size));
	if(A.dims[0] != A.dims[1]) {
		 info = LAPACKE_dgelss(LAPACK_COL_MAJOR,(lapack_int)m,n,nrhs,data,lda,out_data,ldb,s,rcond,&rank);
		//if(info != 0) {
			//mexPrintf("info  = %d",info);
		//}
			C.data =data;
		return C;
	}
  int  *IPIV=(dim_type*)VR_MALLOC(sizeof(int)*n);
    info= LAPACKE_dgesv(LAPACK_COL_MAJOR,(lapack_int)n,(lapack_int)nrhs,data,(lapack_int)lda,IPIV,out_data,(lapack_int)ldb);
 return C;
}

VrArrayPtrF64 BlasDouble::scal_div(VrArrayPtrF64 A, double scal) {
  double *data, *out;
  if (scal == 0) {
    VR_PRINT_ERR("Divide by zero");
  }
  VrArrayPtrF64 B = vec_copy(VR_GET_NDIMS_F64(A), A);
  data = VR_GET_DATA_F64(A);
  out = VR_GET_DATA_F64(B);
  for (int i = 0; i < getNumElem(VR_GET_DIMS_F64(A),VR_GET_NDIMS_F64(A)); i++) {
    out[i] = data[i]/scal;
  }
  return B;
}

void BlasDouble::scal_div(VrArrayPtrF64 A, double scal,VrArrayPtrF64 *B) {
  double *data, *out;
  if (scal == 0) {
    VR_PRINT_ERR("Divide by zero");
  }
   vec_copy(VR_GET_NDIMS_F64(A), A,B);
  data = VR_GET_DATA_F64(A);
  out = VR_GET_DATA_F64((*B));
  for (int i = 0; i < getNumElem(VR_GET_DIMS_F64(A),VR_GET_NDIMS_F64(A)); i++) {
    out[i] /= scal;
  }
}
VrArrayPtrF64 BlasDouble::scal_minus(VrArrayPtrF64 A, double scal) {
  return scal_add(VR_GET_NDIMS_F64(A),A,-scal);
}

void BlasDouble::scal_minus(VrArrayPtrF64 A, double scal,VrArrayPtrF64 *B) {
  scal_add(VR_GET_NDIMS_F64(A),A,-scal,B);
}

VrArrayPtrF64 BlasDouble::elem_mult(VrArrayPtrF64 A, VrArrayPtrF64 B) {
  if ( !checkdims<VrArrayF64>(A,B) ) {
    VR_PRINT_ERR("dimensions do not match. \n Exiting. ");
  }
  VrArrayPtrF64 C = vec_copy(VR_GET_NDIMS_F64(A),A);
  for (int i = 0; i < getNumElem(VR_GET_DIMS_F64(A),VR_GET_NDIMS_F64(A)); i++) {
    VR_GET_DATA_F64(C)[i] *= VR_GET_DATA_F64(B)[i];
  }
  return C;
}

void BlasDouble::elem_mult(VrArrayPtrF64 A, VrArrayPtrF64 B,VrArrayPtrF64 *C) {
  if ( !checkdims<VrArrayF64>(A,B) ) {
    VR_PRINT_ERR("dimensions do not match. \n Exiting. ");
  }
  vec_copy(VR_GET_NDIMS_F64(A),A,C);
  for (int i = 0; i < getNumElem(VR_GET_DIMS_F64(A),VR_GET_NDIMS_F64(A)); i++) {
    VR_GET_DATA_F64((*C))[i] *= VR_GET_DATA_F64(B)[i];
  }
}

void BlasDouble::elem_mult(VrArrayPtrF64 A, double scal,VrArrayPtrF64 *B) {

  scal_mult(VR_GET_NDIMS_F64(A), A, scal,B); 
  
}
void BlasDouble::elem_mult(double scal,VrArrayPtrF64 A ,VrArrayPtrF64 *B) {

  scal_mult(VR_GET_NDIMS_F64(A), A, scal,B); 
  
}
VrArrayPtrF64 BlasDouble::elem_mult(VrArrayPtrF64 A, double scal) {

 return scal_mult(VR_GET_NDIMS_F64(A), A, scal); 
  
}
VrArrayPtrF64 BlasDouble::elem_mult(double scal,VrArrayPtrF64 A ) {

 return scal_mult(VR_GET_NDIMS_F64(A), A, scal); 
  
}
VrArrayPtrF64 BlasDouble::elem_div(VrArrayPtrF64 A, VrArrayPtrF64 B, bool zeroCheck) {
  if (!checkdims<VrArrayF64>(A,B) ) {
    VR_PRINT_ERR("dimensions do not match. Exiting. ");
  }
  VrArrayPtrF64 C = vec_copy(VR_GET_NDIMS_F64(A),A);
  if(!zeroCheck){
   for (int i = 0; i < getNumElem(VR_GET_DIMS_F64(A),VR_GET_NDIMS_F64(A)); i++) {
     VR_GET_DATA_F64(C)[i] /= VR_GET_DATA_F64(B)[i];
   }
  }else {
    for (int i = 0; i < getNumElem(VR_GET_DIMS_F64(A),VR_GET_NDIMS_F64(A)); i++) {
     if (VR_GET_DATA_F64(B)[i]==0) {
       VR_PRINT_ERR("divide by zero. \n Exiting");
     }
     VR_GET_DATA_F64(C)[i] /= VR_GET_DATA_F64(B)[i];
   }
  }
  return C;
}
void BlasDouble::elem_div(VrArrayPtrF64 A, VrArrayPtrF64 B, VrArrayPtrF64 *C,bool zeroCheck) {
  if (!checkdims<VrArrayF64>(A,B) ) {
    VR_PRINT_ERR("dimensions do not match. Exiting. ");
  }
   vec_copy(VR_GET_NDIMS_F64(A),A,C);
  if(!zeroCheck){
   for (int i = 0; i < getNumElem(VR_GET_DIMS_F64(A),VR_GET_NDIMS_F64(A)); i++) {
     VR_GET_DATA_F64((*C))[i] /= VR_GET_DATA_F64(B)[i];
   }
  }else {
    for (int i = 0; i < getNumElem(VR_GET_DIMS_F64(A),VR_GET_NDIMS_F64(A)); i++) {
     if (VR_GET_DATA_F64(B)[i]==0) {
       VR_PRINT_ERR("divide by zero. \n Exiting");
       exit(0);
     }
     VR_GET_DATA_F64((*C))[i] /= VR_GET_DATA_F64(B)[i];
   }
  }
}
