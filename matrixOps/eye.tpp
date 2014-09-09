template<class ArrayType,class DataType>
ArrayType eye(dim_type m) {
	ArrayType  arr;
	arr.data = static_cast<DataType*>(VR_MALLOC(sizeof(DataType) *m *m));
	memset(arr.data,0,sizeof(DataType)*m*m);
	arr.ndims = 2;
	arr.dims = static_cast<dim_type*>(VR_MALLOC(sizeof(dim_type)*2));	 
	arr.dims[0] = m;
	arr.dims[1] = m;
	for(int i = 0; i < m; i++) {
		arr.data[i*m+i] = 1;	
	}
	return arr;
} 
