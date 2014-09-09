template<class DataType,class DimType>
DataType* prod(DataType *data, DimType collapseIndx, DimType collapsedDims, DimType size, DimType step) {
	DataType *outData = (DataType*)VR_MALLOC(sizeof(DataType)*collapsedDims);
	for	(int i = 0; i < collapsedDims; i++)	{
		outData[i] = 1;	
		for	(int j = 0; j < size; j++) {
			outData[i] *= data[i*size+j*step]; 
		}
		
	}
	return outData;
}

