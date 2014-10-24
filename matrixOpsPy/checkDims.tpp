template<class ArrayType>
inline bool checkDimStartImpl(ArrayType A,int nIndices, long *indices) {
    if(VR_GET_DATA_F64(A) ==NULL) {
        return false;
    }
    for(int i = 0; i< nIndices; i++) {
        if(indices[i] <= -(A.dims[i] +1)) {
            return false;
        }
    }
    return true;
}

template<class ArrayType>
inline bool checkDimStopImpl(ArrayType A, int nIndices, long *indices) {
    if(VR_GET_DATA_F64(A) == NULL) {
        return false;
    }
    for(int i = 0; i< nIndices; i++) {
        if(indices[i] >= A.dims[i]) {
            return false;
        }
    }
    return true;
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
