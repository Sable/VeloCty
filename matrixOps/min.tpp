template<class DataType,class DimType>
DataType* min(DataType *data, DimType collapseIndx, DimType collapsedDims, DimType size, DimType step) {
	DataType *outData = (DataType*)VR_MALLOC(sizeof(DataType)*collapsedDims);
	for(int i = 0; i < collapsedDims; i++)	{
		DataType minVal = data[i*size];
		for(int j = 1; j < size; j++) {
			minVal = minVal < data[i*size+j*step]? minVal : data[i * size + j * step]; 
		}
		outData[i] = minVal;
	}
	return outData;
}

template<class DimType>
double complex* min(double complex *data, DimType collapseIndx, DimType collapsedDims, DimType size, DimType step) {
	double complex *outData = (double complex*)VR_MALLOC(sizeof(double complex)*collapsedDims);
	for(int i = 0; i < collapsedDims; i++)	{
		double complex  minVal = data[i*size];
		for(int j = 1; j < size; j++) {
			minVal = cabs(minVal) < cabs(data[i*size+j*step])? minVal : data[i * size + j * step]; 
		}
		outData[i] = minVal;
	}
	return outData;
}

template<class DimType>
float complex* min(float complex *data, DimType collapseIndx, DimType collapsedDims, DimType size, DimType step) {
	float complex *outData = (float complex*)VR_MALLOC(sizeof(float complex)*collapsedDims);
	for(int i = 0; i < collapsedDims; i++)	{
		float complex  minVal = data[i*size];
		for(int j = 1; j < size; j++) {
			minVal = cabsf(minVal) < cabsf(data[i*size+j*step])? minVal : data[i * size + j * step]; 
		}
		outData[i] = minVal;
	}
	return outData;
}
