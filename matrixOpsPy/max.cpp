#include<library_ops.hpp>
#include<max.tpp>
double max_scalar(VrArrayPtrF64 A) {
	double out; 
	dim_type numel = getNumElem(A.dims,A.ndims);
	out = max<double>(numel, VR_GET_DATA_F64(A));
	return out;
} 

float max_scalar(VrArrayPtrF32 A) {
	float out; 
	dim_type numel = getNumElem(A.dims,A.ndims);
	out = max<float>(numel, VR_GET_DATA_F64(A));
	return out;
} 

float complex  max_scalar(VrArrayPtrCF32 A) {
	float complex  out; 
	dim_type numel = getNumElem(A.dims,A.ndims);
	out = max<float complex>(numel, VR_GET_DATA_F64(A));
	return out;
} 
double complex  max_scalar(VrArrayPtrCF64 A) {
	double complex  out; 
	dim_type numel = getNumElem(A.dims,A.ndims);
	out = max<double complex>(numel, VR_GET_DATA_F64(A));
	return out;
}
