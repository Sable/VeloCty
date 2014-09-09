#include "library_ops.hpp"
#include <stdarg.h>
#include "helper.hpp"
#include <math.h>
#include <iostream>
#define _USE_MATH_DEFINES
#define MAX(A,B) (A>B?A:B)
VrArrayPtrF64 floor( VrArrayPtrF64 arr) {
	dim_type numel = getNumElem(arr.dims,arr.ndims);
	VrArrayPtrF64 out(arr.ndims,arr.dims);
	for(int i = 0; i < numel; i++) {
		out.data[i] = floor(arr.data[i]);	
	}
	return out;
}
bool isVector(dim_type* dims,dim_type ndims) {
	return ndims ==2 && ( dims[0] == 1 || dims[1] == 1);
}
complex double i(){
	return I;
}

long length(VrArrayPtrF64 A){
	int ndims=VR_GET_NDIMS_F64(A);
	int max=INT_MIN;
	int *dims=VR_GET_DIMS_F64(A);
	for(int i=0;i<ndims;i++){
		
		max=MAX(max,dims[i]);
	}
	return max;
}
#ifdef __MATLAB__
VrArrayPtrF64 zeros_double(int nargs,...){
	va_list args;
  int ndims = 0;
  bool zeroFlag = false;
  if (nargs <=  0) {
    std::cout<<"number of arguments have to be positive"<<std::endl;
    exit(0);
  } 

  if(nargs == 1) {
    ndims = 2;
  }
  else {
    ndims = nargs;
  }
  dim_type *dims= (dim_type*)VR_MALLOC(sizeof(dim_type)*ndims);
  memset(dims,0,sizeof(dim_type)*ndims);
	va_start(args,nargs);
	for(int i = 0; i < nargs; i++){
	  dims[i] = va_arg(args,int);
	  if(dims[i] < 0) {
	    VR_PRINT_ERR("Dimensions have to be greater than 0");
	  }
      if (dims[i] == 0) {
     	zeroFlag = true;
      }
	}
  if(nargs == 1 && !zeroFlag) {
    dims[1] = dims[0];
  }

  VrArrayPtrF64 c;
  if (!zeroFlag) {
     c = vrAllocArrayF64RM(ndims,0,dims);
     
  }else {
    c =  vrAllocArrayF64RM(ndims,2,dims);
  }
	va_end(args);
	return c;
}

VrArrayPtrF64 ones(int nargs,...){
	va_list args;
  int ndims = 0;
  bool zeroFlag = false;
  if (nargs <=  0) {
    std::cout<<"number of arguments have to be positive"<<std::endl;
    exit(0);
  } 
  if (nargs == 1) {
    ndims = 2;
  }else {
    ndims = nargs;
  }
	int *dims = (int*)VR_MALLOC(sizeof(int) * ndims);
memset(dims, 0, ndims * sizeof(dim_type));
	va_start(args,nargs);
	for(int i = 0; i < nargs; i++){

		dims[i] = va_arg(args,int);
    if(dims[i] < 0) {
      std::cout<<"dimensions have to be positive "<<std::endl;
      exit(0);
    }
    if (dims[i] == 0) {
      zeroFlag = true;
      break;
    }
	}
  if (nargs == 1 && !zeroFlag) {
    dims[1] = dims[0]; 
  }
  VrArrayPtrF64 c;
  if (!zeroFlag) {
    c=vrAllocArrayF64RM(ndims,1,dims);
  }else {
    VR_GET_DATA_F64(c) = NULL;
    VR_GET_DIMS_F64(c) = dims;
    VR_GET_NDIMS_F64(c) = ndims;
  }
	va_end(args);
	return c;
}
#elif defined __PYTHON__
VrArrayPtrF64 zeros_double(int nargs,...){
	va_list args;
	
  int ndims = 0;
  bool zeroFlag = false;
  if (nargs <=  0) {
    std::cout<<"number of arguments have to be positive"<<std::endl;
    exit(0);
  } 
  
  /* if(nargs == 1) { */
  /*     ndims = 2; */
  /* } */
  /* else { */
      ndims = nargs;
  /* } */
  dim_type *dims= (dim_type*)VR_MALLOC(sizeof(dim_type)*ndims);
  va_start(args,nargs);
  for(int i = 0; i < nargs; i++){
      dims[i] = va_arg(args,int);
      if(dims[i] < 0) {
          VR_PRINT_ERR("Dimensions have to be greater than 0");
      }
      if (dims[i] == 0) {
          zeroFlag = true;
      }
  }
  va_end(args);
  /* if(nargs == 1 && !zeroFlag) { */
  /*     dims[1] = dims[0]; */
  /*     dims[0] = 1; */
  /* } */

  VrArrayPtrF64 c;
  if (!zeroFlag) {
     c = vrAllocArrayF64RM(ndims,0,dims);
     
  }else {
    c =  vrAllocArrayF64RM(ndims,2,dims);
  }
  return c;
}

