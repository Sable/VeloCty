#include<mex.h>
#include<iostream>
#include"vrbinding.hpp"
#include<cblas.h>
#include"helper.hpp"
#include<dim-handler.hpp>
#include<lapacke.h>
#include "matrix_ops.hpp"
#include "complexdatahandler.h"
#include "library_ops.hpp"
#include"vr_data.hpp"
void testMean(mxArray* plhs[],mxArray* prhs[]) {
	VrArrayPtrF32 A = getVrArrayF32(prhs[0]);
	VrArrayPtrF32 B = mean(A);
	plhs[0] = mxCreateNumericArray(B.ndims,B.dims,mxSINGLE_CLASS,mxREAL);
	mxSetData(plhs[0],B.data);
}

void testArrayGrowth( mxArray*plhs[],mxArray*prhs[]) {
	dim_type dims[] = {1,3};
	VrArrayPtrF64 A = vrAllocArrayF64RM(2,1,dims);
	checkBounds<VrArrayF64,double>(&A,true,3,range(2,5,1),range(3,4,1),range(2,4,1));		
	//checkBounds<VrArrayF64,double>(&A,true,1,range(10));		
	plhs[0] = mxCreateNumericArray(A.ndims,A.dims,mxDOUBLE_CLASS,mxREAL);
	mxSetPr(plhs[0],A.data);
}

void testFloor( mxArray* plhs[],mxArray*prhs[]) {
	VrArrayPtrF64 A = getVrArrayF64(prhs[0]);
	VrArrayPtrF64 B = floor(A);
	plhs[0] = mxCreateNumericArray(B.ndims,B.dims,mxDOUBLE_CLASS,mxREAL);
	mxSetPr(plhs[0],B.data);
} 
void testmatdiv( mxArray*[],mxArray*[]); 
VrArrayPtrCF64 testComplexF64(const mxArray*[],mxArray*[]);
VrArrayPtrCF32 testComplexF32(const mxArray*[],mxArray*[]);
void testHorzcat(const mxArray* prhs[], mxArray* plhs[]);
void testBoundCheck(mxArray *prhs[],mxArray* plhs[]) {
	VrArrayPtrF64 A = getVrArrayF64(prhs[0]);
	//int a =	getMaxDim(VR_GET_DIMS_F64(A),VR_GET_NDIMS_F64(A));
	//std::cout<<"max dim "<<a<<std::endl;
	VrIndex indices[2];
	indices[0].m_isRange = true;
	indices[0].m_val.range_val[0] =0;
	indices[0].m_val.range_val[1] =3;
	indices[0].m_val.range_val[2] =1;
	indices[1].m_val.const_val = 1;
	indices[1].m_isRange = false;
	//bool maxIndex = exceedsMaxIndex(2,indices, VR_GET_DIMS_F64(A),VR_GET_NDIMS_F64(A));
	//std::cout<<"max index "<<maxIndex<<std::endl;
	int minIndex = getMinIndex(2,indices, VR_GET_DIMS_F64(A));
	std::cout<<"min index "<<minIndex<<std::endl;
	//checkBounds<VrArrayPtrF64,double>(&A,false,2,indices[0],indices[1]);	
}
void testAny(mxArray* plhs[],mxArray*prhs[]) {
	VrArrayPtrF64 A = getVrArrayF64(prhs[0]);
	VrArrayPtrF64 B = getVrArrayF64(prhs[1]);
	std::cout<<"size "<<size(A,0)<<std::endl;
}
void testSumScalar(mxArray* plhs[],mxArray* prhs[]) {
	VrArrayPtrF64 A = getVrArrayF64(prhs[0]);
    double b;
	sum(A,&b);
	plhs[0] = mxCreateDoubleScalar(b);
}
void testSum(mxArray* plhs[],mxArray* prhs[]) {
	VrArrayPtrF64 A = getVrArrayF64(prhs[0]);
	VrArrayPtrF64 B = sum(A);
	plhs[0] = mxCreateNumericArray(B.ndims,B.dims,mxDOUBLE_CLASS,mxREAL);
	mxSetData(plhs[0],B.data);
	/* mat_setComplexDataCF64(plhs[0],B); */
}

