#include<mex.h>
#include<vrbinding.hpp>
void mexFunction(int nlhs,mxArray *plhs[],int nrhs,const mxArray *prhs[]){
	 int dims[]={2,2};
	//dims[1]=2;
	//mexPrintf("%d  %d",dims[0],dims[1]);
	/*
	plhs[0]=vrAllocArrayF32RM(2,0,dims);
	plhs[1]=vrAllocArrayF64RM(2,1,dims);
	plhs[2]=vrAllocArrayI64RM(2,1,dims);
	plhs[3]=vrAllocArrayI32RM(2,1,dims);
	*/
	/*VrArrayPtrF32 f32Ptr =NULL;
	f32Ptr=vrAllocArrayF32RM(2,0,dims);
	
	VrArrayPtrF64 f64Ptr=vrAllocArrayF64RM(2,1,dims);
	VrArrayPtrI64 i64Ptr=vrAllocArrayI64RM(2,1,dims);
	VrArrayPtrI32 i32Ptr=vrAllocArrayI32RM(2,1,dims);
	
	plhs[0]= mxCreateNumericArray(f32Ptr->ndims,f32Ptr->dims,mxSINGLE_CLASS,mxREAL);//vrAllocArrayF32RM(2,0,dims);
	mxSetData(plhs[0],f32Ptr->data);
	plhs[1]=mxCreateNumericArray(f64Ptr->ndims,f32Ptr->dims,mxDOUBLE_CLASS,mxREAL);
	mxSetData(plhs[1],f64Ptr->data);
	plhs[2]=  mxCreateNumericArray(i64Ptr->ndims,f32Ptr->dims,mxINT64_CLASS,mxREAL); //vrAllocArrayI64RM(2,1,dims);
	mxSetData(plhs[2],i64Ptr->data);
	plhs[3]=  mxCreateNumericArray(i32Ptr->ndims,f32Ptr->dims,mxINT32_CLASS,mxREAL); //vrAllocArrayI32RM(2,1,dims);
	mxSetData(plhs[3],i32Ptr->data);
*/
	VrArrayPtrCF32 cf32Ptr =NULL;
	cf32Ptr=vrAllocArrayF32CM(2,0,dims);
	
	VrArrayPtrCF64 cf64Ptr=vrAllocArrayF64CM(2,1,dims);
//	VrArrayPtrCI64 ci64Ptr=vrAllocArrayI64CM(2,1,dims);
//	VrArrayPtrCI32 ci32Ptr=vrAllocArrayI32CM(2,1,dims);
	
	plhs[0]= mxCreateNumericArray(cf32Ptr->ndims,cf32Ptr->dims,mxSINGLE_CLASS,mxCOMPLEX);//vrAllocArrayF32RM(2,0,dims);
	mxSetData(plhs[0],cf32Ptr->data);
	plhs[1]=mxCreateNumericArray(cf64Ptr->ndims,cf64Ptr->dims,mxDOUBLE_CLASS,mxCOMPLEX);
	mxSetData(plhs[1],cf64Ptr->data);
//	plhs[2]=  mxCreateNumericArray(ci64Ptr->ndims,ci64Ptr->dims,mxINT64_CLASS,mxCOMPLEX); //vrAllocArrayI64RM(2,1,dims);
//	mxSetData(plhs[2],ci64Ptr->data);
//	plhs[3]=  mxCreateNumericArray(ci32Ptr->ndims,ci32Ptr->dims,mxINT32_CLASS,mxCOMPLEX); //vrAllocArrayI32RM(2,1,dims);
//	mxSetData(plhs[3],ci32Ptr->data);
}
