#include<matrix_ops.hpp>
VrArrayPtrI64 BlasLong::scal_mult(int ndims, VrArrayPtrI64 A, long alpha) {
    VrArrayPtrI64 Y = vrAllocArrayI64RM(A.ndims,0,A.dims);
    long N = 1;
    for(int i = 0; i < A.ndims; i++) {
        N *= A.dims[i];
    }
    for(int i = 0; i < N; i++ ){
        Y.data[i] = A.data[i] * alpha;
    }
    return Y;
}
