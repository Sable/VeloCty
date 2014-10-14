template<class ArrayType>
ArrayType getIndexPtrImpl(ArrayType A, dim_type nIndices, dim_type * indices) {
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

template<class ArrayType>
ArrayType getIndexPtr(ArrayType A, dim_type nIndices, ...) {
    va_list args;     
    dim_type *indices = VR_MALLOC(sizeof(dim_type) * nIndices);
    va_start(args,nIndices);
    for(int i = 0; i < nIndices; i++) {
        indices[i] = va_arg(args,dim_type);
    }
    return getIndexPtrImpl(A, nIndices, indices); 
}

template<class ArrayType>
ArrayType getIndexPtrSpec(ArrayType A, dim_type row) {
    va_list args;     
    dim_type indices[1];
    indices[0] = row; 
    return getIndexPtrImpl(A,1, indices); 
}

template<class ArrayType>
ArrayType getIndexPtrSpec(ArrayType A, dim_type row, dim_type col) {
    va_list args;     
    dim_type indices[2];
    indices[0] = row; 
    indices[1] = col;
    return getIndexPtrImpl(A,2, indices); 
}

template<class ArrayType>
ArrayType getIndexPtrSpec(ArrayType A, dim_type row, dim_type col,dim_type index_3) {
    va_list args;     
    dim_type indices[3];
    indices[0] = row; 
    indices[1] = col;
    indices[2] = index_3
    return getIndexPtrImpl(A,3, indices); 
}
