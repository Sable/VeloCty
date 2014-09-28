#ifndef __HELPER_H__
#define __HELPER_H__
#include "vrbinding.hpp"
int getNumElem(int * dims, int ndims);
VrArrayPtrF64 getVrArrayF64(mxArray*);
VrArrayPtrI64 getVrArrayI64(mxArray*);
VrArrayPtrF32 getVrArrayF32(mxArray*);
VrArrayPtrB getVrArrayB(mxArray*);
VrArrayPtrCF64 getVrArrayCF64(mxArray * arr);
VrArrayPtrCF32 getVrArrayCF32(mxArray * arr);
bool checkNdims(int ndims_a, int ndims_b);
bool checkdim(int dim_a, int dim_b);
template<typename T> //Template should be a structure with ndims and dims field.
bool checkdims(T a, T b) {
  if (!checkNdims(VR_GET_NDIMS_F64(a), VR_GET_NDIMS_F64(b))) 
  {	
		if(a.ndims < b.ndims) {
			for(int i = a.ndims; i < b.ndims; i++){
				if(b.dims[i] != 1) return false;		
			}
		}
		if(a.ndims > b.ndims) {
			for(int i = b.ndims; i < a.ndims; i++){
				if(a.dims[i] != 1) return false;		
			}
		}
  }
  for (int i=0;i<VR_GET_NDIMS_F64(a);i++) {
    if(!checkdim(VR_GET_DIMS_F64(a)[i],VR_GET_DIMS_F64(b)[i])) return false;    
  }
  return true;
}

template<typename T> 
bool isScalar(T A) {
  return VR_GET_NDIMS_F64(A)==2&&VR_GET_DIMS_F64(A)[0]==1&&VR_GET_DIMS_F64(A)[1]==1;
}
#endif
