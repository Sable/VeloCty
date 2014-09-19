template<class ArrayType,class DimType> 
ArrayType (*getAllocFunc())(DimType ,DimType,DimType*){
    return NULL;
}
template<>
VrArrayF64 (*getAllocFunc<VrArrayF64,dim_type>())(dim_type, dim_type, dim_type* ){
    return &vrAllocArrayF64RM;
}

template<class ArrayType, class DataType, class DimType>
ArrayType horzcat(int nargs, va_list args ) {
    if (nargs <= 0) {
        VR_PRINT_ERR("Number of arguments cannot be less than or equal to 0. \n Exiting.");
    }
    ArrayType *arr = static_cast<ArrayType*>(VR_MALLOC(sizeof(ArrayType)*nargs));
    // va_list args;
    // va_start(args,nargs);
    for(int i = 0; i < nargs; i++) {
        arr[i] = va_arg(args,ArrayType);
    } 
    // va_end(args);  
    DimType ndims = 0; 
    DimType *dims = NULL;
    for(int i = 0; i < nargs; i++) {
        ArrayType A = arr[i];
        if(ndims != 0 && VR_GET_NDIMS_F64(arr[i]) != ndims && VR_GET_DATA_F64(A) != NULL) {
            if(A.dims[A.ndims] !=1) {
                VR_PRINT_ERR("Number of dimensions do not match ");
            }
        }
        if(VR_GET_DATA_F64(A) == NULL){
            continue;
        }
        if(ndims == 0) {
            ndims = VR_GET_NDIMS_F64(A);
        }
        bool zeroFlag = false;
        for(int j = 0; j < VR_GET_NDIMS_F64(A); j++) {
            if(dims != NULL && j != 1) {
                if(dims[j] != VR_GET_DIMS_F64(A)[j] && VR_GET_DIMS_F64(A)[j] != 0){
                    VR_PRINT_ERR("Dimensions do not match ");
                }
            }
            if(VR_GET_DIMS_F64(A)[j] == 0) {
                zeroFlag = true;
                break;
            }
        }
        if(zeroFlag == false && dims == NULL) {
            dims = static_cast<DimType*>(VR_MALLOC(sizeof(DimType)*ndims));
            memcpy(dims,VR_GET_DIMS_F64(A),ndims*sizeof(DimType));
        }
    }
    int totalCols = 0 ;
    for (int i = 0; i < nargs; i++) {
        ArrayType X = arr[i];
        if(VR_GET_DATA_F64(X) == NULL){
            continue;
        }
        totalCols += VR_GET_DIMS_F64(arr[i])[1];
    }
    DimType *outDims = dims;
    outDims[1] = totalCols;
    ArrayType(*func)(DimType,DimType,DimType*) =getAllocFunc<ArrayType,DimType>();
    ArrayType out = func(ndims, 0, outDims);
    DimType j =0;
    DimType hdims = getNumHigherDims<ArrayType,DimType>(arr[0]);
    for(int k = 0; k < hdims; k++){
        for(int i = 0; i < nargs; i++) {
            ArrayType A = arr[i];
            if( A.data == NULL) {
                continue;
            }
            DimType numel = VR_GET_DIMS_F64(arr[i])[0]*VR_GET_DIMS_F64(arr[i])[1];
            memcpy(VR_GET_DATA_F64(out)+j,VR_GET_DATA_F64(A)+k*numel,sizeof(DataType)*numel);
            j = j + numel;
        }
    }
    VR_FREE(dims);
    return out;
}
