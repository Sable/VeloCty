#include<vrbinding.hpp>
#include<library_ops.hpp>
template<class DimType>
DimType getRange(VrIndex indx){
    if(indx.m_isArray == true) {
        return getNumElem(indx.arr.dims,indx.arr.ndims);
    } 
    if(indx.m_isRange == true){
        DimType range =  fix<long,long>((indx.m_val.range_val[1]-indx.m_val.range_val[0])/indx.m_val.range_val[2]) +1;
        return range;
    }
    return 1;
}
#ifdef __MATLAB__
template<class DataType, class DimType>
DataType* getColData(DimType indx3,const VrIndex& rowIndex, const VrIndex& colIndex, DimType *dims, DimType ndims, DataType *data){
    int indx_diff = 0;
    if (!vrZeroBasedIndex) {
        indx_diff=1;
    }
    DimType total_range = getRange<DimType>(colIndex);
    DimType row_range = getRange<DimType>(rowIndex);
    DataType *outArr;
    outArr = static_cast<DataType*>(VR_MALLOC(sizeof(DataType)*total_range*row_range));
    if ( colIndex.m_isRange == false && colIndex.m_isArray == false ) {
        memcpy(outArr, getRowData(indx3*dims[1]+colIndex.m_val.const_val-indx_diff, rowIndex,dims,ndims,data),
                sizeof(DataType)*row_range);
    } else if( colIndex.m_isArray  == true) {
        VrArrayPtrF64 arr = colIndex.arr;
        DimType numel  =  total_range;
        for( DimType i = 0; i < numel; i++) {
            outArr[i] = data[static_cast<dim_type>(arr.data[i])];
        }
    } else {
        DataType *out_data;
        DimType start = colIndex.m_val.range_val[0];
        DimType stop = colIndex.m_val.range_val[1];
        DimType step = colIndex.m_val.range_val[2];
        for( int i = start-indx_diff, j = 0;i <= (stop-indx_diff);i += step, j += row_range){
            out_data = getRowData(indx3*dims[1] + i,rowIndex,dims,ndims,data);
            memcpy(outArr+j,out_data,sizeof(DataType)*row_range);
        }	    
    }
    return outArr;
}

template<class DataType, class DimType>
DataType *getRowData(int col , const VrIndex &  rowIndx,DimType *dims, DimType ndims,DataType *data) {
    DataType *out_data;
    int indexDiff = 0;
    if (!vrZeroBasedIndex) {
           indexDiff=1;
    }
    if (rowIndx.m_isRange == false && rowIndx.m_isArray == false) {
      out_data = (data + ((rowIndx.m_val.const_val-indexDiff) + (col)*dims[0]));
      return out_data;
    } else if(rowIndx.m_isArray ==  true) {
    	int range = getRange<DimType>(rowIndx);
    	out_data = static_cast<DataType*>(VR_MALLOC(sizeof(DataType)*range));
        for( int i = 0; i < range; i++) {
            out_data[i] = data[static_cast<dim_type>(rowIndx.arr.data[i])];
        }
    }
    int start = rowIndx.m_val.range_val[0];
    int stop = rowIndx.m_val.range_val[1];
    int step = rowIndx.m_val.range_val[2];
	if(step == 1){
    	return (data+ (start-indexDiff)+(col)*dims[0]);
	} else {
    	int range = getRange<DimType>(rowIndx);
    	out_data = static_cast<DataType*>(VR_MALLOC(sizeof(DataType)*range));
		for(int i = start - indexDiff,j=0; i <= stop - indexDiff; i += step,j++) {
			out_data[j] = data[(i-indexDiff) +col*dims[0]];
		}
	}
	return out_data;
}

