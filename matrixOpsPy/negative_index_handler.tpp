template<class ArrayType>
inline long getIndexVal(ArrayType arr, int nIndices, ...) {
    VrIndex *indices = static_cast<VrIndex*>(VR_MALLOC(sizeof(VrIndex) * nIndices));
    va_list args;
    va_start(args,nIndices);
    for(int i = 0; i < nIndices; i++) {
        indices[i] = va_arg(args, VrIndex); 
    }  
    long multiplier = 1;
    long finalIndex = 0;
    for( int i = nIndices - 1; i >= 0; i-- ) {
        if (!indices[i].m_isRange && !indices[i].m_isArray) {
            if(indices[i].m_val.const_val < 0) {
                finalIndex += (arr.dims[i] + indices[i].m_val.const_val) * multiplier;
            } else {
                finalIndex += indices[i].m_val.const_val * multiplier;
            }
            multiplier *= arr.dims[i]; 
        } else {
            std::cout<<"Not currently supported."<<std::endl;
            exit(0);
        }
    } 
    return finalIndex;
}

template<class ArrayType>
inline long getIndexVal_spec(int layout=0,ArrayType arr, long row,  long col) {
    if( row < 0) {
      row = arr.dims[0] - row;  
    }

    if(col < 0) {
      col = arr.dims[1] - col;
    }
    // Row Major layout
    if(layout == 0) {
        return row * arr.dims[1] + col;
    //Column Major layout
    } else{
        return row  + col* arr.dims[0];
    }
}

template<class ArrayType>
long getIndexVal_spec(ArrayType arr, long row) {
    if( row < 0) {
      row = arr.dims[0] - row;  
    }
    return row;
}

template<class ArrayType>
long getIndexVal_spec(int layout=0, ArrayType arr, long row, long col, long index_3) {
    if( row < 0) {
      row = arr.dims[0] - row;  
    }
    if( col < 0) {
      col = arr.dims[0] - col;  
    }
    if( index_3 < 0) {
      index_3 = arr.dims[0] - index_3;  
    }
    // Row major layout
    if(layout == 0) {
        return row *arr.dims[1]* arr.dims[2] + col * arr.dims[1] + index_3;
    } else if(layout == 1) { // Column Major layout
        return index_3 *arr.dims[0]* arr.dims[1] + col * arr.dims[0] + row;
    } else{
        printf("Not currently supported\n Stride major in getindexVal\n");
    }
}
