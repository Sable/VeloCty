#include<library_ops.hpp>


const int indx_diff =0;
dim_type* size(VrArrayPtrF64 arr) {
	return arr.dims;
}

dim_type size(VrArrayPtrF64 arr,dim_type dimIndex) {
	if(dimIndex - indx_diff < 0) {
		VR_PRINT_ERR("Dimensions have to be positive");
	}
	return arr.ndims?arr.dims[dimIndex - indx_diff]:1;
}

dim_type* size(VrArrayPtrCF64 arr) {
	return arr.dims;
}

dim_type size(VrArrayPtrI32 arr, dim_type dimIndex) {
	if(dimIndex - indx_diff < 0) {
		VR_PRINT_ERR("Dimensions have to be positive");
	}
	return arr.ndims?arr.dims[dimIndex - indx_diff]:1;
}

dim_type* size(VrArrayPtrI32 arr) {
	return arr.dims;
}
dim_type size(VrArrayPtrCF64 arr,dim_type dimIndex) {
	if(dimIndex - indx_diff  < 0) {
		VR_PRINT_ERR("Dimensions have to be positive");
	}
	return dimIndex <= arr.ndims?arr.dims[dimIndex - indx_diff]:1;
}
dim_type* size(VrArrayPtrF32 arr) {
	return arr.dims;
}

dim_type size(VrArrayPtrF32 arr,dim_type dimIndex) {
	if(dimIndex - indx_diff < 0) {
		VR_PRINT_ERR("Dimensions have to be positive");
	}
	return dimIndex <= arr.ndims?arr.dims[dimIndex - indx_diff]:1;
}
dim_type* size(VrArrayPtrCF32 arr) {
	return arr.dims;
}

dim_type size(VrArrayPtrCF32 arr,dim_type dimIndex) {
	if(dimIndex - indx_diff < 0) {
		VR_PRINT_ERR("Dimensions have to be positive");
	}
	return dimIndex <= arr.ndims?arr.dims[dimIndex - indx_diff]:1;
}
