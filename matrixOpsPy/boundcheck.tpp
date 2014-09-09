/* inline bool exceedsMaxIndex(dim_type nIndices, VrIndex * indices, dim_type *dims,dim_type ndims){ */
/*     const dim_type indexDiff = vrZeroBasedIndex?0:1; */
/*     dim_type prod = 1; */
/*     dim_type maxIndx = 0; */
/*     bool flag = false; */
/*     int *outDims = dims; */ 
/*     dim_type lastDim = outDims[nIndices - 1]; */
/*     if(nIndices < ndims) { */
/*         outDims[nIndices - 1] = flatten(outDims,nIndices -1, ndims); */ 
/*     } */
/*     for(int i = 0; i < nIndices; i++) { */
/*         if(indices[i].m_isRange) { */
/*             if(outDims[i]  <= indices[i].m_val.range_val[1] - indexDiff) { */
/*                 flag = true; */
/*                 break; */
/*             } else if(indices[i].m_val.range_val[0] - indexDiff  < 0){ */
/*                 flag = true; */
/*                 break; */
/*             } */
/*         } else if (indices[i].m_isArray) { */
/*             dim_type range = getNumElem(indices[i].arr.dims,indices[i].arr.ndims); */
/*             for( int j = 0; j <  range; j++ ){ */
/*                 if( static_cast<dim_type>(indices[i].arr.data[j]) - indexDiff >= outDims[i]) { */
/*                     flag = true; */
/*                     break; */
/*                 } */
/*             } */
/*         } else { */
/*             if(indices[i].m_val.const_val- indexDiff >= outDims[i]) { */
/*                 flag = true; */
/*                 break; */
/*             } else if(indices[i].m_val.const_val - indexDiff < 0){ */
/*                 flag = true; */
/*                 break; */
/*             } */

/*         } */
/*     } */
/*     dims[nIndices - 1] = lastDim; */
/*     return flag; */
/* } */


/* inline bool exceedsMaxIndex(dim_type nIndices, dim_type * indices, dim_type *dims,dim_type ndims){ */
/*  dim_type indexDiff; */
/*   if(vrZeroBasedIndex) { */
/*     indexDiff = 0; */
/*   }else{ */
/*     indexDiff = 1; */
/*   } */
/*   dim_type prod = 1; */
/*   dim_type maxIndx = 0; */
/*     bool flag = false; */
/* 	int *outDims = dims; */ 
/* 	dim_type lastDim = outDims[nIndices - 1]; */
/* 	if(nIndices < ndims) { */
/* 		outDims[nIndices - 1] = flatten(outDims,nIndices -1, ndims); */ 
/* 	} */
/* 	int j =0; */
/* 	for(int i = 0; i < nIndices; i++) { */
/* 			if(outDims[i]  <= indices[i] - indexDiff) { */
/* 				flag = true; */
/* 				break; */
/* 			} */
			
/* 	} */
/*   dims[nIndices - 1] = lastDim; */
/*   return flag; */
/* } */

/* inline bool getMinIndex(dim_type nIndices, VrIndex * indices, dim_type *dims){ */
/*  dim_type indexDiff; */
/*   if(vrZeroBasedIndex) { */
/*     indexDiff = 0; */
/*   }else{ */
/*     indexDiff = 1; */
/*   } */
/*   for(int i = 0; i < nIndices; i++){ */
/*     if(indices[i].m_isRange) { */
/*       //minIndx += *prod; */
/* 	  if( indices[i].m_val.range_val[0]-indexDiff < 0 ){ */
/* 	  	return true; */
/* 	  } */
/*     }else { */
/* 	  if(indices[i].m_val.const_val - indexDiff < 0) { */
        
/* 	  	return true; */
/* 	  } */
/*     } */
/*   } */
/*   return false; */
/* } */

/* //! specialised for index expressions containing only constant expressions and not range expressions. */ 
/* inline bool getMinIndex(dim_type nIndices, dim_type * indices, dim_type *dims){ */
/*  dim_type indexDiff; */
/*   if(vrZeroBasedIndex) { */
/*     indexDiff = 0; */
/*   }else{ */
/*     indexDiff = 1; */
/*   } */
/*   for(int i = 0; i < nIndices; i++){ */
/* 	  if( indices[i]-indexDiff < 0 ){ */
/* 	  	return true; */
/* 	  } */
/*   } */
/*   return false; */
/* } */

