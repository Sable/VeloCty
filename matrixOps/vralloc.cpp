#include<vrbinding.hpp>
#include<string.h>
#include "helper.hpp"
VrArrayPtrF32 vrAllocArrayF32RM(int ndims,int func,int *sizes){
	VrArrayPtrF32 arr;
	switch(func){
	case 0: //Zeros
		{
		//VrArrayPtrF32 arr;//=(VrArrayPtrF32)VR_MALLOC(sizeof(VrArrayF32));  
		VR_GET_DATA_F32(arr)=NULL;
		VR_GET_DATA_F32(arr)=(float*)VR_MALLOC(sizeof(float)*getNumElem(sizes,ndims));
		memset(VR_GET_DATA_F32(arr),0,sizeof(float)*getNumElem(sizes,ndims));
		VR_GET_NDIMS_F32(arr)=ndims ==1?2:ndims;
		VR_GET_DIMS_F32(arr)=(dim_type*)VR_MALLOC(sizeof(dim_type)*arr.ndims);
		memset(arr.dims,1,sizeof(dim_type)*arr.ndims);
		memcpy(VR_GET_DIMS_F32(arr),sizes,ndims*sizeof(dim_type));
		return arr;
		}
	case 1: // Ones
		{
		//arr//(VrArrayPtrF32)VR_MALLOC(sizeof(VrArrayF32)); 
		VR_GET_DATA_F32(arr)=(float*)VR_MALLOC(sizeof(float)*getNumElem(sizes,ndims));
		VR_GET_NDIMS_F32(arr)=ndims ==1?2:ndims;
		VR_GET_DIMS_F32(arr)=(dim_type*)VR_MALLOC(sizeof(dim_type)*arr.ndims);
		memset(arr.dims,1,sizeof(dim_type)*arr.ndims);
		memcpy(VR_GET_DIMS_F32(arr),sizes,ndims*sizeof(dim_type));
			int numel=getNumElem(sizes,ndims);
			for(int i=0;i<numel;i++){
				VR_GET_DATA_F32(arr)[i]=1.0f;
			}
		return arr;
		}
	case 2 : //Empty Matrix 
		{
			//arr=(VrArrayPtrF32)VR_MALLOC(sizeof(VrArrayF32));
			VR_GET_DATA_F32(arr) = NULL;
			VR_GET_NDIMS_F32(arr) = ndims== 1?2:ndims;
			VR_GET_DIMS_F32(arr)=(dim_type*)VR_MALLOC(sizeof(dim_type)*arr.ndims);
			memset(arr.dims,1,sizeof(dim_type)*arr.ndims);
			memcpy(VR_GET_DIMS_F32(arr),sizes,ndims*sizeof(dim_type));
			return arr;
		}
	}
}
VrArrayPtrF64 vrAllocArrayF64RM(int ndims,int func,int *sizes){
	VrArrayPtrF64 arr; 
	switch(func){
	case 0: //Zeros
		{
		VR_GET_DATA_F64(arr)=(double*)VR_MALLOC(sizeof(double)*getNumElem(sizes,ndims));
		memset(VR_GET_DATA_F64(arr),0,sizeof(double)*getNumElem(sizes,ndims));
		VR_GET_NDIMS_F64(arr)=ndims ==1?2:ndims;
		VR_GET_DIMS_F64(arr)=(dim_type*)VR_MALLOC(sizeof(dim_type)*arr.ndims);
        arr.dims[1] = 1;
		memcpy(VR_GET_DIMS_F64(arr),sizes,ndims*sizeof(dim_type));
		return arr;
		}
	case 1: // Ones
		
		{
		//arr=(VrArrayPtrF64)VR_MALLOC(sizeof(VrArrayF64));  
		VR_GET_DATA_F64(arr)=(double*)VR_MALLOC(sizeof(double)*getNumElem(sizes,ndims));
		VR_GET_NDIMS_F64(arr)=ndims;
		//memcpy(arr->dims,sizes,ndims*sizeof(int));
		VR_GET_DIMS_F64(arr)=(dim_type*)VR_MALLOC(sizeof(dim_type)*ndims);
		memcpy(VR_GET_DIMS_F64(arr),sizes,ndims*sizeof(dim_type));
		int numel=getNumElem(sizes,ndims);
			for(int i=0;i<numel;i++){
				VR_GET_DATA_F64(arr)[i]=1.0f;
			}
		return arr;
		}
	
	
	case 2 : //Empty Matrix 
		{
		//	arr=(VrArrayPtrF64)VR_MALLOC(sizeof(VrArrayF64));
			VR_GET_DATA_F64(arr) = NULL;
			VR_GET_NDIMS_F64(arr) = ndims;
			VR_GET_DIMS_F64(arr)=(dim_type*)VR_MALLOC(sizeof(dim_type)*ndims);
			memcpy(VR_GET_DIMS_F64(arr),sizes,ndims*sizeof(dim_type));
			return arr;
		}
	}
}

