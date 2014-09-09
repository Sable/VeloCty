#include<library_ops.hpp>
#include<min.tpp>
#ifdef __MATLAB__
VrArrayPtrF64 min(VrArrayPtrF64 A) {
	dim_type collapseDim = getIndexOfFirstNonSingletonDim<VrArrayPtrF64,dim_type>(A);
	dim_type step = getStep<VrArrayPtrF64,dim_type>(A,collapseDim);
	dim_type size = getSize<VrArrayPtrF64,dim_type>(A,collapseDim);	
	dim_type *dims = static_cast<dim_type*>(VR_MALLOC(sizeof(dim_type)*A.ndims));
	memcpy(dims,A.dims,sizeof(dim_type)*A.ndims);
	dims[collapseDim] = 1;
    dim_type bucketSize = getCollapsedDimension<VrArrayPtrF64,dim_type>(A,collapseDim);
	int ndims = A.ndims;
	VrArrayPtrF64 outArr = vrAllocArrayF64RM(ndims,0,dims);
	VR_GET_DATA_F64(outArr) = min<double,dim_type>(VR_GET_DATA_F64(A),collapseDim,bucketSize,size,step);
	return outArr;
}
 
VrArrayPtrF32 min(VrArrayPtrF32 A) {
	dim_type collapseDim = getIndexOfFirstNonSingletonDim<VrArrayPtrF32,dim_type>(A);
	dim_type step = getStep<VrArrayPtrF32,dim_type>(A,collapseDim);
	dim_type size = getSize<VrArrayPtrF32,dim_type>(A,collapseDim);	
	dim_type *dims = static_cast<dim_type*>(VR_MALLOC(sizeof(dim_type)*A.ndims));
	memcpy(dims,A.dims,sizeof(dim_type)*A.ndims);
	dims[collapseDim] = 1;
    dim_type bucketSize = getCollapsedDimension<VrArrayPtrF32,dim_type>(A,collapseDim);
	int ndims = A.ndims;
	VrArrayPtrF32 outArr = vrAllocArrayF32RM(ndims,0,dims);
	VR_GET_DATA_F32(outArr) = min<float,dim_type>(VR_GET_DATA_F32(A),collapseDim,bucketSize,size,step);
	return outArr;
} 

VrArrayPtrCF32 min(VrArrayPtrCF32 A) {
	dim_type collapseDim = getIndexOfFirstNonSingletonDim<VrArrayPtrCF32,dim_type>(A);
	dim_type step = getStep<VrArrayPtrCF32,dim_type>(A,collapseDim);
	dim_type size = getSize<VrArrayPtrCF32,dim_type>(A,collapseDim);	
	dim_type *dims = static_cast<dim_type*>(VR_MALLOC(sizeof(dim_type)*A.ndims));
	memcpy(dims,A.dims,sizeof(dim_type)*A.ndims);
	dims[collapseDim] = 1;
    dim_type bucketSize = getCollapsedDimension<VrArrayPtrCF32,dim_type>(A,collapseDim);
	int ndims = A.ndims;
	VrArrayPtrCF32 outArr = vrAllocArrayF32CM(ndims,0,dims);
	VR_GET_DATA_CF32(outArr) = min<dim_type>(VR_GET_DATA_CF32(A),collapseDim,bucketSize,size,step);
	return outArr;
} 

VrArrayPtrCF64 min(VrArrayPtrCF64 A) {
	dim_type collapseDim = getIndexOfFirstNonSingletonDim<VrArrayPtrCF64,dim_type>(A);
	dim_type step = getStep<VrArrayPtrCF64,dim_type>(A,collapseDim);
	dim_type size = getSize<VrArrayPtrCF64,dim_type>(A,collapseDim);	
	dim_type *dims = static_cast<dim_type*>(VR_MALLOC(sizeof(dim_type)*A.ndims));
	memcpy(dims,A.dims,sizeof(dim_type)*A.ndims);
	dims[collapseDim] = 1;
    dim_type bucketSize = getCollapsedDimension<VrArrayPtrCF64,dim_type>(A,collapseDim);
	int ndims = A.ndims;
	VrArrayPtrCF64 outArr = vrAllocArrayF64CM(ndims,0,dims);
	VR_GET_DATA_CF64(outArr) = min<dim_type>(VR_GET_DATA_CF64(A),collapseDim,bucketSize,size,step);
	return outArr;
}
#elif defined __PYTHON__
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
#endif
