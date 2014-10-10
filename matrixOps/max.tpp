template<class ArrayType,class ReturnType>
ReturnType max(ArrayType arr) {
    dim_type numel = getNumElem(arr.dims,arr.ndims);
    ReturnType maxVal = arr.data[0];
    for(dim_type i = 1; i < numel; i++ ){
       if(maxVal < arr.data[i]) {
            maxVal = arr.data[i];
        } 
    }
    return maxVal;
}

template<class DataType,class DimType>
DataType* max(DataType *data, DimType collapseIndx, DimType collapsedDims, DimType size, DimType step) {
	DataType *outData = (DataType*)VR_MALLOC(sizeof(DataType)*collapsedDims);
	for(int i = 0; i < collapsedDims; i++)	{
		DataType maxVal = data[i*size];
		for(int j = 1; j < size; j++) {
			maxVal = maxVal > data[i*size+j*step]? maxVal : data[i * size + j * step]; 
		}
		outData[i] = maxVal;
	}
	return outData;
}

template<class DimType>
double complex* max(double complex *data, DimType collapseIndx, DimType collapsedDims, DimType size, DimType step) {
	double complex *outData = (double complex*)VR_MALLOC(sizeof(double complex)*collapsedDims);
	for(int i = 0; i < collapsedDims; i++)	{
		double complex  maxVal = data[i*size];
		for(int j = 1; j < size; j++) {
            if(cabs(maxVal) != cabs(data[i*size+j*step])){
                maxVal = cabs(maxVal) > cabs(data[i*size+j*step])? maxVal : data[i * size + j * step]; 
            } else{
                printf("Max for complex may be wrong");  
            }
		}
		outData[i] = maxVal;
	}
	return outData;
}

template<class DimType>
float complex* max(float complex *data, DimType collapseIndx, DimType collapsedDims, DimType size, DimType step) {
	float complex *outData = (float complex*)VR_MALLOC(sizeof(float complex)*collapsedDims);
	for(int i = 0; i < collapsedDims; i++)	{
		float complex  maxVal = data[i*size];
		for(int j = 1; j < size; j++) {
			maxVal = cabsf(maxVal) > cabsf(data[i*size+j*step])? maxVal : data[i * size + j * step]; 
		}
		outData[i] = maxVal;
	}
	return outData;
}
