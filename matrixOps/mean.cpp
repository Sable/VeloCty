#include "library_ops.hpp"
#ifdef __MATLAB__
VrArrayPtrF64 mean(VrArrayPtrF64 A) {
	dim_type collapseDim = getIndexOfFirstNonSingletonDim<VrArrayPtrF64,dim_type>(A);
	dim_type step = getStep<VrArrayPtrF64,dim_type>(A,collapseDim);
	dim_type size = getSize<VrArrayPtrF64,dim_type>(A,collapseDim);	
	dim_type *dims = static_cast<dim_type*>(VR_MALLOC(sizeof(dim_type)*A.ndims));
	memcpy(dims,A.dims,sizeof(dim_type)*A.ndims);
	dims[collapseDim] = 1;
    dim_type bucketSize = getCollapsedDimension<VrArrayPtrF64,dim_type>(A,collapseDim);
	int ndims = A.ndims;
	VrArrayF64 outArr = vrAllocArrayF64RM(ndims,0,dims);	
	for (dim_type i = 0; i < bucketSize; i++) {
		double sum = 0;
		for(dim_type j = 0; j < size; j++ ) {
			sum +=A.data[i*size + j*step ];
		}	
		outArr.data[i] = sum / size;
	}		 	 
	return outArr;
}

VrArrayPtrF32 mean(VrArrayPtrF32 A) {
	dim_type collapseDim = getIndexOfFirstNonSingletonDim<VrArrayPtrF32,dim_type>(A);
	dim_type step = getStep<VrArrayPtrF32,dim_type>(A,collapseDim);
	dim_type size = getSize<VrArrayPtrF32,dim_type>(A,collapseDim);	
	dim_type *dims = static_cast<dim_type*>(VR_MALLOC(sizeof(dim_type)*A.ndims));
	memcpy(dims,A.dims,sizeof(dim_type)*A.ndims);
	dims[collapseDim] = 1;
    dim_type bucketSize = getCollapsedDimension<VrArrayPtrF32,dim_type>(A,collapseDim);
	int ndims = A.ndims;
	VrArrayF32 outArr = vrAllocArrayF32RM(ndims,0,dims);	
	for (dim_type i = 0; i < bucketSize; i++) {
		float sum = 0;
		for(dim_type j = 0; j < size; j++ ) {
			sum +=A.data[i*size + j*step ];
		}	
		outArr.data[i] = sum / size;
	}		 	 
  return outArr;
}

VrArrayPtrCF64 mean(VrArrayPtrCF64 A) {
	dim_type collapseDim = getIndexOfFirstNonSingletonDim<VrArrayPtrCF64,dim_type>(A);
	dim_type step = getStep<VrArrayPtrCF64,dim_type>(A,collapseDim);
	dim_type size = getSize<VrArrayPtrCF64,dim_type>(A,collapseDim);	
	dim_type *dims = static_cast<dim_type*>(VR_MALLOC(sizeof(dim_type)*A.ndims));
	memcpy(dims,A.dims,sizeof(dim_type)*A.ndims);
	dims[collapseDim] = 1;
    dim_type bucketSize = getCollapsedDimension<VrArrayPtrCF64,dim_type>(A,collapseDim);
	int ndims = A.ndims;
	VrArrayCF64 outArr = vrAllocArrayF64CM(ndims,0,dims);	
	for (dim_type i = 0; i < bucketSize; i++) {
		double complex sum = 0;
		for(dim_type j = 0; j < size; j++ ) {
			sum +=A.data[i*size + j*step ];
		}	
		outArr.data[i] = sum / size;
	}		 	 
	return outArr;
}

VrArrayPtrCF32 mean(VrArrayPtrCF32 A) {
	dim_type collapseDim = getIndexOfFirstNonSingletonDim<VrArrayPtrCF32,dim_type>(A);
	dim_type step = getStep<VrArrayPtrCF32,dim_type>(A,collapseDim);
	dim_type size = getSize<VrArrayPtrCF32,dim_type>(A,collapseDim);	
	dim_type *dims = static_cast<dim_type*>(VR_MALLOC(sizeof(dim_type)*A.ndims));
	memcpy(dims,A.dims,sizeof(dim_type)*A.ndims);
	dims[collapseDim] = 1;
    dim_type bucketSize = getCollapsedDimension<VrArrayPtrCF32,dim_type>(A,collapseDim);
	int ndims = A.ndims;
	VrArrayCF32 outArr = vrAllocArrayF32CM(ndims,0,dims);	
	for (dim_type i = 0; i < bucketSize; i++) {
		float complex sum = 0;
		for(dim_type j = 0; j < size; j++ ) {
			sum +=A.data[i*size + j*step ];
		}	
		outArr.data[i] = sum / size;
	}		 	 
  return outArr;
}
#elif defined __PYTHON__
double mean(VrArrayPtrF64 A) {
	dim_type numel = getNumElem(A.dims,A.ndims);
	double sum = 0, out;
	for (dim_type i = 0; i < numel; i++) {
		sum +=A.data[i];
	}		 	 
	out = sum / numel;
	return out;
}

float mean(VrArrayPtrF32 A) {
	float sum = 0, out;
	dim_type numel = getNumElem(A.dims,A.ndims);
	for (dim_type i = 0; i < numel; i++) {
		sum +=A.data[i];
	}		 	 
  out = sum / numel;
  return out;
}

double complex mean(VrArrayPtrCF64 A) {
	double complex  sum = 0, out;
	dim_type numel = getNumElem(A.dims,A.ndims);
	for (dim_type i = 0; i < numel; i++) {
		sum += A.data[i];
	}		 	 
	out = sum / numel;
	return out;
}

float complex mean(VrArrayPtrCF32 A) {
	float complex sum = 0, out;
	dim_type numel = getNumElem(A.dims,A.ndims);
	for (dim_type i = 0; i < numel; i++) {
		float complex sum = 0;
			sum +=A.data[i];
	}		 	 
	out = sum / numel;
   return out;
}
#endif
