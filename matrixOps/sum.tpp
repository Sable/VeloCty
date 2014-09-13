template<class DataType,class DimType>
void sum(DataType *data, DataType *outData,DimType collapseIndx, DimType collapsedDims, DimType size, DimType step) {
	for	(int i = 0; i < collapsedDims; i++)	{
		for	(int j = 0; j < size; j++) {
			outData[i] += data[i*size+j*step]; 
		}
		
	}
}