VrArrayPtrI32 zeros_int(int nargs,...){
	va_list args;
	
  int ndims = 0;
  bool zeroFlag = false;
  if (nargs <=  0) {
    std::cout<<"number of arguments have to be positive"<<std::endl;
    exit(0);
  } 
  
  /* if(nargs == 1) { */
  /*     ndims = 2; */
  /* } */
  /* else { */
      ndims = nargs;
  /* } */
  dim_type *dims= (dim_type*)VR_MALLOC(sizeof(dim_type)*ndims);
  va_start(args,nargs);
  for(int i = 0; i < nargs; i++){
      dims[i] = va_arg(args,int);
      if(dims[i] < 0) {
          VR_PRINT_ERR("Dimensions have to be greater than 0");
      }
      if (dims[i] == 0) {
          zeroFlag = true;
      }
  }
  va_end(args);
  /* if(nargs == 1 && !zeroFlag) { */
  /*     dims[1] = dims[0]; */
  /*     dims[0] = 1; */
  /* } */

  VrArrayPtrI32 c;
  if (!zeroFlag) {
     c = vrAllocArrayI32RM(ndims,0,dims);
     
  }else {
    c =  vrAllocArrayI32RM(ndims,2,dims);
  }
  return c;
}
VrArrayPtrF64 ones(int nargs,...){
	va_list args;
  int ndims = 0;
  bool zeroFlag = false;
  if (nargs <=  0) {
    std::cout<<"number of arguments have to be positive"<<std::endl;
    exit(0);
  } 
    ndims = nargs;
	int *dims = (int*)VR_MALLOC(sizeof(int) * ndims);
  memset(dims, 0, ndims * sizeof(dim_type));
	va_start(args,nargs);
	for(int i = 0; i < nargs; i++){

		dims[i] = va_arg(args,int);
    if(dims[i] < 0) {
      std::cout<<"dimensions have to be positive "<<std::endl;
      exit(0);
    }
    if (dims[i] == 0) {
      zeroFlag = true;
      break;
    }
	}
  /* if (nargs == 1 && !zeroFlag) { */
  /*   dims[1] = dims[0]; */ 
  /*   dims[0] = 1; */
  /* } */
  VrArrayPtrF64 c;
  if (!zeroFlag) {
    c=vrAllocArrayF64RM(ndims,1,dims);
  }else {
    VR_GET_DATA_F64(c) = NULL;
    VR_GET_DIMS_F64(c) = dims;
    VR_GET_NDIMS_F64(c) = ndims;
  }
	va_end(args);
	return c;
}

inline long getIndexVal(VrArrayPtrF64 arr, int nIndices, ...) {
    VrIndex *indices = static_cast<VrIndex*>(VR_MALLOC(sizeof(VrIndex) * nIndices));
    va_list args;
    va_start(args,nIndices);
    for(int i = 0; i < nIndices; i++) {
        indices[i] = va_arg(args, VrIndex); 
    }  
    long multiplier = 1;
    long finalIndex = 0;
    for( int i = nIndices - 1; i >= 0; i-- ) {
        if (!indices[i].m_isRange && !indices[i].m_isArray) {
            if(indices[i].m_val.const_val < 0) {
                finalIndex += (arr.dims[i] + indices[i].m_val.const_val) * multiplier;
            } else {
                finalIndex += indices[i].m_val.const_val * multiplier;
            }
            multiplier *= arr.dims[i]; 
        } else {
            std::cout<<"Not currently supported."<<std::endl;
            exit(0);
        }
    } 
    return finalIndex;
}
inline long getIndexVal_spec(VrArrayPtrF64 arr, long row,  long col) {
    if( row < 0) {
      row = arr.dims[0] - row;  
    }
    if(col < 0) {
      col = arr.dims[1] - col;
    }
    return row * arr.dims[1] + col;
}
long getIndexVal_spec(VrArrayPtrF64 arr, long row) {
    if( row < 0) {
      row = arr.dims[0] - row;  
    }
    return row;
}