void testProd(mxArray* plhs[], mxArray *prhs[]) {
	VrArrayPtrCF64 A = getVrArrayCF64(prhs[0]);
	VrArrayPtrCF64 B = prod(A);
	plhs[0] = mxCreateNumericArray(B.ndims,B.dims,mxDOUBLE_CLASS,mxCOMPLEX);
	//mxSetData(plhs[0],B.data);
	mat_setComplexDataCF64(plhs[0],B);
}
void testColon() {
	std::cout<<"Testing colon function call "<<std::endl;
	VrArrayPtrF64 A = colon(12,-3,0);
	int m = fix<int,double>(-12/-3);
	
	for(int i = 0; i <= m; i++) {
		std::cout<<A.data[i]<<"\t";
	}
}
void testRandn(mxArray* plhs[],mxArray*prhs[]) {
	VrArrayPtrF64 A = randn(10,5);
  plhs[0] = mxCreateNumericArray(A.ndims,A.dims, mxDOUBLE_CLASS,mxREAL);
  mxSetPr(plhs[0],A.data);
		
}
void testMax(mxArray* plhs[],mxArray*prhs[]) {
  VrArrayPtrCF64 A = getVrArrayCF64(prhs[0]);
  VrArrayPtrCF64 B = max(A);
  plhs[0] = mxCreateNumericArray(B.ndims,B.dims, mxDOUBLE_CLASS,mxCOMPLEX);
//  mxSetPr(plhs[0],B.data);
  mat_setComplexDataCF64(plhs[0],B);
}
void testMin(mxArray* plhs[],mxArray*prhs[]) {
  VrArrayPtrF64 A = getVrArrayF64(prhs[0]);
  VrArrayPtrF64 B = min(A);
  plhs[0] = mxCreateNumericArray(B.ndims,B.dims, mxDOUBLE_CLASS,mxREAL);
  mxSetPr(plhs[0],B.data);
//  mat_setComplexDataCF64(plhs[0],B);
}
void testVertcat(mxArray* plhs[],mxArray* prhs[]) {
	VrArrayPtrF64 A = getVrArrayF64(prhs[0]);
 	VrArrayPtrF64 B = getVrArrayF64(prhs[1]);
 	VrArrayPtrF64 C = getVrArrayF64(prhs[2]);
 	VrArrayPtrF64 X = vertcat<VrArrayF64>(3,A,B,C);
 	plhs[0] = mxCreateNumericArray(X.ndims,X.dims,mxDOUBLE_CLASS,mxREAL);
 	mxSetPr(plhs[0],X.data);
}

void testEmpty(mxArray * plhs[], mxArray* prhs[]) {
  int dims[] = {0,3};
  VrArrayPtrF64 A = vrAllocArrayF64RM(2,2,dims);
  plhs[0] = mxCreateNumericArray(0,dims,mxDOUBLE_CLASS,mxREAL);
}

