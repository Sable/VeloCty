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

template<class ArrayType, class DimType> 
void  arraySlice(ArrayType inArr, ArrayType outArr, VrIndex *indices, DimType currOffset, DimType *outoffSet,DimType currIndx) {
    int indx_diff = 1;
    if(currIndx == 0) {
        if(indices[0].m_isRange == false && indices[0].m_isArray ==  false){
            outArr.data[*outoffSet] = inArr.data[currOffset + indices[0].m_val.const_val-indx_diff];
            return;	
        }		
        if(indices[0].m_isRange == true) {
            DimType start  = indices[0].m_val.range_val[0];
            DimType stop = indices[0].m_val.range_val[1];
            DimType step = indices[0].m_val.range_val[2];
            if(step == 1) {
                memcpy(&outArr.data[(*outoffSet)], &inArr.data[currOffset], sizeof(inArr.data[0])* (stop - start+1));
            } else{
                for(DimType i = start -1; i <= stop - 1; i+=step) {
                    outArr.data[(*outoffSet)++] = inArr.data[currOffset + i];	
                }
            }
            return;
        }
        if( indices[0].m_isArray == true) {
            VrArrayPtrF64 arr = indices[0].arr;
            DimType numel  =  getNumElem(arr.dims,arr.ndims);
            for( DimType i = 0; i < numel; i++) {
                outArr.data[(*outoffSet)++] = inArr.data[currOffset + static_cast<DimType>(arr.data[i]) - indx_diff];	
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
			arraySlice(inArr,outArr,indices,currOffset + i * jmp,outoffSet,currIndx -1);
		}
		return;
	} else if(indices[currIndx].m_isArray == true) {
		VrArrayPtrF64 arr = indices[currIndx].arr;
		DimType numel  =  getNumElem(arr.dims,arr.ndims);
		for( DimType i = 0; i < numel; i++) {
			arraySlice(inArr,outArr,indices,currOffset + (static_cast<DimType>(arr.data[i]) - indx_diff) * jmp,outoffSet,currIndx -1);
		}
		return;
	} else {
		DimType val = indices[currIndx].m_val.const_val - indx_diff;
		arraySlice(inArr,outArr,indices,currOffset + val * jmp,outoffSet, currIndx -1);
		return;
	}
}

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
            outArr.data[jmp + static_cast<DimType>(arr.data[i])] = inArr.data[*(currOffset)];
            if(inArr.dims[0] == 1 && inArr.dims[1] == 1 && inArr.ndims == 2) {
                continue;
            }
            (*currOffset)++;
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
           setRowData<ArrayType,DataType,DimType>(inArr, outArr,jmpVal + i*outArr.dims[0], row, currOffSet); 
		}
		return;
	} else {
		DimType val = col.m_val.const_val - indx_diff;
        setRowData<ArrayType,DataType,DimType>(inArr, outArr, jmpVal + val*outArr.dims[0], row, currOffSet); 
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
	DimType jmp = getStep<ArrayType, DimType>(outArr,2);
	if(index_3.m_isRange) {
		DimType start = index_3.m_val.range_val[0];
		DimType stop = index_3.m_val.range_val[1];
		DimType step = index_3.m_val.range_val[2];
		for(DimType i = start -indx_diff; i <= stop - indx_diff; i += step) {
           setColData<ArrayType,DataType,DimType>(inArr, outArr,  i*jmp, row,col,&currOffSet); 
		}
		return;
	} else if(index_3.m_isArray == true) {
		VrArrayPtrF64 arr = index_3.arr;
		DimType numel  =  getNumElem(arr.dims,arr.ndims);
		for( DimType i = 0; i < numel; i++) {
           setColData<ArrayType,DataType,DimType>(inArr, outArr, i*jmp, row,col, &currOffSet); 
		}
		return;
	} else {
		DimType val = index_3.m_val.const_val - indx_diff;
        setColData<ArrayType,DataType,DimType>(inArr, outArr, val*jmp, row,col, &currOffSet); 
		return;
	}
}