long getIndexVal_spec(VrArrayPtrI32 arr, long row,  long col) {
    if( row < 0) {
      row = arr.dims[0] - row;  
    }
    if(col < 0) {
      col = arr.dims[1] - col;
    }
    return row * arr.dims[1] + col;
}

long getIndexVal_spec(VrArrayPtrI32 arr, long row) {
    if( row < 0) {
      row = arr.dims[0] - row;  
    }
    return row;
}
long getIndexVal(VrArrayPtrI32 arr, int nIndices, ...) {
    VrIndex *indices = static_cast<VrIndex*>(VR_MALLOC(sizeof(VrIndex) * nIndices));
    va_list args;
    va_start(args,nIndices);
    for(int i = 0; i < nIndices; i++) {
        indices[i] = va_arg(args, VrIndex); 
    }  
    long multiplier = 1;
    long finalIndex = 0;
    for( int i = nIndices - 1; i >= 0; i-- ) {
        if (!indices[i].m_isRange && !indices[i].m_isArray) {
            if(indices[i].m_val.const_val < 0) {
                finalIndex += (arr.dims[i] + indices[i].m_val.const_val) * multiplier;
            } else {
                finalIndex += indices[i].m_val.const_val * multiplier;
            }
            multiplier *= arr.dims[i]; 
        } else {
            std::cout<<"Not currently supported."<<std::endl;
            exit(0);
        }
    } 
    return finalIndex;
}
#endif
VrArrayPtrF64 abs(VrArrayPtrF64 arr){
    int numel=getNumElem(VR_GET_DIMS_F64(arr),VR_GET_NDIMS_F64(arr));
    VrArrayPtrF64 out=vrAllocArrayF64RM(VR_GET_NDIMS_CF64(arr),0,VR_GET_DIMS_CF64(arr));
	
	for(int i=0;i<numel;i++){
		VR_GET_DATA_F64(out)[i] = fabs(VR_GET_DATA_F64(arr)[i]);		
	}	
	return out;
}

VrArrayPtrF32 abs(VrArrayPtrF32 arr){
	int numel=getNumElem(VR_GET_DIMS_F32(arr),VR_GET_NDIMS_F32(arr));
	VrArrayPtrF32 out=vrAllocArrayF32RM(VR_GET_NDIMS_CF64(arr),0,VR_GET_DIMS_CF64(arr));
	
	for(int i=0;i<numel;i++){
		VR_GET_DATA_F32(out)[i]=fabs(VR_GET_DATA_F32(arr)[i]);		
	}	
	return out;
}

VrArrayPtrF64 cos(VrArrayPtrF64 arr){
	int numel=getNumElem(VR_GET_DIMS_F64(arr),VR_GET_NDIMS_F64(arr));
	VrArrayPtrF64 out=vrAllocArrayF64RM(VR_GET_NDIMS_CF64(arr),0,VR_GET_DIMS_CF64(arr));
	
	for(int i=0;i<numel;i++){
		VR_GET_DATA_F64(out)[i] = cos(VR_GET_DATA_F64(arr)[i]);		
	}	
	return out;
}

VrArrayPtrF32 cos(VrArrayPtrF32 arr){
	int numel=getNumElem(VR_GET_DIMS_F32(arr),VR_GET_NDIMS_F32(arr));
	VrArrayPtrF32 out=vrAllocArrayF32RM(VR_GET_NDIMS_CF64(arr),0,VR_GET_DIMS_CF64(arr));
	
	for(int i=0;i<numel;i++){
		VR_GET_DATA_F32(out)[i]=cos(VR_GET_DATA_F32(arr)[i]);		
	}	
	return out;
}