VrArrayPtrI64 vrAllocArrayI64RM(int ndims,int func,int *sizes){
	VrArrayPtrI64 arr; 
	switch(func){
	case 0: //Zeros
		{
		//arr=(VrArrayPtrI64)VR_MALLOC(sizeof(VrArrayI64));
		VR_GET_DATA_I64(arr)=(long*)VR_MALLOC(sizeof(float)*getNumElem(sizes,ndims));
		memset(VR_GET_DATA_I64(arr),0,sizeof(double)*getNumElem(sizes,ndims));
		VR_GET_NDIMS_I64(arr)=ndims;
		VR_GET_DIMS_I64(arr)=sizes;
		
		return arr;
		}
	case 1: // Ones
		{
		//arr=(VrArrayPtrI64)VR_MALLOC(sizeof(VrArrayI64));  
		VR_GET_DATA_I64(arr)=(long*)VR_MALLOC(sizeof(long)*getNumElem(sizes,ndims));
		VR_GET_NDIMS_I64(arr)=ndims;
		VR_GET_DIMS_I64(arr)=sizes;
		int numel=getNumElem(sizes,ndims);
			for(int i=0;i<numel;i++){
				VR_GET_DATA_I64(arr)[i]=1.0f;
			}
		
		return arr;
		}
	case 2 : //Empty Matrix 
		{
		//	arr=(VrArrayPtrI64)VR_MALLOC(sizeof(VrArrayI64));
			VR_GET_DATA_I64(arr) = NULL;
			VR_GET_NDIMS_I64(arr) = ndims;
			VR_GET_DIMS_I64(arr)=(dim_type*)VR_MALLOC(sizeof(dim_type)*ndims);
			memcpy(VR_GET_DIMS_I64(arr),sizes,ndims*sizeof(dim_type));
			return arr;
		}
	}
}

VrArrayPtrI32 vrAllocArrayI32RM(int ndims,int func,int *sizes){
	VrArrayPtrI32 arr; 
	switch(func){
	case 0: //Zeros
		{
		//arr=(VrArrayPtrI32)VR_MALLOC(sizeof(VrArrayI32)); 
		VR_GET_DATA_I32(arr)=(int*)VR_MALLOC(sizeof(int)*getNumElem(sizes,ndims));
		memset(VR_GET_DATA_I32(arr),0,sizeof(int)*getNumElem(sizes,ndims));
		VR_GET_NDIMS_I32(arr)=ndims;
		VR_GET_DIMS_I32(arr)=(dim_type*)VR_MALLOC(sizeof(dim_type)*ndims);
		memcpy(VR_GET_DIMS_I32(arr),sizes,ndims*sizeof(dim_type));
		return arr;
		}
	case 1: // Ones
		{
		//arr=(VrArrayPtrI32)VR_MALLOC(sizeof(VrArrayI32));  
		VR_GET_DATA_I32(arr)=(int*)VR_MALLOC(sizeof(int)*getNumElem(sizes,ndims));
		VR_GET_NDIMS_I32(arr)=ndims;
		VR_GET_DIMS_I32(arr)=(dim_type*)VR_MALLOC(sizeof(dim_type)*ndims);
		memcpy(VR_GET_DIMS_I32(arr),sizes,ndims*sizeof(dim_type));
		int numel=getNumElem(sizes,ndims);
		for(int i=0;i<numel;i++){
			VR_GET_DATA_I32(arr)[i]=1.0f;
		}
		return arr;
		}
	case 2 : //Empty Matrix 
		{
		//	arr=(VrArrayPtrI32)VR_MALLOC(sizeof(VrArrayI32));
			VR_GET_DATA_I32(arr) = NULL;
			VR_GET_NDIMS_I32(arr) = ndims;
			VR_GET_DIMS_I32(arr)=(dim_type*)VR_MALLOC(sizeof(dim_type)*ndims);
			memcpy(VR_GET_DIMS_CI32(arr),sizes,ndims*sizeof(dim_type));
			return arr;
		}
	}
}