template<class ArrayType,class DataType, class DimType>
ArrayType arraySlice(ArrayType arr,const VrIndex &indx) {
    int indxDiff = 0;
    if(!vrZeroBasedIndex) {
        indxDiff=1;
    }
    ArrayType outArr;
    DimType width = getRange<DimType>(indx);
    outArr.data=(DataType*)VR_MALLOC(sizeof(DataType)*width);
    outArr.dims=(DimType*)VR_MALLOC(sizeof(DimType)*2);
    outArr.ndims=2;
    if (!indx.m_isRange && ! indx.m_isArray) {
        outArr.data[0] = arr.data[indx.m_val.const_val-indxDiff]; 
        outArr.dims[0]=1;
        outArr.dims[1]=1;
        return outArr;
    } 
    if(indx.m_isArray) {
        for( int i = 0; i < width; i++) {
            outArr.data[i] = arr.data[static_cast<dim_type>(indx.arr.data[i])];
        }
    }else{
        if (indx.m_val.range_val[2] == 1) {
            memcpy(outArr.data,arr.data+indx.m_val.range_val[0]-indxDiff,sizeof(DataType)*width);
        } else {
            int start = indx.m_val.range_val[0];
            int stop = indx.m_val.range_val[1];
            int step = indx.m_val.range_val[2];
            for(int i=start-indxDiff, j = 0 ; i < stop-indxDiff; i += step,j++) {
                outArr.data[j] = arr.data[i - indxDiff];
            }
        }
    }
    if(arr.dims[0] == 1){
        outArr.dims[0] = 1;
        outArr.dims[1] = width;
    } else  {
        outArr.dims[0] = width;
        outArr.dims[1] = 1;
    }


    return outArr;
}

template<class ArrayType,class DataType, class DimType>
ArrayType arraySlice(ArrayType arr, const VrIndex & rowIndex, const VrIndex &  colIndex) {
    int indx_diff = 0;
	if (!vrZeroBasedIndex) {
		indx_diff=1;
	}
      ArrayType outArr;
      DimType total_range = getRange<DimType>(colIndex);
      DimType row_range = getRange<DimType>(rowIndex);
      outArr.dims = static_cast<DimType*>(VR_MALLOC(sizeof(DimType)*2));
      outArr.dims[0] = getRange<DimType>(rowIndex);
      outArr.dims[1] = getRange<DimType>(colIndex);
      outArr.data = getColData(0,rowIndex, colIndex,arr.dims,arr.ndims,arr.data);
      outArr.ndims = 2;     
	return outArr;
}

template<class ArrayType,class DataType, class DimType>
ArrayType arraySlice(ArrayType arr, const VrIndex & rowIndex, const VrIndex &  colIndex, const VrIndex & index_3) {
    int indx_diff = 0;
	if (!vrZeroBasedIndex) {
		indx_diff=1;
	}
      ArrayType outArr;
	  DimType index3_range = getRange<DimType>(index_3);
      DimType col_range = getRange<DimType>(colIndex);
      DimType row_range = getRange<DimType>(rowIndex);
      outArr.dims = static_cast<DimType*>(VR_MALLOC(sizeof(DimType)*3));
      outArr.dims[0] = row_range; 
      outArr.dims[1] = col_range ;
	  outArr.dims[2] = index3_range;
      outArr.data = static_cast<DataType*>(VR_MALLOC(sizeof(DataType)*col_range*row_range*index3_range));
      outArr.ndims = 3;  
    if ( index_3.m_isRange == false ) {
      memcpy(outArr.data, getColData(index_3.m_val.const_val - indx_diff,rowIndex, colIndex,arr.dims,arr.ndims,arr.data),
             sizeof(DataType)*row_range*col_range);
    } else {
		DataType *out_data;
		DimType start = index_3.m_val.range_val[0];
		DimType stop = index_3.m_val.range_val[1];
		DimType step = index_3.m_val.range_val[2];
		for( int i = start-indx_diff, j = 0;i <= (stop-indx_diff);i += step, j += row_range*col_range){
			out_data = getColData(i,rowIndex,colIndex,arr.dims,arr.ndims,arr.data);
			memcpy(outArr.data+j,out_data,sizeof(DataType)*row_range*col_range);
      	}	    
    }
    return outArr;
}