void testRange(mxArray** plhs,mxArray**prhs){
	VrArrayPtrF64 A= getVrArrayF64(prhs[0]);
//VrArrayPtrF64 B = VrIndex(ones(2,1,3)).arr;
//
/* VrArrayPtrF64 B  =A(3,VrIndex(1,4,1),VrIndex(1,4,1),VrIndex(1)); */		
	//VrArrayPtrF64 B = A(1,VrIndex(2,4,1));		
  /* plhs[0] = mxCreateNumericArray(B.ndims,B.dims, mxDOUBLE_CLASS,mxREAL); */
  /* mxSetPr(plhs[0],B.data); */
}
void testSetSlice(mxArray **plhs,mxArray**prhs){
	VrArrayPtrF64 A = getVrArrayF64(prhs[0]);
	VrArrayPtrF64 B = zeros_double(3,4,4,3);//getVrArray(prhs[1]);
	B.setArraySliceSpec(A,VrIndex(1,3,1),VrIndex(2,4,1),VrIndex(3));	
	plhs[0] = mxCreateNumericArray(B.ndims,B.dims,mxDOUBLE_CLASS,mxREAL);
	mxSetPr(plhs[0],B.data);
}
void testSlice(mxArray **plhs,mxArray**prhs){
	VrArrayPtrF64 A = getVrArrayF64(prhs[0]);
	VrArrayPtrF64 B = A.sliceArray(2,VrIndex(2,4,2),VrIndex(2,4,2));	
	plhs[0] = mxCreateNumericArray(B.ndims,B.dims,mxDOUBLE_CLASS,mxREAL);
	mxSetPr(plhs[0],B.data);
}
void  testMmult(mxArray**plhs, mxArray** prhs) {
	VrArrayPtrF64 A = getVrArrayF64(prhs[0]);
	VrArrayPtrF64 B = getVrArrayF64(prhs[1]);
	VrArrayPtrF64 C = zeros_double(2,3,3);
    BlasDouble::mmult(CblasColMajor,CblasNoTrans,CblasNoTrans,A,B,&C);
	plhs[0] = mxCreateNumericArray(C.ndims,C.dims,mxDOUBLE_CLASS,mxREAL);
	mxSetPr(plhs[0],C.data);
}

void testCopy(mxArray **plhs,mxArray **prhs) {
	VrArrayPtrF64 A = getVrArrayF64(prhs[0]);
	VrArrayPtrF64 C = zeros_double(2,3,3);
	/* VrArrayPtrF64 C; */
    BlasDouble::vec_copy(A.ndims,A,&C);
	plhs[0] = mxCreateNumericArray(C.ndims,C.dims,mxDOUBLE_CLASS,mxREAL);
	mxSetPr(plhs[0],C.data);
}

void testScalAdd(mxArray **plhs,mxArray**prhs){
    VrArrayPtrF64 A = getVrArrayF64(prhs[0]);
    VrArrayPtrF64 C;
    BlasDouble::scal_minus(A,3,&C);
    plhs[0] = mxCreateNumericArray(C.ndims,C.dims,mxDOUBLE_CLASS,mxREAL);
    mxSetPr(plhs[0],C.data);
}
void testVecSub(mxArray **plhs,mxArray**prhs){
    VrArrayPtrF64 A = getVrArrayF64(prhs[0]);
    /* VrArrayPtrF64 B = getVrArrayF64(prhs[1]); */
    VrArrayPtrF64 C = zeros_double(2,3,3);
    /* VrArrayPtrF64 C; */
    BlasDouble::vec_sub(A.ndims,4,A,&C);
    plhs[0] = mxCreateNumericArray(C.ndims,C.dims,mxDOUBLE_CLASS,mxREAL);
    mxSetPr(plhs[0],C.data);
}

