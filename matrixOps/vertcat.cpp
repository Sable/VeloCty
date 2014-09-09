#include<library_ops.hpp>
#include<vertcat.tpp>
/*VrArrayPtrF64 vertcat(int nargs, ...) {
  if (nargs <= 0) {
    std::cout<<"Number of arguments cannot be less than or equal to 0. \n Exiting.  "<<std::endl;
    exit(0);
  }
  VrArrayPtrF64 *arr = static_cast<VrArrayPtrF64*>(VR_MALLOC(sizeof(VrArrayPtrF64)*nargs));
  va_list args;
  va_start(args,nargs);
  for(int i = 0; i < nargs; i++) {
    arr[i] = va_arg(args,VrArrayPtrF64);
  } 
  va_end(args);
  dim_type ndims = 0; 
  dim_type *dims = NULL;
  for(int i = 0; i < nargs; i++) {
    VrArrayPtrF64 A = arr[i];
    if(ndims != 0 && VR_GET_NDIMS_F64(arr[i]) != ndims && VR_GET_DATA_F64(A) != NULL) {
      std::cout<<"Number of dimensions do not match "<<std::endl;
      exit(0);
    }
    if(VR_GET_DATA_F64(A) == NULL){
      continue;
    }
    if(ndims == 0) {
      ndims = VR_GET_NDIMS_F64(A);
    }
    bool zeroFlag = false;
    for(int j = 0; j < VR_GET_NDIMS_F64(A); j++) {
      if(dims != NULL && j > 0) {
	if(dims[j] != VR_GET_DIMS_F64(A)[j] && VR_GET_DIMS_F64(A)[j] != 0){
	  std::cout<<"Dimensions do not match "<<std::endl;
	  exit(0);
	}
      }
      if(VR_GET_DIMS_F64(A)[j] == 0) {
	zeroFlag = true;
	break;
      }
    }
    if(zeroFlag == false && dims == NULL) {
      dims = static_cast<dim_type*>(VR_MALLOC(sizeof(dim_type)*ndims));
      memcpy(dims,VR_GET_DIMS_F64(A),ndims*sizeof(dim_type));
    }
  }
  int totalRows = 0 ;
  for (int i = 0; i < nargs; i++) {
    VrArrayPtrF64 X = arr[i];
    if(VR_GET_DATA_F64(arr[i]) == NULL){
      continue;
    }
    totalRows += VR_GET_DIMS_F64(arr[i])[0];
  }
  dim_type *outDims = dims;
  outDims[0] = totalRows;
  VrArrayPtrF64 outArr = vrAllocArrayF64RM(ndims, 0, outDims);
  int ncols = outDims[1];
  int k =0;
  dim_type hDims = getNumHigherDims<VrArrayF64,dim_type>(arr[0]);
  for(int m = 0; m < hDims; m++ ) {
  	for(int i = 0; i < ncols; i++) {
    	for(int j = 0; j < nargs; j++) {
			if(VR_GET_DATA_F64(arr[j]) == NULL) {
	  			continue;
			}
			dim_type numel = VR_GET_DIMS_F64(arr[j])[0] * VR_GET_DIMS_F64(arr[j])[1];
        	memcpy(VR_GET_DATA_F64(outArr) + k, (VR_GET_DATA_F64(arr[j]))+(m*numel)+ i * VR_GET_DIMS_F64(arr[j])[0], sizeof(double) * VR_GET_DIMS_F64(arr[j])[0]);
        	k += VR_GET_DIMS_F64(arr[j])[0];
    	}
  	}
  }
  return outArr;
}
*/
template <class ArrayType> 
ArrayType vertcat(int nargs, ...){
}
template<>
VrArrayPtrF64 vertcat<VrArrayF64> (int nargs, ... ) {
va_list args;
va_start(args,nargs);
VrArrayPtrF64 arr =  vertcat<VrArrayF64,double,dim_type>(nargs,args);
va_end(args);
return arr;
}

template<>
VrArrayPtrF32 vertcat <VrArrayF32>(int nargs, ... ) {
va_list args;
va_start(args,nargs);
VrArrayPtrF32 arr =  vertcat<VrArrayF32,double,dim_type>(nargs,args);
va_end(args);
return arr;
}

template<>
VrArrayPtrCF32 vertcat <VrArrayCF32>(int nargs, ... ) {
va_list args;
va_start(args,nargs);
VrArrayPtrCF32 arr =  vertcat<VrArrayCF32,double,dim_type>(nargs,args);
va_end(args);
return arr;
}

template<>
VrArrayPtrCF64 vertcat <VrArrayCF64>(int nargs, ... ) {
va_list args;
va_start(args,nargs);
VrArrayPtrCF64 arr =  vertcat<VrArrayCF64,double,dim_type>(nargs,args);
va_end(args);
return arr;
}