template<class ArrayType, class DimType> 
void  arraySlice(ArrayType inArr, ArrayType outArr, VrIndex *indices, DimType currOffset, DimType *outoffSet,DimType currIndx) {
    int indx_diff = 0;
	if (!vrZeroBasedIndex) {
		indx_diff=1;
	}
	if(currIndx == 0) {
		if(indices[0].m_isRange == false && indices[0].m_isArray ==  false){
			outArr.data[*outoffSet] = inArr.data[currOffset + indices[0].m_val.const_val-indx_diff];
			(*outoffSet)++;
			return;	
		}		
		if(indices[currIndx].m_isRange == true) {
			DimType start  = indices[0].m_val.range_val[0];
			DimType stop = indices[0].m_val.range_val[1];
			DimType step = indices[0].m_val.range_val[2];
			//if(step != 1){
			for(DimType i = start -indx_diff; i <= stop - indx_diff; i+=step) {
				outArr.data[(*outoffSet)++] = inArr.data[currOffset + i];	
			}
			//}
			return;
		}
		if( indices[currIndx].m_isArray == true) {
			VrArrayPtrF64 arr = indices[currIndx].arr;
			DimType numel  =  getNumElem(arr.dims,arr.ndims);
			for( DimType i = 0; i < numel; i++) {
				outArr.data[(*outoffSet)++] = inArr.data[currOffset + static_cast<DimType>(arr.data[i]) - indx_diff];	
			}
			return;
		}
	}
	DimType jmp = getStep<ArrayType, DimType>(inArr, currIndx);
	if(indices[currIndx].m_isRange) {
		DimType start  = indices[currIndx].m_val.range_val[0];
		DimType stop = indices[currIndx].m_val.range_val[1];
		DimType step = indices[currIndx].m_val.range_val[2];
		for(DimType i = start -indx_diff; i <= stop - indx_diff; i+=step) {
			arraySlice(inArr,outArr,indices,currOffset + i*jmp,outoffSet,currIndx -1);
		}
		return;
	} else if (indices[currIndx].m_isArray) {	
		VrArrayPtrF64 arr = indices[currIndx].arr;
		DimType numel  =  getNumElem(arr.dims,arr.ndims);
		for( DimType i = 0; i < numel; i++) {
			arraySlice(inArr,outArr,indices,currOffset + (static_cast<DimType>(arr.data[i]) - indx_diff)*jmp,outoffSet,currIndx -1);
		}
		return;
	} else {
		DimType val = indices[currIndx].m_val.const_val - indx_diff;
		arraySlice(inArr,outArr,indices,currOffset + val*jmp,outoffSet, currIndx -1);
		return;
	}
}
#elif defined __PYTHON__ 
void  arraySlice(ArrayType inArr, ArrayType outArr, VrIndex *indices, DimType currOffset, DimType *outoffSet,DimType maxIndx,DimType currIndx) {
    int indx_diff = 0;
	if (!vrZeroBasedIndex) {
		indx_diff=1;
	}
	if(currIndx == maxIndx) {
		if(indices[currIndx].m_isRange == false && indices[currIndx].m_isArray ==  false){
            long indx = indices[currIndx].m_val.const_val - indx_diff; 
            if( indx < 0 ) { 
                indx = inArr.dims[currIndx] + indx;
            }
			outArr.data[*outoffSet] = inArr.data[currOffset + indx];
			(*outoffSet)++;
			return;	
		}		
		if(indices[currIndx].m_isRange == true) {
			DimType start  = indices[0].m_val.range_val[0];
            if ( start - indx_diff < 0 ) {
                start = inArr.dims[0] + start - indx_diff;
            }
			DimType stop = indices[0].m_val.range_val[1];
            if ( stop  - indx_diff< 0 ) {
                stop = inArr.dims[0] + stop - indx_diff;
            }
			DimType step = indices[0].m_val.range_val[2];
            if ( step  - indx_diff< 0 ) {
                step = inArr.dims[0] + step - indx_diff;
            }
			//if(step != 1){
			for(DimType i = start -indx_diff; i <= stop - indx_diff; i+=step) {
				outArr.data[(*outoffSet)++] = inArr.data[currOffset + i];	
			}
			//}
			return;
		}
		if( indices[currIndx].m_isArray == true) {
            printf("here\n");
			VrArrayPtrF64 arr = indices[currIndx].arr;
			DimType numel  =  getNumElem(arr.dims,arr.ndims);
			for( DimType i = 0; i < numel; i++) {
                DimType indx = static_cast<DimType>(arr.data[i]) - indx_diff;
                if( indx < 0 ) {
                    indx = inArr.dims[currIndx] + indx;
                }
				outArr.data[(*outoffSet)++] = inArr.data[currOffset + indx];	
			}
			return;
		}
	}
	DimType jmp = getStep<ArrayType, DimType>(inArr, currIndx);
	if(indices[currIndx].m_isRange) {
		DimType start  = indices[currIndx].m_val.range_val[0];
            if ( start - indx_diff < 0 ) {
                start = inArr.dims[currIndx] + start - indx_diff;
            }
		DimType stop = indices[currIndx].m_val.range_val[1];
            if ( stop - indx_diff < 0 ) {
                stop = inArr.dims[currIndx] + stop - indx_diff;
            }
		DimType step = indices[currIndx].m_val.range_val[2];
            if ( step - indx_diff < 0 ) {
                step = inArr.dims[currIndx] + step - indx_diff;
            }
		for(DimType i = start -indx_diff; i <= stop - indx_diff; i+=step) {
			arraySlice(inArr,outArr,indices,currOffset + i*jmp,outoffSet,maxIndx,currIndx + 1);
		}
		return;
	} else if (indices[currIndx].m_isArray) {	
		VrArrayPtrF64 arr = indices[currIndx].arr;
		DimType numel  =  getNumElem(arr.dims,arr.ndims);
		for( DimType i = 0; i < numel; i++) {
                DimType indx = static_cast<DimType>(arr.data[i]) - indx_diff;
                if( indx < 0 ) {
                    indx = inArr.dims[currIndx] + indx;
                }
			arraySlice(inArr,outArr,indices,currOffset + (indx)*jmp,outoffSet,maxIndx,currIndx + 1);
		}
		return;
	} else {
		DimType val = indices[currIndx].m_val.const_val - indx_diff;
            if( val < 0 ) { 
                val = inArr.dims[currIndx] + val;
            }
		arraySlice(inArr,outArr,indices,currOffset + val*jmp,outoffSet,maxIndx, currIndx + 1);
		return;
	}
}
#endif

