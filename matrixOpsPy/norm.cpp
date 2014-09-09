#include<cblas.h>
#include<library_ops.hpp>
double norm(VrArrayPtrF64 arr) {
	const dim_type numel  = getNumElem(arr.dims,arr.ndims);
	return cblas_dnrm2(numel,arr.data,1);
}
