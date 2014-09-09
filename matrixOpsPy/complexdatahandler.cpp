#include"complexdatahandler.h"
#include<iostream>
float complex  * mat_getComplexDataCF32(PyArrayObject* arr){
    return static_cast<float complex*>(PyArray_DATA(arr));
}

double complex * mat_getComplexDataCF64(PyArrayObject* arr){

    return static_cast<double complex*>(PyArray_DATA(arr));

}
void mat_setComplexDataCF64(PyArrayObject* arr, VrArrayPtrCF64 B){
	int numel = getNumElem(VR_GET_DIMS_CF64(B),VR_GET_NDIMS_F64(B));
	memcpy(PyArray_DATA(arr),B.data,sizeof(double complex)*numel);	
}


void mat_setComplexDataCF32(PyArrayObject* arr, VrArrayPtrCF32 B){
	int numel = getNumElem(VR_GET_DIMS_CF64(B),VR_GET_NDIMS_F64(B));
	memcpy(PyArray_DATA(arr),B.data,sizeof(float complex)*numel);	
}
