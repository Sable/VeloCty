template<class ArrayType,class DimType> 
ArrayType (*getAllocFunc_vertcat())(DimType ,DimType,DimType*){
    return NULL;
}
template<>
VrArrayF64 (*getAllocFunc_vertcat<VrArrayF64,dim_type>())(dim_type, dim_type, dim_type* ){
    return &vrAllocArrayF64RM;
}
template<class ArrayType,class DataType, class DimType> 
ArrayType vertcat(int nargs, va_list args) {
    // Check if the number of arguments are positive
    if (nargs <= 0) {
        //TODO : Replace with print_err
        std::cout<<"Number of arguments cannot be less than or equal to 0. \n Exiting.  "<<std::endl;
        exit(0);
    }
    //Put input into an array
    ArrayType *arr = static_cast<ArrayType*>(VR_MALLOC(sizeof(ArrayType)*nargs));
    for(int i = 0; i < nargs; i++) {
        arr[i] = va_arg(args,ArrayType);
    } 
    DimType ndims = 0; 
    DimType *dims = NULL;
    //Determine ndims and dims of output array
    // Done by looping through the array and getting total row and max col size
    for(int i = 0; i < nargs; i++) {
        ArrayType A = arr[i];
        //Error Checking. Is it really needed ??? Might improve performance if conditions are removed 
        if(ndims != 0 && VR_GET_NDIMS_F64(arr[i]) != ndims && VR_GET_DATA_F64(A) != NULL) {
            std::cout<<"Number of dimensions do not match "<<std::endl;
            exit(0);
        }
        if(VR_GET_DATA_F64(A) == NULL){
            continue;
        }
        if(ndims == 0) {
            ndims = VR_GET_NDIMS_F64(A);
        }
        bool zeroFlag = false;
        for(int j = 0; j < VR_GET_NDIMS_F64(A); j++) {
            if(dims != NULL && j > 0) {
                if(dims[j] != VR_GET_DIMS_F64(A)[j] && VR_GET_DIMS_F64(A)[j] != 0){
                    std::cout<<"Dimensions do not match "<<std::endl;
                    exit(0);
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
    int totalRows = 0 ;
    for (int i = 0; i < nargs; i++) {
        ArrayType X = arr[i];
        if(VR_GET_DATA_F64(arr[i]) == NULL){
            continue;
        }
        totalRows += VR_GET_DIMS_F64(arr[i])[0];
    }
    DimType *outDims = dims;
    outDims[0] = totalRows;
    ArrayType(*func)(DimType,DimType,DimType*) = getAllocFunc_vertcat<ArrayType,DimType>();
    ArrayType outArr = func(ndims, 0, outDims);
    int ncols = outDims[1];
    int k =0;
    DimType hDims = getNumHigherDims<ArrayType,DimType>(arr[0]);
    for(int m = 0; m < hDims; m++ ) {
        for(int i = 0; i < ncols; i++) {
            for(int j = 0; j < nargs; j++) {
                if(VR_GET_DATA_F64(arr[j]) == NULL) {
                    continue;
                }
                DimType numel = VR_GET_DIMS_F64(arr[j])[0] * VR_GET_DIMS_F64(arr[j])[1];
                memcpy(VR_GET_DATA_F64(outArr) + k, (VR_GET_DATA_F64(arr[j]))+(m*numel)+ i * VR_GET_DIMS_F64(arr[j])[0], sizeof(DataType) * VR_GET_DIMS_F64(arr[j])[0]);
                k += VR_GET_DIMS_F64(arr[j])[0];
            }
        }
    }
    return outArr; 
}
