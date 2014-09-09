#ifndef _PYBINDING_H
#define _PYBINDING_H
#ifndef VR_MINIMAL
#include "vrbinding_llvm.hpp"
#endif
//#include <Poco/Types.h>
//#include<complex.h>
//#include "complexdatahandler.h"
//#define VR_ZERO_INDEX

#ifdef VR_ZERO_INDEX 
const bool vrZeroBasedIndex = true;
#else
const bool vrZeroBasedIndex = false;
#endif

#define VR_FLATTEN_INDEX

#ifdef VR_FLATTEN_INDEX
const bool vrFlattenIndex = true;
#else
const bool vrFlattenIndex = false;
#endif 

#define VR_MIN_TWO_DIMS

#ifdef VR_MIN_TWO_DIMS
const bool vrMinTwoDims = true;
#else
const bool vrMinTwoDims = false;
#endif

//#define VR_AUTO_UPCAST_MATRIX
#ifdef VR_AUTO_UPCAST_MATRIX
const bool vrAutoCastMatrix = true; 
#else
const bool vrAutoCastMatrix = false;
#endif
#define VR_MALLOC(x) mxMalloc(x)
#define VR_REALLOC(x,y) (mxRealloc(x,y))
#define VR_FREE(x) mxFree(x)
/* #define VR_MALLOC(x) GC_MALLOC_ATOMIC(x) */
/* #define VR_FREE(x) GC_FREE(x) */
extern "C"{
#include "mex.h"
#include "vr_data.hpp"
#define VR_PRINT_ERR(str) mexErrMsgTxt(str)
typedef VrArrayF32 VrArrayPtrF32;
typedef VrArrayF64 VrArrayPtrF64;
typedef VrArrayI32 VrArrayPtrI32;
typedef VrArrayI64 VrArrayPtrI64;
typedef VrArrayB VrArrayPtrB;
typedef VrArrayCF32 VrArrayPtrCF32;
typedef VrArrayCF64 VrArrayPtrCF64;
typedef VrArrayCI32 VrArrayPtrCI32;
typedef VrArrayCI64 VrArrayPtrCI64;
//typedef VrArrayCB *VrArrayPtrCB;

typedef long VrDimsInt_t;
typedef long VrStrideInt_t;

extern float complex  * mat_getComplexDataCF32(mxArray*);
extern double complex* mat_getComplexDataCF64(VrArrayPtrCF64);
//extern double * mat_getComplexDataCF64(mxArray*);
#define VR_GET_NDIMS_F32(x) (x.ndims)
#define VR_GET_NDIMS_F64(x) (x.ndims) 
#define VR_GET_NDIMS_I32(x) (x.ndims) 
#define VR_GET_NDIMS_I64(x) (x.ndims) 
#define VR_GET_NDIMS_CF32(x) (x.ndims)
#define VR_GET_NDIMS_CF64(x) (x.ndims)
#define VR_GET_NDIMS_CI32(x) (x.ndims)
#define VR_GET_NDIMS_CI64(x) (x.ndims)
#define VR_GET_NDIMS_B(x) (x.ndims)

#define VR_GET_DIMS_F32(x) (x.dims)
#define VR_GET_DIMS_F64(x) (x.dims) 
#define VR_GET_DIMS_I32(x) (x.dims) 
#define VR_GET_DIMS_I64(x) (x.dims) 
#define VR_GET_DIMS_CF32(x) (x.dims)
#define VR_GET_DIMS_CF64(x) (x.dims)
#define VR_GET_DIMS_CI32(x) (x.dims)
#define VR_GET_DIMS_CI64(x) (x.dims)
#define VR_GET_DIMS_B(x) (x.dims)
/*
#define VR_GET_STRIDES_F32(x) ((x).strides)
#define VR_GET_STRIDES_F64(x) ((x).strides)
#define VR_GET_STRIDES_I32(x) ((x).strides)
#define VR_GET_STRIDES_I64(x) ((x).strides)

#define VR_GET_STRIDES_CF32(x) ((x).strides)
#define VR_GET_STRIDES_CF64(x) ((x).strides)
#define VR_GET_STRIDES_CI32(x) ((x).strides)
#define VR_GET_STRIDES_CI64(x) ((x).strides)
*/
#define VR_GET_DATA_F32(x)  (x.data)
#define VR_GET_DATA_F64(x)  (x.data)
#define VR_GET_DATA_I32(x)  (x.data)
#define VR_GET_DATA_I64(x)  (x.data)
#define VR_GET_DATA_B(x)  (x.data)

#define VR_GET_DATA_CF32(x)  ((x).data)
#define VR_GET_DATA_CF64(x)  ((x).data)
#define VR_GET_DATA_CI32(x)  ((x).data)
#define VR_GET_DATA_CI64(x)  ((x).data)
 //TODO: No complex representation for arrays of Ints . what to do  ? 
extern void vrRefCountInc(void*);
extern void vrRefCountDec(void*);
//func. zeros = 0, ones = 1, empty = 2.
/*
extern VrArrayPtrF32 vrAllocArrayF32RM(Poco::Int32 ndims,Poco::Int32 func,Poco::Int64 *sizes);
extern VrArrayPtrF64 vrAllocArrayF64RM(Poco::Int32 ndims,Poco::Int32 func,Poco::Int64 *sizes);
extern VrArrayPtrI32 vrAllocArrayI32RM(Poco::Int32 ndims,Poco::Int32 func,Poco::Int64 *sizes);
extern VrArrayPtrI64 vrAllocArrayI64RM(Poco::Int32 ndims,Poco::Int32 func,Poco::Int64 *sizes);

extern VrArrayPtrCF32 vrAllocArrayF32CM(Poco::Int32 ndims,Poco::Int32 func,Poco::Int64 *sizes);
extern VrArrayPtrCF64 vrAllocArrayF64CM(Poco::Int32 ndims,Poco::Int32 func,Poco::Int64 *sizes);
extern VrArrayPtrCI32 vrAllocArrayI32CM(Poco::Int32 ndims,Poco::Int32 func,Poco::Int64 *sizes);
extern VrArrayPtrCI64 vrAllocArrayI64CM(Poco::Int32 ndims,Poco::Int32 func,Poco::Int64 *sizes);
*/
/*extern VrArrayPtrF32 vrAllocArrayF32RM(int ndims,int func,long *sizes);
extern VrArrayPtrF64 vrAllocArrayF64RM(int ndims,int func,long *sizes);
extern VrArrayPtrI32 vrAllocArrayI32RM(int ndims,int func,long *sizes);
extern VrArrayPtrI64 vrAllocArrayI64RM(int ndims,int func,long *sizes);

extern VrArrayPtrF32 vrAllocArrayF32CM(int ndims,int func,long *sizes);
extern VrArrayPtrF64 vrAllocArrayF64CM(int ndims,int func,long *sizes);
extern VrArrayPtrI32 vrAllocArrayI32CM(int ndims,int func,long *sizes);
extern VrArrayPtrI64 vrAllocArrayI64CM(int ndims,int func,long *sizes);
*/

extern VrArrayPtrF32 vrAllocArrayF32RM(int ndims,int func,int *sizes);
extern VrArrayPtrF64 vrAllocArrayF64RM(int ndims,int func,int *sizes);
extern VrArrayPtrI32 vrAllocArrayI32RM(int ndims,int func,int *sizes);
extern VrArrayPtrI64 vrAllocArrayI64RM(int ndims,int func,int *sizes);

extern VrArrayPtrCF32 vrAllocArrayF32CM(int ndims,int func,int *sizes);
extern VrArrayPtrCF64 vrAllocArrayF64CM(int ndims,int func,int *sizes);
extern VrArrayPtrCI32 vrAllocArrayI32CM(int ndims,int func,int *sizes);
extern VrArrayPtrCI64 vrAllocArrayI64CM(int ndims,int func,int *sizes);

extern VrArrayPtrF64 vrCloneArrayF64RM(VrArrayPtrF64  );
enum VrErrCodes {VR_OUT_OF_BOUNDS=0, VR_DIV_BY_ZERO, VR_SHAPE_MISMATCH};

//argDesc describes argument. argDesc[i] in i-th dimension. 0 = integer index, 1 = slice (i.e. 3 values)
/*extern VrArrayPtrF32 vrSliceF32_ref(VrArrayPtrF32 *arr,Poco::Int32 nindices,Poco::Int32 *argDesc,Poco::Int64 *indices,VrErrCodes *err);
extern VrArrayPtrF64 vrSliceF64_ref(VrArrayPtrF64 *arr,Poco::Int32 nindices,Poco::Int32 *argDesc,Poco::Int64 *indices,VrErrCodes *err);
extern VrArrayPtrI32 vrSliceI32_ref(VrArrayPtrI32 *arr,Poco::Int32 nindices,Poco::Int32 *argDesc,Poco::Int64 *indices,VrErrCodes *err);
extern VrArrayPtrI64 vrSliceI64_ref(VrArrayPtrI64 *arr,Poco::Int32 nindices,Poco::Int32 *argDesc,Poco::Int64 *indices,VrErrCodes *err);

extern VrArrayPtrCF32 vrSliceCF32_ref(VrArrayPtrCF32 *arr,Poco::Int32 nindices,Poco::Int32 *argDesc,Poco::Int64 *indices,VrErrCodes *err);
extern VrArrayPtrCF64 vrSliceCF64_ref(VrArrayPtrCF64 *arr,Poco::Int32 nindices,Poco::Int32 *argDesc,Poco::Int64 *indices,VrErrCodes *err);
extern VrArrayPtrCI32 vrSliceCI32_ref(VrArrayPtrCI32 *arr,Poco::Int32 nindices,Poco::Int32 *argDesc,Poco::Int64 *indices,VrErrCodes *err);
extern VrArrayPtrCI64 vrSliceCI64_ref(VrArrayPtrCI64 *arr,Poco::Int32 nindices,Poco::Int32 *argDesc,Poco::Int64 *indices,VrErrCodes *err);

extern VrArrayPtrF32 vrSliceF32_copy(VrArrayPtrF32 *arr,Poco::Int32 nindices,Poco::Int32 *argDesc,Poco::Int64 *indices,VrErrCodes *err);
extern VrArrayPtrF64 vrSliceF64_copy(VrArrayPtrF64 *arr,Poco::Int32 nindices,Poco::Int32 *argDesc,Poco::Int64 *indices,VrErrCodes *err);
extern VrArrayPtrI32 vrSliceI32_copy(VrArrayPtrI32 *arr,Poco::Int32 nindices,Poco::Int32 *argDesc,Poco::Int64 *indices,VrErrCodes *err);
extern VrArrayPtrI64 vrSliceI64_copy(VrArrayPtrI64 *arr,Poco::Int32 nindices,Poco::Int32 *argDesc,Poco::Int64 *indices,VrErrCodes *err);

extern VrArrayPtrCF32 vrSliceCF32_copy(VrArrayPtrCF32 *arr,Poco::Int32 nindices,Poco::Int32 *argDesc,Poco::Int64 *indices,VrErrCodes *err);
extern VrArrayPtrCF64 vrSliceCF64_copy(VrArrayPtrCF64 *arr,Poco::Int32 nindices,Poco::Int32 *argDesc,Poco::Int64 *indices,VrErrCodes *err);
extern VrArrayPtrCI32 vrSliceCI32_copy(VrArrayPtrCI32 *arr,Poco::Int32 nindices,Poco::Int32 *argDesc,Poco::Int64 *indices,VrErrCodes *err);
extern VrArrayPtrCI64 vrSliceCI64_copy(VrArrayPtrCI64 *arr,Poco::Int32 nindices,Poco::Int32 *argDesc,Poco::Int64 *indices,VrErrCodes *err);

//argDesc: 0 = int64 index, 1 = slice (3 int64 values), 2 = int32 array (this will consume two args, first the number of dims and then the actual array itself), 3 = int64 array (this will consume two arguments, first the number of dims and then the value), 4 = f32 array (consumes two args), 5 = f64 array (consumes two args)
extern VrArrayPtrF32 vrSliceF32_indirect(VrArrayPtrF32 *arr,Poco::Int32 ndims,Poco::Int32 *argDesc,void *args,VrErrCodes *err);
extern VrArrayPtrF64 vrSliceF64_indirect(VrArrayPtrF64 *arr,Poco::Int32 ndims,Poco::Int32 *argDesc,void *args,VrErrCodes *err);
extern VrArrayPtrI32 vrSliceI32_indirect(VrArrayPtrI32 *arr,Poco::Int32 ndims,Poco::Int32 *argDesc,void *args,VrErrCodes *err);
extern VrArrayPtrI64 vrSliceI64_indirect(VrArrayPtrI64 *arr,Poco::Int32 ndims,Poco::Int32 *argDesc,void *args,VrErrCodes *err);

extern VrArrayPtrCF32 vrSliceCF32_indirect(VrArrayPtrCF32 *arr,Poco::Int32 ndims,Poco::Int32 *argDesc,void *args,VrErrCodes *err);
extern VrArrayPtrCF64 vrSliceCF64_indirect(VrArrayPtrCF64 *arr,Poco::Int32 ndims,Poco::Int32 *argDesc,void *args,VrErrCodes *err);
extern VrArrayPtrCI32 vrSliceCI32_indirect(VrArrayPtrCI32 *arr,Poco::Int32 ndims,Poco::Int32 *argDesc,void *args,VrErrCodes *err);
extern VrArrayPtrCI64 vrSliceCI64_indirect(VrArrayPtrCI64 *arr,Poco::Int32 ndims,Poco::Int32 *argDesc,void *args,VrErrCodes *err);
*/
//TODO : Cell arrays ?
/*
typedef PyObject VrTupleType;
typedef PyObject *VrTupleTypePtr;
*?
VrTupleTypePtr vrAllocTuple(Poco::Int32 len);
void *vrGetTupleElem(Poco::Int32 i,VrErrCodes *errcode);
void vrSetTupleItem(Poco::Int32 index,void *item,VrErrCodes *errcode);
*/
}


#endif
