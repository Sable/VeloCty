template<class T,class dataType> 
inline  void checkBounds(T* arr,bool onLhs,int nargs,...) {
    VrIndex * indices = static_cast<VrIndex*>(VR_MALLOC(sizeof(VrIndex) * nargs));
    va_list args;
    va_start(args,nargs);
    for(int i = 0; i < nargs; i++) {
        indices[i] = va_arg(args,VrIndex);
    }
    va_end(args);
    if(VR_GET_DATA_F64((*arr)) ==NULL) {
        VR_PRINT_ERR("Index exceeds matrix dimensions"); 		
        exit(0);
    }
    if(nargs > VR_GET_NDIMS_F64((*arr))) {
        VR_PRINT_ERR(" number of dimension ");
    }
    for (int i = 0; i < nargs; i++) {
        if(indices[i].m_isRange){
            continue;
        }
        if(indices[i].m_isArray) {
            int numel = getNumElem(indices[i].arr.dims,indices[i].arr.ndims);
            for( int j = 0; j < numel; j++) {
                if(indices[i].arr.dims[j] >= arr->dims[i]){
                    VR_PRINT_ERR("Index exceeds matrix dimensions"); 		
                    exit(0);
                }
                if(indices[i].arr.dims[j]< 0) {
                    if(abs(indices[i].arr.dims[j] > arr->dims[i])) {
                        VR_PRINT_ERR("Index Exceeds dimensions");
                    } 
                }
            }
        }
        if(indices[i].m_val.const_val >= arr->dims[i]){
            VR_PRINT_ERR("Index Exceeds dimensions");
        }
        if(indices[i].m_val.const_val < 0) {
            if(abs(indices[i].m_val.const_val > arr->dims[i])) {
                VR_PRINT_ERR("Index Exceeds dimensions");
            } 
        }
    }
    VR_FREE(indices);
}

template<class T,class DataType> 
inline void checkBounds_spec(T* arr,bool onLhs,VrIndex row,VrIndex col) {
    if(VR_GET_DATA_F64((*arr)) ==NULL ) {
        VR_PRINT_ERR("Array is not allocated.");
    }
    int nargs = 2;
    VrIndex indices[2];
    indices[0] = row;
    indices[1] = col;
    for( int i = 0; i < nargs; i++) {
        if(indices[i].m_isRange){
            continue;
        }
        if(indices[i].m_isArray) {
            int numel = getNumElem(indices[i].arr.dims,indices[i].arr.ndims);
            for( int j = 0; j < numel; j++) {
                if(indices[i].arr.dims[j] >= arr->dims[i]){
                    VR_PRINT_ERR("Index exceeds matrix dimensions"); 		
                    exit(0);
                }
                if(indices[i].arr.dims[j]< 0) {
                    if(abs(indices[i].arr.dims[j] > arr->dims[i])) {
                        VR_PRINT_ERR("Index Exceeds dimensions");
                    } 
                }
            }
        }
        if(indices[i].m_val.const_val >= arr->dims[i]){
            VR_PRINT_ERR("Index Exceeds dimensions");
        }
        if(indices[i].m_val.const_val < 0) {
            if(abs(indices[i].m_val.const_val > arr->dims[i])) {
                VR_PRINT_ERR("Index Exceeds dimensions");
            } 
        }
    }
    return;
}

template<class T,class DataType> 
inline void checkBounds_spec(T* arr,bool onLhs,dim_type row) {
    if(row >= arr->dims[0]){
        VR_PRINT_ERR("Index Exceeds dimensions");
    }
    if((row < -(arr->dims[0]))){
        VR_PRINT_ERR("Index Exceeds dimensions");
        exit(0);
    }
}

template<class T,class dataType> 
inline void checkBounds_spec(T* arr,bool onLhs,dim_type row,dim_type col) {
    if(row >= arr->dims[0] || col >= arr->dims[1]){
        VR_PRINT_ERR("Index Exceeds dimensions");
    }
    if((row < -(arr->dims[0])) || ( col < -(arr->dims[1]))){
        VR_PRINT_ERR("Index Exceeds dimensions");
    }
    return;
}

template<class T,class dataType> 
inline void checkBounds_spec(T* arr,bool onLhs,dim_type row,dim_type col,dim_type indx_3) {
    if(row >= arr->dims[0] || col >= arr->dims[1] || indx_3 >= arr->dims[2]){
        VR_PRINT_ERR("Index Exceeds dimensions");
        exit(0);
    }  
    if((row < -(arr->dims[0])) || ( col < -(arr->dims[1])) || (indx_3 <-(arr->dims[2]))){
        VR_PRINT_ERR("Index Exceeds dimensions");
        exit(0);
    }
   return;

}

template<class T,class DataType> 
inline void checkBounds_spec(T* arr,bool onLhs,VrIndex row) {
    if(VR_GET_DATA_F64((*arr)) ==NULL ) {
        VR_PRINT_ERR("Array is not allocated.");
    }
    const int nargs = 1;
    VrIndex indices[1];
    indices[0] = row;
    for( int i = 0; i < nargs; i++) {
        if(indices[i].m_isRange){
            continue;
        }
        if(indices[i].m_isArray) {
            int numel = getNumElem(indices[i].arr.dims,indices[i].arr.ndims);
            for( int j = 0; j < numel; j++) {
                if(indices[i].arr.dims[j] >= arr->dims[i]){
                    VR_PRINT_ERR("Index exceeds matrix dimensions"); 		
                    exit(0);
                }
                if(indices[i].arr.dims[j]< 0) {
                    if(abs(indices[i].arr.dims[j] > arr->dims[i])) {
                        VR_PRINT_ERR("Index Exceeds dimensions");
                    } 
                }
            }
        }
        if(indices[i].m_val.const_val >= arr->dims[i]){
            VR_PRINT_ERR("Index Exceeds dimensions");
        }
        if(indices[i].m_val.const_val < 0) {
            if(abs(indices[i].m_val.const_val > arr->dims[i])) {
                VR_PRINT_ERR("Index Exceeds dimensions");
            } 
        }
    }
    return;
}