VrArrayPtrF64 sin(VrArrayPtrF64 arr){
	int numel=getNumElem(VR_GET_DIMS_F64(arr),VR_GET_NDIMS_F64(arr));
	VrArrayPtrF64 out=vrAllocArrayF64RM(VR_GET_NDIMS_CF64(arr),0,VR_GET_DIMS_CF64(arr));
	
	for(int i=0;i<numel;i++){
		VR_GET_DATA_F64(out)[i] = sin(VR_GET_DATA_F64(arr)[i]);		
	}	
	return out;
}

VrArrayPtrF32 sin(VrArrayPtrF32 arr){
	int numel=getNumElem(VR_GET_DIMS_F32(arr),VR_GET_NDIMS_F32(arr));
	VrArrayPtrF32 out=vrAllocArrayF32RM(VR_GET_NDIMS_CF64(arr),0,VR_GET_DIMS_CF64(arr));
	
	for(int i=0;i<numel;i++){
		VR_GET_DATA_F32(out)[i]=sin(VR_GET_DATA_F32(arr)[i]);		
	}	
	return out;
}

VrArrayPtrF64 pow(VrArrayPtrF64 arr,double  val){
	int numel=getNumElem(VR_GET_DIMS_F64(arr),VR_GET_NDIMS_F64(arr));
	VrArrayPtrF64 out=vrAllocArrayF64RM(VR_GET_NDIMS_CF64(arr),0,VR_GET_DIMS_CF64(arr));
	
	for(int i=0;i<numel;i++){
		VR_GET_DATA_F64(out)[i] = pow(VR_GET_DATA_F64(arr)[i],static_cast<double>(val));		
	}	
	return out;
}

VrArrayPtrF32 pow(VrArrayPtrF32 arr,double val){
	int numel=getNumElem(VR_GET_DIMS_F32(arr),VR_GET_NDIMS_F32(arr));
	VrArrayPtrF32 out=vrAllocArrayF32RM(VR_GET_NDIMS_CF64(arr),0,VR_GET_DIMS_CF64(arr));
	
	for(int i=0;i<numel;i++){
		VR_GET_DATA_F32(out)[i]=powf(VR_GET_DATA_F32(arr)[i],val);		
	}	
	return out;
}

double pi(){
  return M_PI;
}


VrIndex range(dim_type val ){
  VrIndex indx ;
  indx.m_isRange=false;
  indx.m_val.const_val=val;
  return indx;
}

VrIndex range(dim_type start, dim_type stop , dim_type step){
  VrIndex indx;
  indx.m_isRange=true;
  indx.m_val.range_val[0]=start;
  indx.m_val.range_val[1]=stop;
  indx.m_val.range_val[2]=step;
  return indx;
}

VrArrayPtrF64 colon(dim_type start, dim_type stop){
 dim_type dims[2]={1,0};
 long m = fix<long,long>(stop-start);
 dims[1] = m+1; 
 VrArrayPtrF64 arr = vrAllocArrayF64RM(2, 0, dims);
 if(stop<start) {
 	VR_GET_DIMS_F64(arr)[1] = 0;
	return arr;
 }
 for (int i = 0; i <= m; i++) {
  VR_GET_DATA_F64(arr)[i]=start + i;
 }
 return arr;
}

VrArrayPtrF64 colon(dim_type start, dim_type step, dim_type stop) {
 dim_type dims[2]={1,0};
 long m = fix<long,dim_type>((stop - start) / step);
 dims[1]=m+1;
 VrArrayPtrF64 arr = vrAllocArrayF64RM(2,0,dims);
 if (step == 0) {
 	VR_GET_DATA_F64(arr)[1] = 0;
	return arr;
 }
 if (start < stop && step < 0) {
 	VR_GET_DATA_F64(arr)[1] = 0;
	return arr;
 }
 if (start > stop && step > 0) {
 	VR_GET_DATA_F64(arr)[1] = 0;
	return arr;
 }
 int iter = abs(m);
 for(int i = 0; i <= m; i++) {
  VR_GET_DATA_F64(arr)[i] = (start + i*step);
 }
return arr;
}



