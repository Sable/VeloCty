#include<library_ops.hpp>
#include<min.tpp>
double min_scalar(VrArrayPtrF64 A) {
	dim_type numel = getNumElem(A.dims,A.ndims);
	double outArr = min<double>(numel,VR_GET_DATA_F32(A));
	return outArr;
}
 
float min_scalar(VrArrayPtrF32 A) {
	dim_type numel = getNumElem(A.dims,A.ndims);
	float outArr = min<float>(numel,VR_GET_DATA_F32(A));
	return outArr;
} 

float complex  min_scalar(VrArrayPtrCF32 A) {
	dim_type numel = getNumElem(A.dims,A.ndims);
	float complex  outArr = min<float complex>(numel,VR_GET_DATA_F32(A));
	return outArr;
} 

double complex  min_scalar(VrArrayPtrCF64 A) {
	dim_type numel = getNumElem(A.dims,A.ndims);
	double complex  outArr = min<double complex>(numel,VR_GET_DATA_F32(A));
	return outArr;
}
