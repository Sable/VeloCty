template<class DataType>
DataType sum(dim_type numel, DataType *data) {
	DataType outData = 0;
	for	(int i = 0; i < numel; i++)	{
			outData += data[i]; 
	}
	return outData;
}
