/* #define __PYTHON__ */
#ifdef __MATLAB__
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

#elif defined __PYTHON__ 
template<class DataType>
DataType min(dim_type numel, DataType *data) {
	DataType outData = data[0];
	for(int i  = 1; i < numel; i++) {
		outData = outData < data[i]? outData : data[i]; 
	}
	return outData;
}

template<>
double complex min(dim_type numel, double complex *data) {
	double complex outData = data[0];
	for(int i = 1; i < numel; i++)	{
		outData = cabs(outData) < cabs(data[i])? outData : data[i]; 
	}
	return outData;
}

template<>
float complex min(dim_type numel, float complex *data) {
	float complex outData; 
	for(int i = 0; i < numel; i++)	{
		float complex  minVal = data[i];
		minVal = cabsf(minVal) < cabsf(data[i])? minVal : data[i]; 
		outData = minVal;
	}
	return outData;
}
#endif
