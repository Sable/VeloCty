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
