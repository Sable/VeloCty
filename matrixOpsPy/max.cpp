#include<library_ops.hpp>
#include<max.tpp>
double max(VrArrayPtrF64 A) {
	double out; 
	dim_type numel = getNumElem(A.dims,A.ndims);
	out = max<double>(numel, VR_GET_DATA_F64(A));
	return out;
} 

float max(VrArrayPtrF32 A) {
	float out; 
	dim_type numel = getNumElem(A.dims,A.ndims);
	out = max<float>(numel, VR_GET_DATA_F64(A));
	return out;
} 

float complex  max(VrArrayPtrCF32 A) {
	float complex  out; 
	dim_type numel = getNumElem(A.dims,A.ndims);
	out = max<float complex>(numel, VR_GET_DATA_F64(A));
	return out;
} 
double complex  max(VrArrayPtrCF64 A) {
	double complex  out; 
	dim_type numel = getNumElem(A.dims,A.ndims);
	out = max<double complex>(numel, VR_GET_DATA_F64(A));
	return out;
}
