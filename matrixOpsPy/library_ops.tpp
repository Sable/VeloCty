#include<vrbinding.hpp>
#include<omp.h>
template<class T, class U>
T getVrArray(U data) {
  T arr;
  arr.data=(U*)VR_MALLOC(sizeof(U));
  arr.data[0]=data;
  arr.dims = (dim_type*) VR_MALLOC(2*sizeof(dim_type));
  arr.ndims=2;
  arr.dims[1]=1;
  arr.dims[0]=1;
  return arr;
}

template<class T, class U>
U fix(T val) {
	return (U) val < 0 ?-floor(abs(val)):floor(val);
}

template<class T>
std::vector<T> getIterArr(T start,T stop, T step) {
        std::vector<T> loopVec;
        if(step==0 ||step < 0 && start<stop || step>0 && start>stop){
                return loopVec;
        }
        int m= fix<T,T>((stop-start)/step);
        for(int i = 0; i <= m; i++) {
                loopVec.push_back(start+step*i);
        }
        return loopVec;
}

inline dim_type flatten(dim_type * dims, dim_type start, dim_type ndims) {
	int prod = 1;
	for(int i = start; i < ndims; i++) {
		prod *= dims[i];
	}
	return prod;

}
template<class ArrayType,class DimType> 
DimType getStep(ArrayType arr,DimType index) {
	DimType step=1;
	for(int i =index+1; i < arr.ndims; i++) {
		step *= arr.dims[i];
	}
	return step;
}

template<class ArrayType, class DimType>
DimType getSize(ArrayType arr, DimType index) {
	return arr.dims[index];
}

template<class DimType>
DimType getCollapsedDimension(DimType *dims, DimType ndims, DimType index) {
	DimType collapsedDim = 1;
	for(int i =0; i < ndims; i++) {
		if(i != index) {
			collapsedDim *= dims[i];
		}
	}
	return collapsedDim;
}

template<class ArrayType, class DimType>
DimType getCollapsedDimension(ArrayType arr, DimType index) {
	return getCollapsedDimension<DimType>(arr.dims,arr.ndims,index);	
}

template<class ArrayType, class DimType> 
DimType getIndexOfFirstNonSingletonDim( ArrayType arr)	{
	for(int i = 0; i < arr.ndims; i++) {
		if(arr.dims[i]!=1){
			return i;
		}
	}
	return 0;
}  

template<class ArrayType, class DimType>
DimType getNumHigherDims(ArrayType arr){
	DimType ndims=1;
	for(int i = 2; i < arr.ndims; i++) {
		ndims*=arr.dims[i];
	}
	return ndims;
}

