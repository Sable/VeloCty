#include "library_ops.hpp"
#include "sum.tpp"
void sum(VrArrayPtrF64 a, double *out){
    dim_type numel = getNumElem(a.dims,a.ndims);
    *out = 0;
    for(int i = 0; i < numel; i++) {
       *out += a.data[i]; 
    }
}

double sum_scalar(VrArrayPtrF64 A) {
    return sum<VrArrayPtrF64,double>(A);
}

float sum_scalar(VrArrayPtrF32 A) {
    return sum<VrArrayPtrF32,float>(A);
}

float complex sum_scalar(VrArrayPtrCF32 A) {
    return sum<VrArrayCF32,float complex>(A);
}

double complex sum_scalar(VrArrayPtrCF64 A){
    return sum<VrArrayCF64,double complex>(A);    
}

VrArrayPtrF64 sum(VrArrayPtrF64 A) {
	dim_type collapseDim = getIndexOfFirstNonSingletonDim<VrArrayPtrF64,dim_type>(A);
	dim_type step = getStep<VrArrayPtrF64,dim_type>(A,collapseDim);
	dim_type size = getSize<VrArrayPtrF64,dim_type>(A,collapseDim);	
	dim_type *dims = static_cast<dim_type*>(VR_MALLOC(sizeof(dim_type)*A.ndims));
	memcpy(dims,A.dims,sizeof(dim_type)*A.ndims);
	dims[collapseDim] = 1;
    dim_type bucketSize = getCollapsedDimension<VrArrayPtrF64,dim_type>(A,collapseDim);
	int ndims = A.ndims;
	VrArrayPtrF64 outArr = vrAllocArrayF64RM(ndims,0,dims);
	sum<double,dim_type>(VR_GET_DATA_F64(A),VR_GET_DATA_F64(outArr),collapseDim,bucketSize,size,step);
	return outArr;
} 

VrArrayPtrF32 sum(VrArrayPtrF32 A) {
	dim_type collapseDim = getIndexOfFirstNonSingletonDim<VrArrayPtrF32,dim_type>(A);
	dim_type step = getStep<VrArrayPtrF32,dim_type>(A,collapseDim);
	dim_type size = getSize<VrArrayPtrF32,dim_type>(A,collapseDim);	
	dim_type *dims = static_cast<dim_type*>(VR_MALLOC(sizeof(dim_type)*A.ndims));
	memcpy(dims,A.dims,sizeof(dim_type)*A.ndims);
	dims[collapseDim] = 1;
    dim_type bucketSize = getCollapsedDimension<VrArrayPtrF32,dim_type>(A,collapseDim);
	int ndims = A.ndims;
	VrArrayPtrF32 outArr = vrAllocArrayF32RM(ndims,0,dims);
	sum<float,dim_type>(VR_GET_DATA_F32(A),VR_GET_DATA_F32(outArr),collapseDim,bucketSize,size,step);
	return outArr;
} 
VrArrayPtrCF32  sum(VrArrayPtrCF32 A) {
	dim_type collapseDim = getIndexOfFirstNonSingletonDim<VrArrayPtrCF32,dim_type>(A);
	dim_type step = getStep<VrArrayPtrCF32,dim_type>(A,collapseDim);
	dim_type size = getSize<VrArrayPtrCF32,dim_type>(A,collapseDim);	
	dim_type *dims = static_cast<dim_type*>(VR_MALLOC(sizeof(dim_type)*A.ndims));
	memcpy(dims,A.dims,sizeof(dim_type)*A.ndims);
	dims[collapseDim] = 1;
    dim_type bucketSize = getCollapsedDimension<VrArrayPtrCF32,dim_type>(A,collapseDim);
	int ndims = A.ndims;
	VrArrayPtrCF32 outArr = vrAllocArrayF32CM(ndims,0,dims);
	sum<float complex,dim_type>(VR_GET_DATA_CF32(A),VR_GET_DATA_CF32(outArr),collapseDim,bucketSize,size,step);
	return outArr;
} 
VrArrayPtrCF64  sum(VrArrayPtrCF64 A) {
	dim_type collapseDim = getIndexOfFirstNonSingletonDim<VrArrayPtrCF64,dim_type>(A);
	dim_type step = getStep<VrArrayPtrCF64,dim_type>(A,collapseDim);
	dim_type size = getSize<VrArrayPtrCF64,dim_type>(A,collapseDim);	
	dim_type *dims = static_cast<dim_type*>(VR_MALLOC(sizeof(dim_type)*A.ndims));
	memcpy(dims,A.dims,sizeof(dim_type)*A.ndims);
	dims[collapseDim] = 1;
    dim_type bucketSize = getCollapsedDimension<VrArrayPtrCF64,dim_type>(A,collapseDim);
	int ndims = A.ndims;
	VrArrayPtrCF64 outArr = vrAllocArrayF64CM(ndims,0,dims);
	sum<double complex,dim_type>(VR_GET_DATA_CF64(A),VR_GET_DATA_CF64(outArr),collapseDim,bucketSize,size,step);
	return outArr;
}
