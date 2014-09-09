#include<vrbinding.hpp>
#include<library_ops.hpp>
template<class DimType>
DimType getRange(VrIndex indx){
    if(indx.m_isArray == true) {
        return getNumElem(indx.arr.dims,indx.arr.ndims);
    } 
    if(indx.m_isRange == true){
        DimType range =  fix<long,long>((indx.m_val.range_val[1]-indx.m_val.range_val[0])/indx.m_val.range_val[2]);
        return range;
    }
    return 1;
}
template<class ArrayType,class DimType> 
void getArrayData(ArrayType inArr,ArrayType outArr, VrIndex currIndx,DimType currOffset, DimType *outOffset,DimType dimIndex) {
    if(currIndx.m_isRange == false && currIndx.m_isArray ==  false){
        long indx = currIndx.m_val.const_val ; 
        if( indx < 0 ) { 
            indx = inArr.dims[dimIndex] + indx;
        }
        outArr.data[*outOffset] = inArr.data[currOffset + indx];
        (*outOffset)++;
        return;	
    }		
    if(currIndx.m_isRange == true) {
        DimType start  = currIndx.m_val.range_val[0];
        if ( start < 0 ) {
            start = inArr.dims[dimIndex] + start;
        }
        DimType stop = currIndx.m_val.range_val[1];
        if ( stop  < 0 ) {
            stop = inArr.dims[dimIndex] + stop;
        }
        DimType step = currIndx.m_val.range_val[2];
        for(DimType i = start ; i < stop; i+=step) {
            outArr.data[(*outOffset)++] = inArr.data[currOffset + i];	
        }
        return;
    }
    if( currIndx.m_isArray == true) {
        VrArrayPtrF64 arr = currIndx.arr;
        DimType numel  =  getNumElem(arr.dims,arr.ndims);
        for( DimType i = 0; i < numel; i++) {
            DimType indx = static_cast<DimType>(arr.data[i]);
            if( indx < 0 ) {
                indx = inArr.dims[dimIndex] + indx;
            }
            outArr.data[(*outOffset)++] = inArr.data[currOffset + indx];	
        }
        return;
    }
}
template<class ArrayType,class DimType> 
void getArrayData(ArrayType inArr, ArrayType outArr, VrIndex indx1, VrIndex indx2, DimType currOffset,DimType *outOffset,DimType dimIndex){
    DimType jmp = getStep<ArrayType, DimType>(inArr,dimIndex);
    if(indx1.m_isRange == false && indx1.m_isArray == false) {
        long indx = indx1.m_val.const_val;
        if(indx < 0) {
            indx = inArr.dims[dimIndex] + indx;
        }
        getArrayData<ArrayType,DimType>(inArr,outArr, indx2,currOffset + indx*jmp, outOffset,dimIndex +1);
    }
    if(indx1.m_isRange == true) {
        printf("here\n");
        DimType start  = indx1.m_val.range_val[0];
        if ( start < 0 ) {
            start = inArr.dims[dimIndex] + start;
        }
        DimType stop = indx1.m_val.range_val[1];
        if ( stop  < 0 ) {
            stop = inArr.dims[dimIndex] + stop;
        }
        DimType step = indx1.m_val.range_val[2];
        for(DimType i = start ; i < stop; i+=step) {
            getArrayData<ArrayType,DimType>(inArr,outArr, indx2,currOffset + i*jmp, outOffset,dimIndex +1);
        }
        return;
    }
    if( indx1.m_isArray == true) {
        VrArrayPtrF64 arr = indx1.arr;
        DimType numel  =  getNumElem(arr.dims,arr.ndims);
        for( DimType i = 0; i < numel; i++) {
            DimType indx = static_cast<DimType>(arr.data[i]);
            if( indx < 0 ) {
                indx = inArr.dims[dimIndex] + indx;
            }
            getArrayData<ArrayType,DimType>(inArr,outArr, indx2,currOffset + indx*jmp, outOffset,dimIndex +1);
        }
        return;
    }
}