VrArrayPtrCF32 vrAllocArrayF32CM(int ndims,int func,int *sizes){
	VrArrayPtrCF32 arr;	
	switch(func){
	case 0: //Zeros
		{
		//arr=(VrArrayPtrCF32)VR_MALLOC(sizeof(VrArrayCF32));  //mxCreateNumericArray(ndims,(int*)sizes,mxSINGLE_CLASS,mxREAL);
		VR_GET_DATA_CF32(arr)=(float complex *)VR_MALLOC(sizeof(float complex)*getNumElem(sizes,ndims));
		VR_GET_NDIMS_CF32(arr)=ndims;
		memset(VR_GET_DATA_CF32(arr),0,sizeof(double)*getNumElem(sizes,ndims));
		//memcpy(arr->dims,sizes,ndims*sizeof(int));
		VR_GET_DIMS_CF32(arr)=(dim_type*)VR_MALLOC(sizeof(dim_type)*ndims);
		
		memcpy(VR_GET_DIMS_CF32(arr),sizes,ndims*sizeof(dim_type));
		return arr;
		}
	case 1: // Ones
		{
		 //arr=(VrArrayPtrCF32)VR_MALLOC(sizeof(VrArrayCF32));  //mxCreateNumericArray(ndims,(int*)sizes,mxSINGLE_CLASS,mxREAL);
		VR_GET_DATA_CF32(arr)=(float complex*)VR_MALLOC(sizeof(float complex)*getNumElem(sizes,ndims));
		VR_GET_NDIMS_CF32(arr)=ndims;
		//memcpy(arr->dims,sizes,ndims*sizeof(int));
		VR_GET_DIMS_CF32(arr)=(dim_type*)VR_MALLOC(sizeof(dim_type)*ndims);
		memcpy(VR_GET_DIMS_CF32(arr),sizes,ndims*sizeof(dim_type));
		int numel=getNumElem(sizes,ndims);
			for(int i=0;i<numel;i++){
				VR_GET_DATA_CF32(arr)[i]=1.0f;
			}
		}
		return arr;
	case 2 : //Empty Matrix 
		{
		//	arr=(VrArrayPtrCF32)VR_MALLOC(sizeof(VrArrayCF32));
			VR_GET_DATA_CF32(arr) = NULL;
			VR_GET_NDIMS_CF32(arr) = ndims;
			VR_GET_DIMS_CF32(arr)=(dim_type*)VR_MALLOC(sizeof(dim_type)*ndims);
			memcpy(VR_GET_DIMS_CF32(arr),sizes,ndims*sizeof(dim_type));
			return arr;
		}

	}
}
VrArrayPtrCF64 vrAllocArrayF64CM(int ndims,int func,int *sizes){
	VrArrayPtrCF64 arr;
	switch(func){
	case 0: //Zeros
		{	
		//arr=(VrArrayPtrCF64)VR_MALLOC(sizeof(VrArrayCF64));  //mxCreateNumericArray(ndims,(int*)sizes,mxSINGLE_CLASS,mxREAL);
		VR_GET_DATA_CF64(arr)=(double complex*)VR_MALLOC(sizeof(double complex)*getNumElem(sizes,ndims));
		VR_GET_NDIMS_CF64(arr)=ndims;
		memset(VR_GET_DATA_CF64(arr),0,sizeof(double complex )*getNumElem(sizes,ndims));
		//memcpy(arr->dims,sizes,ndims*sizeof(int));
		VR_GET_DIMS_CF64(arr)=(dim_type*)VR_MALLOC(sizeof(dim_type)*ndims);
		memcpy(VR_GET_DIMS_CF64(arr),sizes,ndims*sizeof(dim_type));
		return arr;
		}
	case 1: // Ones
		
		{
		//VrArrayPtrCF64 arr=(VrArrayPtrCF64)VR_MALLOC(sizeof(VrArrayCF64));  //mxCreateNumericArray(ndims,(int*)sizes,mxSINGLE_CLASS,mxREAL);
		VR_GET_DATA_CF64(arr)=(double complex*)VR_MALLOC(sizeof(double complex)*getNumElem(sizes,ndims));
		VR_GET_NDIMS_CF64(arr)=ndims;
		//memcpy(arr->dims,sizes,ndims*sizeof(int));
		VR_GET_DIMS_CF64(arr)=(dim_type*)VR_MALLOC(sizeof(dim_type)*ndims);
		memcpy(VR_GET_DIMS_CF64(arr),sizes,ndims*sizeof(dim_type));
		int numel=getNumElem(sizes,ndims);
			for(int i=0;i<numel;i++){
				VR_GET_DATA_CF64(arr)[i]=1.0f;
			}
		return arr;
		}
	case 2 : //Empty Matrix 
		{
		//	arr=(VrArrayPtrCF64)VR_MALLOC(sizeof(VrArrayCF64));
			VR_GET_DATA_CF64(arr) = NULL;
			VR_GET_NDIMS_CF64(arr) = ndims;
			VR_GET_DIMS_CF64(arr)=(dim_type*)VR_MALLOC(sizeof(dim_type)*ndims);
			memcpy(VR_GET_DIMS_CF64(arr),sizes,ndims*sizeof(dim_type));
			return arr;
		}
	}
}
