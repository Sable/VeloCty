#include<library_ops.hpp>
#include<stdlib.h>

double randi(double max) {
	int min = 1;	
	int base_random = rand(); /* in [0, RAND_MAX] */
  if (RAND_MAX == base_random) return randi( max);
  /* now guaranteed to be in [0, RAND_MAX) */
  int range       = max - min,
      remainder   = RAND_MAX % range,
      bucket      = RAND_MAX / range;
  /* There are range buckets, plus one smaller interval
     within remainder of RAND_MAX */
  if (base_random < RAND_MAX - remainder) {
    return min + base_random/bucket;
  } else {
    return randi (max);
  }	
}

VrArrayPtrF64 randi(double max,dim_type row,dim_type col) {
	dim_type *dims = static_cast<dim_type*>(VR_MALLOC(sizeof(dim_type) * 2));
	dims[0] = row;
	dims[1] = col;	
	dim_type numel = getNumElem(dims,2);
	VrArrayPtrF64 arr;
	arr.data = static_cast<double*>(VR_MALLOC(sizeof(double) * numel));
	arr.dims = dims;
	arr.ndims = 2;
	for( int i = 0; i < numel; i++ ) {
		arr.data[i] = randi(max);
	}
	return arr;
}
