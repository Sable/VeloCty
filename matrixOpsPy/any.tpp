#include<library_ops.hpp>
template<class ArrayType>
bool any(ArrayType arr) {
	if(arr.dims[0] != 1 && arr.dims[1] != 1) {
		VR_PRINT_ERR("non-vectors not supported");
	} 
	dim_type numel = getNumElem(arr.dims,arr.ndims);
	for(int i = 0; i < numel; i++ ) {
		if( arr.data[i] != 0 ) {
			return true;
		}
	}	
	return false;
}