template<class ArrayType,  class DimType> 
void  arraySliceSet(ArrayType inArr, ArrayType outArr, VrIndex *indices, DimType currOffset, DimType *outoffSet,DimType currIndx) {
    int indx_diff = 0;
	if (!vrZeroBasedIndex) {
		indx_diff=1;
	}
	if(currIndx == 0) {
		if(indices[0].m_isRange == false && indices[0].m_isArray == false ){
			outArr.data[currOffset + indices[0].m_val.const_val-indx_diff] = inArr.data[*outoffSet];
			(*outoffSet)++;
			return;	
		}		
		if(indices[currIndx].m_isRange == true) {
			DimType start  = indices[0].m_val.range_val[0];
			DimType stop = indices[0].m_val.range_val[1];
			DimType step = indices[0].m_val.range_val[2];
			//if(step != 1){
				for(DimType i = start - indx_diff; i <= stop - indx_diff; i+=step) {
					outArr.data[currOffset + i] = inArr.data[(*outoffSet)++];	
				}
			//}
			return;
		}
		if(indices[currIndx].m_isArray == true) {
			VrArrayPtrF64 arr = indices[currIndx].arr;
			DimType numel  =  getNumElem(arr.dims,arr.ndims);
			for(DimType i = 0; i < numel; i++) {
				outArr.data[currOffset + static_cast<DimType>(arr.data[i]) -  indx_diff  ] = inArr.data[(*outoffSet)++];
			}
			return;
		}	
	}
	DimType jmp = getStep<ArrayType, DimType>(outArr, currIndx);
	if(indices[currIndx].m_isRange) {
		DimType start = indices[currIndx].m_val.range_val[0];
		DimType stop = indices[currIndx].m_val.range_val[1];
		DimType step = indices[currIndx].m_val.range_val[2];
		for(DimType i = start -indx_diff; i <= stop - indx_diff; i += step) {
			arraySliceSet(inArr,outArr,indices,currOffset + i * jmp,outoffSet,currIndx -1);
		}
		return;
	} else if(indices[currIndx].m_isArray == true) {
		VrArrayPtrF64 arr = indices[currIndx].arr;
		DimType numel  =  getNumElem(arr.dims,arr.ndims);
		for( DimType i = 0; i < numel; i++) {
			arraySliceSet(inArr,outArr,indices,currOffset + (static_cast<DimType>(arr.data[i]) - indx_diff) * jmp,outoffSet,currIndx -1);
		}
		return;
	} else {
		DimType val = indices[currIndx].m_val.const_val - indx_diff;
		arraySliceSet(inArr,outArr,indices,currOffset + val * jmp,outoffSet, currIndx -1);
		return;
	}
}

