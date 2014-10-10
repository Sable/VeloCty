#include<library_ops.hpp>
#include<min.tpp>
double min_scalar(VrArrayPtrF64 A) {
    dim_type numel = getNumElem(A.dims,A.ndims);
    double minVal = DBL_MAX;
    for(int i = 0; i < numel; i++) {
       if(A.data[i] < minVal) {
            minVal = A.data[i];
        }
    }
    return minVal;
}
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