template<class ArrayType,class DimType> 
void getArrayData(ArrayType inArr, ArrayType outArr, VrIndex indx1, VrIndex indx2,VrIndex indx3, DimType currOffset,DimType *outOffset,DimType dimIndex){
    DimType jmp = getStep<ArrayType, DimType>(inArr,dimIndex);
    if(indx1.m_isRange == false && indx1.m_isArray == false) {
        long indx = indx1.m_val.const_val;
        if(indx < 0) {
            indx = inArr.dims[dimIndex] + indx;
        }
        getArrayData<ArrayType,DimType>(inArr,outArr, indx2,indx3,currOffset + indx*jmp, outOffset,dimIndex +1);
    }
    if(indx1.m_isRange == true) {
        DimType start  = indx1.m_val.range_val[0];
        if ( start < 0 ) {
            start = inArr.dims[dimIndex] + start;
        }
        DimType stop = indx1.m_val.range_val[1];
        if ( stop  < 0 ) {
            stop = inArr.dims[dimIndex] + stop;
        }
        DimType step = indx1.m_val.range_val[2];
        for(DimType i = start ; i < stop; i+=step) {
            getArrayData<ArrayType,DimType>(inArr,outArr, indx2,indx3,currOffset + i*jmp, outOffset,dimIndex +1);
        }
        return;
    }
    if( indx1.m_isArray == true) {
        VrArrayPtrF64 arr = indx1.arr;
        DimType numel  =  getNumElem(arr.dims,arr.ndims);
        for( DimType i = 0; i < numel; i++) {
            DimType indx = static_cast<DimType>(arr.data[i]);
            if( indx < 0 ) {
                indx = inArr.dims[dimIndex] + indx;
            }
            getArrayData<ArrayType,DimType>(inArr,outArr, indx2,indx3, currOffset + indx*jmp, outOffset,dimIndex +1);
        }
        return;
    }
}

template<class ArrayType,class DimType>
void arraySlice(ArrayType inArr,ArrayType outArr, VrIndex row, VrIndex col) {
    int out = 0;
    getArrayData<ArrayType,DimType>(inArr,outArr,row,col,0,&out,0);
}

template<class ArrayType,class DimType>
void arraySlice(ArrayType inArr,ArrayType outArr, VrIndex row) {
    int out = 0;
    getArrayData<ArrayType,DimType>(inArr,outArr, row,0,&out,0);
}

template<class ArrayType,class DimType>
void arraySlice(ArrayType inArr,ArrayType outArr, VrIndex row,VrIndex col, VrIndex index3) {
    int out = 0;
    getArrayData<ArrayType,DimType>(inArr,outArr, row,col,index3 ,0,&out,0);
}

template<class ArrayType,class DimType>
void  arraySlice(ArrayType inArr, ArrayType outArr, VrIndex *indices, DimType currOffset, DimType *outOffset,DimType maxIndx,DimType currIndx) {
    if(currIndx == maxIndx) {
        if(indices[currIndx].m_isRange == false && indices[currIndx].m_isArray ==  false){
            long indx = indices[currIndx].m_val.const_val ; 
            if( indx < 0 ) { 
                indx = inArr.dims[currIndx] + indx;
            }
            outArr.data[*outOffset] = inArr.data[currOffset + indx];
            (*outOffset)++;
            return;	
        }		
        if(indices[currIndx].m_isRange == true) {
            DimType start  = indices[currIndx].m_val.range_val[0];
            if ( start < 0 ) {
                start = inArr.dims[currIndx] + start;
            }
            DimType stop = indices[currIndx].m_val.range_val[1];
            if ( stop  < 0 ) {
                stop = inArr.dims[currIndx] + stop;
            }
            DimType step = indices[currIndx].m_val.range_val[2];
            for(DimType i = start ; i < stop; i+=step) {
                outArr.data[(*outOffset)++] = inArr.data[currOffset + i];	
            }
            return;
        }
        if( indices[currIndx].m_isArray == true) {
            VrArrayPtrF64 arr = indices[currIndx].arr;
            DimType numel  =  getNumElem(arr.dims,arr.ndims);
            for( DimType i = 0; i < numel; i++) {
                DimType indx = static_cast<DimType>(arr.data[i]);
                if( indx < 0 ) {
                    indx = inArr.dims[currIndx] + indx;
                }
                outArr.data[(*outOffset)++] = inArr.data[currOffset + indx];	
            }
            return;
        }
    }
    DimType jmp = getStep<ArrayType, DimType>(inArr, currIndx);
    if(indices[currIndx].m_isRange) {
        DimType start  = indices[currIndx].m_val.range_val[0];
        if ( start < 0 ) {
            start = inArr.dims[currIndx] + start;
        }
        DimType stop = indices[currIndx].m_val.range_val[1];
        if ( stop < 0 ) {
            stop = inArr.dims[currIndx] + stop ;
        }
        DimType step = indices[currIndx].m_val.range_val[2];
        for(DimType i = start ; i < stop; i+=step) {
            arraySlice(inArr,outArr,indices,currOffset + i*jmp,outOffset,maxIndx,currIndx + 1);
        }
        return;
    } else if (indices[currIndx].m_isArray) {	
        VrArrayPtrF64 arr = indices[currIndx].arr;
        DimType numel  =  getNumElem(arr.dims,arr.ndims);
        for( DimType i = 0; i < numel; i++) {
            DimType indx = static_cast<DimType>(arr.data[i]);
            if( indx < 0 ) {
                indx = inArr.dims[currIndx] + indx;
            }
            arraySlice(inArr,outArr,indices,currOffset + (indx)*jmp,outOffset,maxIndx,currIndx + 1);
        }
        return;
    } else {
        DimType val = indices[currIndx].m_val.const_val;
        if( val < 0 ) { 
            val = inArr.dims[currIndx] + val;
        }
        arraySlice(inArr,outArr,indices,currOffset + val*jmp,outOffset,maxIndx, currIndx + 1);
        return;
    }
}