template<class ArrayType,class DimType>
void  arraySliceSet(double inArr, ArrayType outArr, VrIndex *indices, DimType currOffset,DimType currIndx) {
	int indx_diff = 0;
	if (!vrZeroBasedIndex) {
		indx_diff=1;
	}
	if(currIndx == 0) {
		if(indices[0].m_isRange == false &&
		   indices[0].m_isArray == false){
			outArr.data[currOffset + indices[0].m_val.const_val-indx_diff] = inArr;
			return;	
		} else if(indices[currIndx].m_isRange == true) {
			DimType start  = indices[0].m_val.range_val[0];
			DimType stop = indices[0].m_val.range_val[1];
			DimType step = indices[0].m_val.range_val[2];
			//if(step != 1){
				for(DimType i = start - indx_diff; i <= stop - indx_diff; i+=step) {
					outArr.data[currOffset + i] = inArr;
				}
			//}
			return;
		} else if(indices[currIndx].m_isArray == true) {
			VrArrayPtrF64 arr = indices[currIndx].arr;
			DimType numel  =  getNumElem(arr.dims,arr.ndims);
			for( DimType i = 0; i < numel; i++) {
				outArr.data[currOffset + static_cast<DimType>(arr.data[i]) - indx_diff] = inArr;		
			}
			return;
		}
	}
	DimType jmp = getStep<ArrayType, DimType>(outArr, currIndx);
	if(indices[currIndx].m_isRange) {
		DimType start = indices[currIndx].m_val.range_val[0];
		DimType stop = indices[currIndx].m_val.range_val[1];
		DimType step = indices[currIndx].m_val.range_val[2];
		for(DimType i = start -indx_diff; i <= stop - indx_diff; i += step) {
			arraySliceSet(inArr,outArr,indices,currOffset + i * jmp,currIndx -1);
		}
		return;
	} else if(indices[currIndx].m_isArray == true) {
		VrArrayPtrF64 arr = indices[currIndx].arr;
		DimType numel  =  getNumElem(arr.dims,arr.ndims);
		for( DimType i = 0; i < numel; i++) {
			arraySliceSet(inArr,outArr,indices,currOffset + (static_cast<DimType>(arr.data[i]) - indx_diff) * jmp,currIndx -1);
		}
		return;
	} else {
		DimType val = indices[currIndx].m_val.const_val - indx_diff;
		arraySliceSet(inArr,outArr,indices,currOffset + val * jmp, currIndx -1);
		return;
	}
}