/* inline bool checkOneIndices(VrIndex *indices, int start,int ndims) { */
/* 	for(int i = start; i < ndims; i++ ) { */
/* 		if(indices[i].m_isRange) { */
/* 			return false; */
/* 		} */
/* 		if(indices[i].m_val.const_val != 1) { */
/* 			return false; */
/* 		} */
/* 	} */
/* 	return true; */
/* } */
/* //! specialised for index expressions containing only constant expressions and not range expressions. */ 
/* inline bool checkOneIndices(dim_type *indices, int start,int ndims) { */
/* 	for(int i = start; i < ndims; i++ ) { */
/* 		if(indices[i] != 1) { */
/* 			return false; */
/* 		} */
/* 	} */
/* 	return true; */
/* } */

/* template<class T,class dataType> */ 
/* void  growArray(T* arr,dim_type nIndices, VrIndex *indices) { */
/* 	if(VR_GET_DATA_F64((*arr)) == NULL) { */
/* 		dim_type ndims = nIndices == 1?2:nIndices; */
/* 		dim_type *dims = static_cast<dim_type*>(VR_MALLOC(sizeof(dim_type)*ndims)); */ 	
/* 		memset(dims,1,sizeof(dim_type) * ndims); */
/* 		for	(int i = 0; i < nIndices; i++) { */
/* 			if(indices[i].m_isRange) { */
/* 				dims[i] = indices[i].m_val.range_val[1]; */
/* 			}else { */
/* 				dims[i] = indices[i].m_val.const_val; */
/* 			} */
/* 		} */
/* 		if (nIndices == 1) { */
/* 			dims[1] = dims[0]; */	
/* 			dims[0] = 1; */
/* 		} */
/* 		*arr = vrAllocArrayF64RM(ndims,0,dims); */	
/* 		return; */
/* 	} else { */
/* 		dim_type ndims = nIndices >= arr->ndims?nIndices:arr->ndims; */
/* 		dim_type  *dims = static_cast<dim_type*>(VR_MALLOC(sizeof(dim_type) * ndims)); */
/* 		//Specialised condition for row vectors */
/* 		if(nIndices == 1 && ( isVector(arr->dims,arr->ndims) && arr->dims[1] != 1 || isScalar<T>(*arr))) { */
/* 			dims[0] = 1; */
/* 			dim_type currIndex ; */
/* 			if( indices[0].m_isRange ) { */
/* 				currIndex = indices[0].m_val.range_val[1]; */
/* 			} else { */
/* 				currIndex = indices[0].m_val.const_val; */
/* 			} */
/* 			if( currIndex  > arr->dims[1] ) { */
/* 				dims[1] = currIndex; */
/* 			} */	
			
/* 		} else { */
/* 			for(int i = 0; i < ndims; i++) { */
/* 				dims[i] =1; */
/* 			} */
/* 			for	(int i = 0; i < nIndices; i++) { */
/* 				dim_type currDim = arr->dims[i]; */
/* 				dim_type currIndex; */
/* 				if(indices[i].m_isRange) { */
/* 					currIndex = indices[i].m_val.range_val[1]; */
/* 				}else { */
/* 					currIndex = indices[i].m_val.const_val; */
/* 				} */
/* 				if(i == ( nIndices - 1) && nIndices < arr->ndims) { */
/* 					currDim = flatten(arr->dims,i,arr->ndims); */
/* 				} */
/* 				if( i >= arr->ndims) { */
/* 					currDim = 1; */
/* 				} */
/* 				dims[i] = currIndex > currDim ? currIndex:currDim; */
/* 			} */
/* 		} */
/* 		T newArr(ndims,dims); */
		
/* 		dim_type collapseDimSize = getCollapsedDimension<T,dim_type>( *arr, 0); */
/* 		dim_type oldStep = getStep<T,dim_type> (*arr,0); */
/* 		dim_type oldSize = getSize<T,dim_type>(*arr,0); */
/* 		dim_type newStep = getStep<T,dim_type> (newArr,0); */
/* 		dim_type newSize = getSize<T,dim_type>(newArr,0); */
/* 		for (int i = 0; i < collapseDimSize;  i++) { */
/* 			for( int j = 0; j < oldSize; j++) { */ 
/* 				newArr.data[i * newSize + j * newStep] = arr->data[i * oldSize + j * oldStep]; */
/* 			} */
/* 		} */		
/* 		*arr = newArr; */	
/* 		return; */
/* 	} */	
/* } */

