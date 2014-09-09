#include<library_ops.hpp>
#include<min.tpp>
double min(VrArrayPtrF64 A) {
	dim_type numel = getNumElem(A.dims,A.ndims);
	double outArr = min<double>(numel,VR_GET_DATA_F32(A));
	return outArr;
}
 
float min(VrArrayPtrF32 A) {
	dim_type numel = getNumElem(A.dims,A.ndims);
	float outArr = min<float>(numel,VR_GET_DATA_F32(A));
	return outArr;
} 

float complex  min(VrArrayPtrCF32 A) {
	dim_type numel = getNumElem(A.dims,A.ndims);
	float complex  outArr = min<float complex>(numel,VR_GET_DATA_F32(A));
	return outArr;
} 

double complex  min(VrArrayPtrCF64 A) {
	dim_type numel = getNumElem(A.dims,A.ndims);
	double complex  outArr = min<double complex>(numel,VR_GET_DATA_F32(A));
	return outArr;
}
