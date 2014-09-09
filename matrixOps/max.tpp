#define __PYTHON__ 
#ifdef __MATLAB__
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
#elif defined __PYTHON__
template<class DataType>
DataType max(dim_type numel, DataType *data) {
	DataType outData;
	DataType maxVal = data[0];
	for(int i = 0; i < numel; i++)	{
		maxVal = maxVal > data[i]? maxVal : data[i]; 
	}
	outData = maxVal;
	return outData;
}

template<>
double complex max(dim_type numel, double complex *data) {
	double complex outData;
	double complex  maxVal = data[0];
	for(int i = 0; i < numel; i++)	{
			maxVal = cabs(maxVal) > cabs(data[i])? maxVal : data[i]; 
	}
	outData= maxVal;
	return outData;
}

template<>
float complex max(dim_type numel, float complex *data){
	float complex outData;
	float complex  maxVal = data[0];
	for(int i = 0; i < numel; i++)	{
		maxVal = cabsf(maxVal) > cabsf(data[i])? maxVal : data[i]; 
	}
	outData = maxVal;
	return outData;
}
#endif
