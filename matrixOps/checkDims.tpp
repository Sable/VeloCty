template<class ArrayType>
inline bool checkDimStartImpl(ArrayType A,int nIndices, long *indices) {
    if(VR_GET_DATA_F64(A) ==NULL) {
        return false;
    }
    for(int i = 0; i< nIndices; i++) {
        if(indices[i] <=0) {
            return false;
        }
    }
    return true;
}

template<class ArrayType>
inline bool checkDimStopImpl(ArrayType A, int nIndices, long *indices) {
    const dim_type indexDiff = 1;
    int ndims = A.ndims;
    dim_type prod = 1;
    dim_type maxIndx = 0;
    bool flag = true;
    int *dims = A.dims;
    int *outDims = dims; 
    dim_type lastDim = outDims[nIndices - 1];
    if(nIndices < ndims) {
        outDims[nIndices - 1] = flatten(outDims,nIndices -1, ndims); 
    }
    for(int i = 0; i < nIndices; i++) {
        if( i > ndims) {
            if(indices[i] > 1) {
                flag = false;
                break;
            }
        } else {
            if(indices[i] - indexDiff >= outDims[i]) {
                flag= false;
                break;
            }
        }
    }
    dims[nIndices - 1] = lastDim;
    return flag;
}

template<class ArrayType> 
inline bool checkDimStart_spec(ArrayType A, long row, long col) {
    long indices[2];
    indices[0] = row;
    indices[1] = col;
    return checkDimStartImpl(A,2, indices);
}

template<class ArrayType> 
inline bool checkDimStart_spec(ArrayType A, long row) {
    long indices[1];
    indices[0] = row;
    return checkDimStartImpl(A,1, indices);
}

template<class ArrayType> 
inline bool checkDimStart_spec(ArrayType A, long row, long col,long indx_3) {
    long indices[3];
    indices[0] = row;
    indices[1] = col;
    indices[2] = indx_3;
    return checkDimStartImpl(A,3, indices);
}

template<class ArrayType> 
inline bool checkDimStop_spec(ArrayType A, long row, long col) {
    long indices[2];
    indices[0] = row;
    indices[1] = col;
    return checkDimStopImpl(A,2, indices);
}

template<class ArrayType> 
inline bool checkDimStop_spec(ArrayType A, long row) {
    long indices[1];
    indices[0] = row;
    return checkDimStopImpl(A,1, indices);
}

template<class ArrayType> 
inline bool checkDimStop_spec(ArrayType A, long row, long col,long indx_3) {
    long indices[3];
    indices[0] = row;
    indices[1] = col;
    indices[2] = indx_3;
    return checkDimStopImpl(A,3, indices);
}