VrArrayPtrF64 sqrt (VrArrayPtrF64 A) {
  VrArrayPtrF64 outArr = vrAllocArrayF64RM(VR_GET_NDIMS_F64(A), 0, VR_GET_DIMS_F64(A));
  for(int i = 0; i < getNumElem(VR_GET_DIMS_F64(A),VR_GET_NDIMS_F64(A)); i++) {
    VR_GET_DATA_F64(outArr)[i] = sqrt(VR_GET_DATA_F64(A)[i]);
  }
  return outArr;
}

VrArrayPtrF64 rand(int nargs , ...) {
  if(nargs < 0) {
    std::cout<<"number of arguments cannot be less than 0 "<<std::endl;
  }
  int ndims;
  if(nargs == 0) {
    ndims =2;
  }else if(nargs == 1){
    ndims = 2;
  }else {
    ndims = nargs;
  }
  va_list args;
  va_start(args, nargs);
  bool zeroFlag = false;
  dim_type* dims = static_cast<dim_type*>(VR_MALLOC(sizeof(dim_type) * ndims));
  memset(dims,1,sizeof(dim_type) * ndims);
  for(int i = 0; i < nargs; i++) {
    dims[i] = va_arg(args,dim_type);
    if(dims[i] == 0 ) {
      zeroFlag = true;
    }
  }
  va_end(args);
  if(nargs == 1) {
    dims[1] = dims[0];
  }
  if(zeroFlag) {
    return vrAllocArrayF64RM(ndims,2,dims);
  }
  VrArrayPtrF64 A = vrAllocArrayF64RM(ndims,0,dims);
  for(int i = 0; i < getNumElem(dims,ndims); i++) {
    VR_GET_DATA_F64(A)[i] = rand();
  }
  return A;
}

VrArrayPtrF64 exp(VrArrayPtrF64 A) {
  int numel = getNumElem(VR_GET_DIMS_F64(A),VR_GET_NDIMS_F64(A));
  VrArrayPtrF64 B = vrAllocArrayF64RM(VR_GET_NDIMS_F64(A),0,VR_GET_DIMS_F64(A));
  for(int i = 0; i < numel; i++) {
    VR_GET_DATA_F64(B)[i] = exp(VR_GET_DATA_F64(A)[i]);
  }
}

VrArrayPtrF32 exp(VrArrayPtrF32 A) {
  int numel = getNumElem(VR_GET_DIMS_F64(A),VR_GET_NDIMS_F64(A));
  VrArrayPtrF32 B = vrAllocArrayF32RM(VR_GET_NDIMS_F32(A),0,VR_GET_DIMS_F32(A));
  for(int i = 0; i < numel; i++) {
    VR_GET_DATA_F32(B)[i] = exp(VR_GET_DATA_F32(A)[i]);
  }
}

VrArrayPtrCF64 cexp(VrArrayPtrCF64 A) {
  int numel = getNumElem(VR_GET_DIMS_CF64(A),VR_GET_NDIMS_CF64(A));
  VrArrayPtrCF64 B = vrAllocArrayF64CM(VR_GET_NDIMS_CF64(A),0,VR_GET_DIMS_CF64(A));
  for(int i = 0; i < numel; i++) {
    VR_GET_DATA_CF64(B)[i] = cexp(VR_GET_DATA_CF64(A)[i]);
  }
}

VrArrayPtrCF32 exp(VrArrayPtrCF32 A) {
  int numel = getNumElem(VR_GET_DIMS_CF64(A),VR_GET_NDIMS_CF64(A));
  VrArrayPtrCF32 B = vrAllocArrayF32CM(VR_GET_NDIMS_CF32(A),0,VR_GET_DIMS_CF32(A));
  for(int i = 0; i < numel; i++) {
    VR_GET_DATA_CF32(B)[i] = cexpf(VR_GET_DATA_CF32(A)[i]);
  }
}

float complex cexp(float complex scalVal) {
  return cexpf(scalVal);
}



