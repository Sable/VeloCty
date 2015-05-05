template<class ArrayType,class ReturnType>
ReturnType mean(ArrayType arr) {
    dim_type numel = getNumElem(arr.dims, arr.ndims);
    ReturnType avgVal = 0;
    for( int i = 0; i < numel; i++ ) {
       avgVal += arr.data[i]; 
    }
    return avgVal/numel;
}
