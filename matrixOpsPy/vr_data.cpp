#include "vr_data.hpp"
#include "vr_data.tpp"
#include "vrbinding.hpp"
#include "library_ops.hpp"
#include <iostream>

VrArrayF64::VrArrayF64(VrArrayF64 * arr) {
	dim_type numel = getNumElem(arr->dims,arr->ndims);
  	data = static_cast<double*>(VR_MALLOC(sizeof(double) * numel)); 
	memcpy(data, arr->data, sizeof(double)*numel);
	dims = static_cast<dim_type*>(VR_MALLOC(sizeof(dim_type)*arr->ndims));
	memcpy(dims,arr->dims,sizeof(dim_type)*arr->ndims);
	ndims = arr->ndims;
}

VrArrayF64::VrArrayF64(dim_type ndims,dim_type *dims) {
	dim_type numel = getNumElem(dims,ndims);
	this->dims = dims;
	this->ndims = ndims;
	data = static_cast<double*>(VR_MALLOC(sizeof(double) * numel));	
	memset(data,0,sizeof(double) * numel);
}
//Slice
VrArrayPtrF64 VrArrayF64::sliceArray(int nargs, ...){
	if(nargs ==0){
		VR_PRINT_ERR("number of arguments cannot be 0");
	}
	VrIndex *indices = static_cast<VrIndex*>(VR_MALLOC(sizeof(VrIndex)*nargs));
	va_list args;
	va_start(args,nargs);
	for ( int i = 0; i < nargs; i++) {
		indices[i] = va_arg(args,VrIndex);
	}
	va_end(args);
	dim_type *dims = dimsSliced(indices,nargs);
 	VrArrayPtrF64 outArr = vrAllocArrayF64RM(nargs,0,dims);
 	VR_FREE(dims);
 	int k=0;
 	arraySlice<VrArrayF64,dim_type>(*(this),outArr,indices,0,&k,nargs -1,0);
 	VR_FREE(indices);
	return outArr;
}
VrArrayF64 VrArrayF64::sliceArraySpec(VrIndex row ,VrIndex col) {
    VrIndex indices[2] ;
    indices[0] = row;
    indices[1] = col;
	dim_type *dims = dimsSliced(indices,2);
 	VrArrayPtrF64 outArr = vrAllocArrayF64RM(2,0,dims);
 	VR_FREE(dims);
     arraySlice<VrArrayF64,dim_type>(*(this),outArr,row,col);
    return outArr;
}
VrArrayF64 VrArrayF64::sliceArraySpec(VrIndex row) {
    VrIndex indices[1];
    indices[0] = row;
	dim_type *dims = dimsSliced(indices,1);
 	VrArrayPtrF64 outArr = vrAllocArrayF64RM(1,0,dims);
 	VR_FREE(dims);
    arraySlice<VrArrayF64,dim_type>(*(this),outArr,row);
    return outArr;
}
VrArrayF64 VrArrayF64::sliceArraySpec(VrIndex row,VrIndex col,VrIndex index_3) {
    VrIndex indices[2];
    indices[0] = row;
    indices[1] = col;
    indices[2] = index_3;
	dim_type *dims = dimsSliced(indices,3);
 	VrArrayPtrF64 outArr = vrAllocArrayF64RM(3,0,dims);
 	VR_FREE(dims);
     arraySlice<VrArrayF64,dim_type>(*(this),outArr, row,col,index_3);
    return outArr;
}
//Set Slice 
void VrArrayF64::setArraySlice(VrArrayPtrF64 inArr,int nargs, ...) {
	if(nargs ==0){
		VR_PRINT_ERR("number of arguments cannot be 0");
	}
	VrIndex *indices = static_cast<VrIndex*>(VR_MALLOC(sizeof(VrIndex)*nargs));
	va_list args;
	va_start(args,nargs);
	for( int i = 0; i < nargs; i++) {
		indices[i] = va_arg(args,VrIndex);
	}	
	va_end(args);
	int k=0;
	arraySliceSet<VrArrayF64,dim_type>(inArr,*(this),indices,0,&k,nargs -1,0);
	VR_FREE(indices);
}

void VrArrayF64::setArraySliceSpec(VrArrayPtrF64 inArr,VrIndex row, VrIndex col) {
	int k=0;
       VrIndex indices[2];
        indices[0] = row;
        indices[1] = col;
	arraySliceSet<VrArrayF64,dim_type>(inArr,*(this),indices,0,&k,1,0);
}
void VrArrayF64::setArraySliceSpec(VrArrayPtrF64 inArr,VrIndex row) {
	int k=0;
       VrIndex indices[1];
        indices[0] = row;
	arraySliceSet<VrArrayF64,dim_type>(inArr,*(this),indices,0,&k,0,0);
}

void VrArrayF64::setArraySliceSpec(VrArrayPtrF64 inArr,VrIndex row,VrIndex col,VrIndex index_3) {
    int k=0;
    VrIndex indices[3];
    indices[0] = row;
    indices[1] = col;
    indices[2] = index_3;
    arraySliceSet<VrArrayF64,dim_type>(inArr,*(this),indices,0,&k,3,0);
}

dim_type VrArrayF64::numelSliced(VrIndex * indices,dim_type nIndices) {
	dim_type numel=1;
	for(int i = 0; i < nIndices; i++) {
		numel *= getRange<dim_type>(indices[i]);	
	}
	return numel;
}

dim_type* VrArrayF64::dimsSliced(VrIndex * indices, dim_type nindices) {
dim_type *dims;
    dims = static_cast<dim_type*>(VR_MALLOC(sizeof(dim_type)*(nindices)));

	for(int i = 0; i < nindices; i++) {
		dims[i] = getRange<dim_type>(indices[i]);
	}
	return dims;
}
