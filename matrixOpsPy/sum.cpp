#include "library_ops.hpp"
#include "sum.tpp"
double sum(VrArrayPtrF64 A) {
	double out;
	dim_type  numel = getNumElem(A.dims,A.ndims);
	out = sum<double>(numel,VR_GET_DATA_F64(A));
	return out;
} 