template<class ArrayType,class DataType,class DimType>
void setRowData(ArrayType inArr, ArrayType outArr, DimType jmp, VrIndex row,DimType *currOffset) {
	int indx_diff = 0;
	if (!vrZeroBasedIndex) {
		indx_diff=1;
	}
	if(row.m_isRange) {
		DimType start = row.m_val.range_val[0];
		DimType stop = row.m_val.range_val[1];
		DimType step = row.m_val.range_val[2];
		for(DimType i = start -indx_diff; i <= stop - indx_diff; i += step) {
            outArr.data[jmp + i] = inArr.data[(*currOffset)++];
		}
		return;
	} else if(row.m_isArray == true) {
		VrArrayPtrF64 arr = row.arr;
		DimType numel  =  getNumElem(arr.dims,arr.ndims);
		for( DimType i = 0; i < numel; i++) {
            outArr.data[jmp + i] = inArr.data[*(currOffset)++];
		}
		return;
	} else {
		DimType val = row.m_val.const_val - indx_diff;
        outArr.data[jmp + val] = inArr.data[*(currOffset)++]; 
		return;
	}
}

template<class ArrayType,class DataType,class DimType>
void  setColData(ArrayType inArr, ArrayType outArr,dim_type jmpVal, VrIndex row, VrIndex col,dim_type *currOffSet) {
	int indx_diff = 0;
	if (!vrZeroBasedIndex) {
		indx_diff=1;
	}
	if(col.m_isRange) {
		DimType start = col.m_val.range_val[0];
		DimType stop = col.m_val.range_val[1];
		DimType step = col.m_val.range_val[2];
		for(DimType i = start -indx_diff; i <= stop - indx_diff; i += step) {
           setRowData<ArrayType,DataType,DimType>(inArr, outArr, jmpVal + i*outArr.dims[0], row,currOffSet); 
		}
		return;
	} else if(col.m_isArray == true) {
		VrArrayPtrF64 arr = col.arr;
		DimType numel  =  getNumElem(arr.dims,arr.ndims);
		for( DimType i = 0; i < numel; i++) {
           setRowData<ArrayType,DataType,DimType>(inArr, outArr, i*outArr.dims[0], row, currOffSet); 
		}
		return;
	} else {
		DimType val = col.m_val.const_val - indx_diff;
        setRowData<ArrayType,DataType,DimType>(inArr, outArr, val*outArr.dims[0], row, currOffSet); 
		return;
	}
}
template<class ArrayType,class DataType,class DimType>
void  arraySliceSet(ArrayType inArr, ArrayType outArr, VrIndex row, VrIndex col) {
    dim_type offSet =  0;
    setColData<ArrayType,DataType,DimType>(inArr,outArr,0,row,col,&offSet);
}

template<class ArrayType,class DataType,class DimType>
void  arraySliceSet(ArrayType inArr, ArrayType outArr, VrIndex row) {
    dim_type offSet = 0;
    setRowData<ArrayType,DataType,DimType>(inArr,outArr,0,row,&offSet);
}

template<class ArrayType,class DataType,class DimType>
void  arraySliceSet(ArrayType inArr, ArrayType outArr, VrIndex row,VrIndex col,VrIndex index_3) {
    dim_type offSet = 0;
	int indx_diff = 0;
	if (!vrZeroBasedIndex) {
		indx_diff=1;
	}
    dim_type currOffSet = 0;
	if(index_3.m_isRange) {
		DimType start = index_3.m_val.range_val[0];
		DimType stop = index_3.m_val.range_val[1];
		DimType step = index_3.m_val.range_val[2];
		for(DimType i = start -indx_diff; i <= stop - indx_diff; i += step) {
           setColData<ArrayType,DataType,DimType>(inArr, outArr,  i*outArr.dims[0]*outArr.dims[1], row,col,&currOffSet); 
		}
		return;
	} else if(index_3.m_isArray == true) {
		VrArrayPtrF64 arr = index_3.arr;
		DimType numel  =  getNumElem(arr.dims,arr.ndims);
		for( DimType i = 0; i < numel; i++) {
           setColData<ArrayType,DataType,DimType>(inArr, outArr, i*outArr.dims[0]*outArr.dims[1], row,col, &currOffSet); 
		}
		return;
	} else {
		DimType val = index_3.m_val.const_val - indx_diff;
        setColData<ArrayType,DataType,DimType>(inArr, outArr, val*outArr.dims[0]*outArr.dims[1], row,col, &currOffSet); 
		return;
	}
}