void  testMatDiv(mxArray**plhs, mxArray** prhs);
void mexFunction(int nlhs,mxArray *plhs[],int nrhs,const mxArray *prhs[]){
	double alpha=10;
	double alph[]={10,10};
	float bet[]={10,10};
	//testComplexF32(prhs,plhs);
	//VrArrayPtrCF32 A= getVrArrayCF32(const_cast<mxArray*>(prhs[0]));
	//VrArrayPtrCF32 B= getVrArrayCF32(const_cast<mxArray*>(prhs[1]));
	//VrArrayPtrCF32 C= testMatDiv(A,B);
	//plhs[0]=mxCreateNumericMatrix(C->dims[0],C->dims[1],mxSINGLE_CLASS, mxCOMPLEX);
	//mxSetData(plhs[0],C.data);
	//mat_setComplexDataCF32(plhs[0],C);
	//testHorzcat(prhs,plhs);
//	testColon();
	//testMax(plhs,const_cast<mxArray**>(prhs));
	//testEmpty(plhs,const_cast<mxArray**>(prhs));
	//testVertcat(plhs,const_cast<mxArray**>(prhs));
//	testBoundCheck(const_cast<mxArray**>(prhs),plhs);
//	testProd(plhs,const_cast<mxArray**>(prhs));
	//testMin(plhs,const_cast<mxArray**>(prhs));
	//testRange(plhs,const_cast<mxArray**>(prhs));
	/* testSetSlice(plhs,const_cast<mxArray**>(prhs)); */
	/* testSlice(plhs,const_cast<mxArray**>(prhs)); */
	//testAny(plhs,const_cast<mxArray**>(prhs));
	//testRandn(plhs,const_cast<mxArray**>(prhs));
	//testMatDiv(plhs,const_cast<mxArray**>(prhs));
	//testArrayGrowth(plhs,const_cast<mxArray**>(prhs));
	//testFloor(plhs,const_cast<mxArray**>(prhs));
	/* testMmult(plhs,const_cast<mxArray**>(prhs)); */
	/* testCopy(plhs,const_cast<mxArray**>(prhs)); */
	/* testScalAdd(plhs,const_cast<mxArray**>(prhs)); */
	/* testVecSub(plhs,const_cast<mxArray**>(prhs)); */
	/* testSum(plhs,const_cast<mxArray**>(prhs)); */
	testSumScalar(plhs,const_cast<mxArray**>(prhs));
    
    
}
void  testHorzcat(const mxArray* prhs[], mxArray* plhs[]) {
	VrArrayPtrF64 A = getVrArrayF64(const_cast<mxArray*>(prhs[0]));		
	VrArrayPtrF64 B = getVrArrayF64(const_cast<mxArray*>(prhs[1]));		
	VrArrayPtrF64 C = getVrArrayF64(const_cast<mxArray*>(prhs[2]));
	VrArrayPtrF64 D =horzcat<VrArrayF64>(3,A,B,C);		
	plhs[0] = mxCreateNumericArray(D.ndims,D.dims,mxDOUBLE_CLASS, mxREAL);
	mxSetData(plhs[0],D.data);
}
VrArrayPtrCF32 testComplexF32(const mxArray* prhs[], mxArray* plhs[]) {
	float complex alpha = 3+2*I;
	VrArrayPtrCF32 A = getVrArrayCF32(const_cast<mxArray*>(prhs[0]));
	VrArrayPtrCF32 B = getVrArrayCF32(const_cast<mxArray*>(prhs[1]));
	VrArrayPtrCF32 C = BlasComplexSingle::mmult(CblasColMajor,CblasNoTrans,CblasNoTrans,A,B,1,0);
	VrArrayPtrCF32 D = BlasComplexSingle::scal_mult(A.ndims,A,alpha);
	VrArrayPtrCF32 E = BlasComplexSingle::vec_add(A.ndims,A,B);
	VrArrayPtrCF32 F = BlasComplexSingle::scal_minus(A,alpha);
	VrArrayPtrCF32 G = BlasComplexSingle::vec_sub(A.ndims,A,B,1);
	VrArrayPtrCF32 H = BlasComplexSingle::scal_div(A,alpha);
	VrArrayPtrCF32 J = BlasComplexSingle::elem_mult(A,B);
	VrArrayPtrCF32 K = BlasComplexSingle::elem_div(A,B);
	plhs[0]=mxCreateNumericMatrix(C.dims[0],C.dims[1],mxSINGLE_CLASS, mxCOMPLEX);
	mat_setComplexDataCF32(plhs[0],C);
	plhs[1]=mxCreateNumericMatrix(D.dims[0],D.dims[1],mxSINGLE_CLASS, mxCOMPLEX);
	mat_setComplexDataCF32(plhs[1],D);
	plhs[2]=mxCreateNumericMatrix(E.dims[0],E.dims[1],mxSINGLE_CLASS, mxCOMPLEX);
	mat_setComplexDataCF32(plhs[2],E);
	plhs[3]=mxCreateNumericMatrix(F.dims[0],F.dims[1],mxSINGLE_CLASS, mxCOMPLEX);
	mat_setComplexDataCF32(plhs[3],F);
	plhs[4]=mxCreateNumericMatrix(G.dims[0],G.dims[1],mxSINGLE_CLASS, mxCOMPLEX);
	mat_setComplexDataCF32(plhs[4],G);
	plhs[5]=mxCreateNumericMatrix(H.dims[0],H.dims[1],mxSINGLE_CLASS, mxCOMPLEX);
	mat_setComplexDataCF32(plhs[5],H);
	plhs[6]=mxCreateNumericMatrix(J.dims[0],J.dims[1],mxSINGLE_CLASS, mxCOMPLEX);
	mat_setComplexDataCF32(plhs[6],J);
	plhs[7]=mxCreateNumericMatrix(K.dims[0],K.dims[1],mxSINGLE_CLASS, mxCOMPLEX);
	mat_setComplexDataCF32(plhs[7],K);
		//return C;
}
VrArrayPtrCF64 testComplexF64(const mxArray* prhs[], mxArray* plhs[]) {
	double complex alpha = 3+2*I;
	VrArrayPtrCF64 A = getVrArrayCF64(const_cast<mxArray*>(prhs[0]));
	VrArrayPtrCF64 B = getVrArrayCF64(const_cast<mxArray*>(prhs[1]));
	VrArrayPtrCF64 C = BlasComplexDouble::mmult(CblasColMajor,CblasNoTrans,CblasNoTrans,A,B,1,0);
	VrArrayPtrCF64 D = BlasComplexDouble::scal_mult(A.ndims,A,alpha);
	VrArrayPtrCF64 E = BlasComplexDouble::vec_add(A.ndims,A,B);
	VrArrayPtrCF64 F = BlasComplexDouble::scal_minus(A,alpha);
	VrArrayPtrCF64 G = BlasComplexDouble::vec_sub(A.ndims,A,B,1);
	VrArrayPtrCF64 H = BlasComplexDouble::scal_div(A,alpha);
	VrArrayPtrCF64 J = BlasComplexDouble::elem_mult(A,B);
	VrArrayPtrCF64 K = BlasComplexDouble::elem_div(A,B);
	plhs[0]=mxCreateNumericMatrix(C.dims[0],C.dims[1],mxDOUBLE_CLASS, mxCOMPLEX);
	mat_setComplexDataCF64(plhs[0],C);
	plhs[1]=mxCreateNumericMatrix(D.dims[0],D.dims[1],mxDOUBLE_CLASS, mxCOMPLEX);
	mat_setComplexDataCF64(plhs[1],D);
	plhs[2]=mxCreateNumericMatrix(E.dims[0],E.dims[1],mxDOUBLE_CLASS, mxCOMPLEX);
	mat_setComplexDataCF64(plhs[2],E);
	plhs[3]=mxCreateNumericMatrix(F.dims[0],F.dims[1],mxDOUBLE_CLASS, mxCOMPLEX);
	mat_setComplexDataCF64(plhs[3],F);
	plhs[4]=mxCreateNumericMatrix(G.dims[0],G.dims[1],mxDOUBLE_CLASS, mxCOMPLEX);
	mat_setComplexDataCF64(plhs[4],G);
	plhs[5]=mxCreateNumericMatrix(H.dims[0],H.dims[1],mxDOUBLE_CLASS, mxCOMPLEX);
	mat_setComplexDataCF64(plhs[5],H);
	plhs[6]=mxCreateNumericMatrix(J.dims[0],J.dims[1],mxDOUBLE_CLASS, mxCOMPLEX);
	mat_setComplexDataCF64(plhs[6],J);
	plhs[7]=mxCreateNumericMatrix(K.dims[0],K.dims[1],mxDOUBLE_CLASS, mxCOMPLEX);
	mat_setComplexDataCF64(plhs[7],K);
		//return C;
}
void  testMatDiv(mxArray**plhs, mxArray** prhs) {
	VrArrayPtrF64 A = getVrArrayF64(prhs[0]);
	VrArrayPtrF64 B = getVrArrayF64(prhs[1]);
	VrArrayPtrF64 C= BlasDouble::mat_ldiv(1,A,B);
	plhs[0] = mxCreateNumericArray(C.ndims,C.dims,mxDOUBLE_CLASS,mxREAL);
	mxSetPr(plhs[0],C.data);
}