template<class ArrayType,  class DimType> 
void  arraySliceSet(ArrayType inArr, ArrayType outArr, VrIndex *indices, DimType currOffset, DimType *outOffset,DimType maxIndx, DimType currIndx) {
    if(currIndx == maxIndx) {
        if(indices[currIndx].m_isRange == false && indices[currIndx].m_isArray == false ){
            outArr.data[currOffset + indices[currIndx].m_val.const_val] = inArr.data[*outOffset];
            (*outOffset)++;
            return;	
        }		
        if(indices[currIndx].m_isRange == true) {
            DimType start  = indices[currIndx].m_val.range_val[0];
            if ( start < 0 ) {
                start = inArr.dims[currIndx] + start;
            }
            DimType stop = indices[currIndx].m_val.range_val[1];
            if ( stop < 0 ) {
                stop = inArr.dims[currIndx] + stop ;
            }
            DimType step = indices[currIndx].m_val.range_val[2];
            for(DimType i = start ; i < stop ; i+=step) {
                outArr.data[currOffset + i] = inArr.data[(*outOffset)++];	
            }
            return;
        }
        if(indices[currIndx].m_isArray == true) {
            VrArrayPtrF64 arr = indices[currIndx].arr;
            DimType numel  =  getNumElem(arr.dims,arr.ndims);
            for(DimType i = 0; i < numel; i++) {
                outArr.data[currOffset + static_cast<DimType>(arr.data[i]) ] = inArr.data[(*outOffset)++];
            }
            return;
        }	
    }
    DimType jmp = getStep<ArrayType, DimType>(outArr, currIndx);
    if(indices[currIndx].m_isRange) {
        DimType start  = indices[currIndx].m_val.range_val[0];
        if ( start < 0 ) {
            start = inArr.dims[currIndx] + start;
        }
        DimType stop = indices[currIndx].m_val.range_val[1];
        if ( stop < 0 ) {
            stop = inArr.dims[currIndx] + stop ;
        }
        DimType step = indices[currIndx].m_val.range_val[2];
        for(DimType i = start ; i < stop ; i += step) {
            arraySliceSet(inArr,outArr,indices,currOffset + i * jmp,outOffset,maxIndx,currIndx +1);
        }
        return;
    } else if(indices[currIndx].m_isArray == true) {
        VrArrayPtrF64 arr = indices[currIndx].arr;
        DimType numel  =  getNumElem(arr.dims,arr.ndims);
        for( DimType i = 0; i < numel; i++) {
            arraySliceSet(inArr,outArr,indices,currOffset + (static_cast<DimType>(arr.data[i])) * jmp,outOffset,maxIndx,currIndx + 1);
        }
        return;
    } else {
        DimType val = indices[currIndx].m_val.const_val;
        arraySliceSet(inArr,outArr,indices,currOffset + val * jmp,outOffset,maxIndx, currIndx + 1);
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
		for(DimType i = start; i < stop; i += step) {
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
		DimType val = row.m_val.const_val;
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