template<class T,class dataType> 
inline  void checkBounds(T* arr,bool onLhs,int nargs,...) {
    VrIndex * indices = static_cast<VrIndex*>(VR_MALLOC(sizeof(VrIndex) * nargs));
    va_list args;
    va_start(args,nargs);
    for(int i = 0; i < nargs; i++) {
        indices[i] = va_arg(args,VrIndex);
    }
    va_end(args);
    if(VR_GET_DATA_F64((*arr)) ==NULL) {
        VR_PRINT_ERR("Index exceeds matrix dimensions"); 		
        exit(0);
    }
    if(nargs > VR_GET_NDIMS_F64((*arr))) {
        VR_PRINT_ERR(" number of dimension ");
    }
    for (int i = 0; i < nargs; i++) {
        if(indices[i].m_isRange){
            continue;
        }
        if(indices[i].m_isArray) {
            int numel = getNumElem(indices[i].arr.dims,indices[i].arr.ndims);
            for( int j = 0; j < numel; j++) {
                if(indices[i].arr.dims[j] >= arr->dims[i]){
                    VR_PRINT_ERR("Index exceeds matrix dimensions"); 		
                    exit(0);
                }
                if(indices[i].arr.dims[j]< 0) {
                    if(abs(indices[i].arr.dims[j] > arr->dims[i])) {
                        VR_PRINT_ERR("Index Exceeds dimensions");
                    } 
                }
            }
        }
        if(indices[i].m_val.const_val >= arr->dims[i]){
            VR_PRINT_ERR("Index Exceeds dimensions");
        }
        if(indices[i].m_val.const_val < 0) {
            if(abs(indices[i].m_val.const_val > arr->dims[i])) {
                VR_PRINT_ERR("Index Exceeds dimensions");
            } 
        }
    }
    VR_FREE(indices);
}

template<class T,class DataType> 
inline void checkBounds_spec(T* arr,bool onLhs,VrIndex row,VrIndex col) {
    if(VR_GET_DATA_F64((*arr)) ==NULL ) {
        VR_PRINT_ERR("Array is not allocated. Array Growth is badddd");
    }
    int nargs = 2;
    VrIndex indices[2];
    indices[0] = row;
    indices[1] = col;
    for( int i = 0; i < nargs; i++) {
        if(indices[i].m_isRange){
            continue;
        }
        if(indices[i].m_isArray) {
            int numel = getNumElem(indices[i].arr.dims,indices[i].arr.ndims);
            for( int j = 0; j < numel; j++) {
                if(indices[i].arr.dims[j] >= arr->dims[i]){
                    VR_PRINT_ERR("Index exceeds matrix dimensions"); 		
                    exit(0);
                }
                if(indices[i].arr.dims[j]< 0) {
                    if(abs(indices[i].arr.dims[j] > arr->dims[i])) {
                        VR_PRINT_ERR("Index Exceeds dimensions");
                    } 
                }
            }
        }
        if(indices[i].m_val.const_val >= arr->dims[i]){
            VR_PRINT_ERR("Index Exceeds dimensions");
        }
        if(indices[i].m_val.const_val < 0) {
            if(abs(indices[i].m_val.const_val > arr->dims[i])) {
                VR_PRINT_ERR("Index Exceeds dimensions");
            } 
        }
    }
    return;
}

template<class T,class DataType> 
inline void checkBounds_spec(T* arr,bool onLhs,dim_type row) {
    if(row >= arr->dims[0]){
        VR_PRINT_ERR("Index Exceeds dimensions");
    }
}

template<class T,class dataType> 
inline void checkBounds_spec(T* arr,bool onLhs,dim_type row,dim_type col) {
    if(row >= arr->dims[0] || col >= arr->dims[1]){
        VR_PRINT_ERR("Index Exceeds dimensions");
    }
    return;
}

template<class T,class DataType> 
inline void checkBounds_spec(T* arr,bool onLhs,VrIndex row) {
    if(VR_GET_DATA_F64((*arr)) ==NULL ) {
        VR_PRINT_ERR("Array is not allocated. Array Growth is badddd");
    }
    const int nargs = 1;
    VrIndex indices[1];
    indices[0] = row;
    for( int i = 0; i < nargs; i++) {
        if(indices[i].m_isRange){
            continue;
        }
        if(indices[i].m_isArray) {
            int numel = getNumElem(indices[i].arr.dims,indices[i].arr.ndims);
            for( int j = 0; j < numel; j++) {
                if(indices[i].arr.dims[j] >= arr->dims[i]){
                    VR_PRINT_ERR("Index exceeds matrix dimensions"); 		
                    exit(0);
                }
                if(indices[i].arr.dims[j]< 0) {
                    if(abs(indices[i].arr.dims[j] > arr->dims[i])) {
                        VR_PRINT_ERR("Index Exceeds dimensions");
                    } 
                }
            }
        }
        if(indices[i].m_val.const_val >= arr->dims[i]){
            VR_PRINT_ERR("Index Exceeds dimensions");
        }
        if(indices[i].m_val.const_val < 0) {
            if(abs(indices[i].m_val.const_val > arr->dims[i])) {
                VR_PRINT_ERR("Index Exceeds dimensions");
            } 
        }
    }
    return;
}
