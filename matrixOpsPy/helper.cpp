#include"helper.hpp"
#include "complexdatahandler.h"
int getNumElem(dim_type * dims, dim_type ndims){
        int numel=1;

        for(int i=0;i<ndims;i++ ){
                numel*=dims[i];
        }
        return numel;
}

bool checkNdims(int ndims_a, int ndims_b) {
  return ndims_a==ndims_b;
}

bool checkdim(int dim_a, int dim_b) {
  return dim_a==dim_b;
}
VrArrayPtrF64 getVrArrayF64(PyArrayObject* arr){
    
    dim_type *dims = (dim_type*)(VR_MALLOC(sizeof(dim_type) * PyArray_NDIM(arr)));
    for( int i = 0; i < PyArray_NDIM(arr); i++) {
        dims[i] = PyArray_DIMS(arr)[i];
    }
	VrArrayPtrF64 out(static_cast<double*>(PyArray_DATA(arr)),dims,PyArray_NDIM(arr));
	return out;
}

VrArrayPtrI32 getVrArrayI32(PyArrayObject* arr){
	VrArrayPtrI32 out;//=(VrArrayPtrF32)VR_MALLOC(sizeof(VrArrayF32));
	VR_GET_DATA_I32(out)=(int*)PyArray_DATA(arr);
	VR_GET_NDIMS_I32(out)=PyArray_NDIM(arr);
    out.dims = (dim_type*)(VR_MALLOC(sizeof(dim_type) * out.ndims));
    for( int i = 0; i < out.ndims; i++) {
        out.dims[i] = PyArray_DIMS(arr)[i];
    }
	return out;
}
VrArrayPtrF32 getVrArrayF32(PyArrayObject* arr){
	VrArrayPtrF32 out;//=(VrArrayPtrF32)VR_MALLOC(sizeof(VrArrayF32));
	VR_GET_DATA_F32(out)=(float*)PyArray_DATA(arr);
	VR_GET_NDIMS_F32(out)=PyArray_NDIM(arr);
    out.dims = (dim_type*)(VR_MALLOC(sizeof(dim_type) * out.ndims));
    for( int i = 0; i < out.ndims; i++) {
        out.dims[i] = PyArray_DIMS(arr)[i];
    }
	return out;
}

VrArrayPtrB getVrArrayB(PyArrayObject* arr){
	VrArrayPtrB out;//=(VrArrayPtrB)VR_MALLOC(sizeof(VrArrayB));
	VR_GET_DATA_B(out)=(bool*)PyArray_DATA(arr);
	VR_GET_NDIMS_B(out)=PyArray_NDIM(arr);
	VR_GET_DIMS_B(out)=(dim_type*)PyArray_DIMS(arr);
	return out;
}

VrArrayPtrCF64 getVrArrayCF64(PyArrayObject * arr){
    VrArrayPtrCF64 out;//=(VrArrayPtrCF64)VR_MALLOC(sizeof(VrArrayCF64));
	VR_GET_DATA_CF64(out)= static_cast<double complex*>PyArray_DATA((arr));
	VR_GET_NDIMS_CF64(out)=PyArray_NDIM(arr);
	VR_GET_DIMS_CF64(out)=(dim_type*)PyArray_DIMS(arr);
	return out;
}

VrArrayPtrCF32 getVrArrayCF32(PyArrayObject * arr){
    VrArrayPtrCF32 out;//=(VrArrayPtrCF32)VR_MALLOC(sizeof(VrArrayCF32));
	VR_GET_DATA_CF32(out)=static_cast<float complex*>PyArray_DATA((arr));
	VR_GET_NDIMS_CF32(out)=PyArray_NDIM(arr);
	VR_GET_DIMS_CF32(out)=(dim_type*)PyArray_DIMS(arr);
	return out;
}
