#include "library_ops.hpp"
double mean_scalar(VrArrayPtrF64 A) {
	dim_type numel = getNumElem(A.dims,A.ndims);
	double sum = 0, out;
	for (dim_type i = 0; i < numel; i++) {
		sum +=A.data[i];
	}		 	 
	out = sum / numel;
	return out;
}

float mean_scalar(VrArrayPtrF32 A) {
	float sum = 0, out;
	dim_type numel = getNumElem(A.dims,A.ndims);
	for (dim_type i = 0; i < numel; i++) {
		sum +=A.data[i];
	}		 	 
  out = sum / numel;
  return out;
}

double complex mean_scalar(VrArrayPtrCF64 A) {
	double complex  sum = 0, out;
	dim_type numel = getNumElem(A.dims,A.ndims);
	for (dim_type i = 0; i < numel; i++) {
		sum += A.data[i];
	}		 	 
	out = sum / numel;
	return out;
}

float complex mean_scalar(VrArrayPtrCF32 A) {
	float complex sum = 0, out;
	dim_type numel = getNumElem(A.dims,A.ndims);
	for (dim_type i = 0; i < numel; i++) {
		float complex sum = 0;
			sum +=A.data[i];
	}		 	 
	out = sum / numel;
   return out;
}
