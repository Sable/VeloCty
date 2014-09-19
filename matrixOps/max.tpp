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
			maxVal = cabs(maxVal) > cabs(data[i*size+j*step])? maxVal : data[i * size + j * step]; 
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
