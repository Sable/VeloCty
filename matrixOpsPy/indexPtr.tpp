template<class ArrayType>
ArrayType getIndexPtr(ArrayType A, dim_type nIndices, dim_type * indices) {
    dim_type offset = 0;
    dim_type multiplier = 1;
    for(int i = nIndices; i < A.ndims; i++) {
        multiplier *= A.dims[i];
    }

    for(int i = nIndices - 1; i >=0; i++) {
        offset +=  indices[i] * multiplier; 
        multiplier *= A.dims[i];
    } 
    dim_type * dims  = VR_MALLOC(sizeof(dim_type) * (A.ndims - nIndices));      
    memcpy(dims,A.dims, sizeof(dim_type) * (A.ndims - nIndices)); 
    return ArrayType(&A.data[offset], dims, A.ndims - nIndices); 
}

