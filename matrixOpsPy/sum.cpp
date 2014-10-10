#include "library_ops.hpp"
#include "sum.tpp"
double sum_scalar(VrArrayPtrF64 A) {
	double out;
	dim_type  numel = getNumElem(A.dims,A.ndims);
	out = sum<double>(numel,VR_GET_DATA_F64(A));
	return out;
} 

float sum_scalar(VrArrayPtrF32 A) {
	float out;
	dim_type  numel = getNumElem(A.dims,A.ndims);
	out = sum<float>(numel,VR_GET_DATA_F32(A));
	return out;
} 

double complex sum_scalar(VrArrayPtrCF64 A) {
	double complex out;
	dim_type  numel = getNumElem(A.dims,A.ndims);
	out = sum<double complex>(numel,VR_GET_DATA_CF64(A));
	return out;
} 

float complex sum_scalar(VrArrayPtrCF32 A) {
	float complex out;
	dim_type  numel = getNumElem(A.dims,A.ndims);
	out = sum<float complex>(numel,VR_GET_DATA_CF32(A));
	return out;
} 
