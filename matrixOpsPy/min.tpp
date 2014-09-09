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
