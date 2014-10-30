#ifndef __VR_DATA_H__
#define __VR_DATA_H__
#include "string.h"
#include<complex.h>
#include<cstdlib>
#include<stdio.h>
#include<gc.h>
#define VR_MALLOC(x) GC_MALLOC_ATOMIC(x)
typedef int dim_type;
typedef enum Layout{
    COLUMN_MAJOR,ROW_MAJOR
}Layout;

struct VrIndex;

typedef struct VrArrayF64{
  double *data;
  dim_type* dims;  
  int ndims;
  Layout layout;
  VrArrayF64(){
    data=NULL;
    dims=NULL;
    ndims=0;
  }

  VrArrayF64(VrArrayF64 *);

  VrArrayF64(double* data, int *dims, int ndims ):data(data),dims(dims),ndims(ndims) {
  }

  VrArrayF64(double* data, long *dimensions, int ndims ):data(data),ndims(ndims) {
		 dims = static_cast<int*>(VR_MALLOC(sizeof(int)*ndims));
		for (int  i = 0; i < ndims; i++) {
			dims[i] = static_cast<int>(dimensions[i]);
		}	
  }

  VrArrayF64(double scal) :ndims(2) {
  	data = static_cast<double*>(VR_MALLOC(sizeof(double)));
	*data = scal;
	dims  = static_cast<dim_type*>(VR_MALLOC(sizeof(dim_type)*2));
	dims[0] = 1;
	dims[1] = 1;
  }
  VrArrayF64(dim_type ndims,dim_type *dims);
  dim_type numelSliced(VrIndex*,dim_type);
  dim_type* dimsSliced(VrIndex*, dim_type);
  VrArrayF64 sliceArray(int nargs, ...);
  void setArraySliceSpec(VrArrayF64 inArr,VrIndex row, VrIndex col);
  void setArraySliceSpec(VrArrayF64 inArr,VrIndex row);
  void setArraySliceSpec(VrArrayF64 inArr,VrIndex row, VrIndex col,VrIndex index_3);
  void setArraySlice(VrArrayF64 inArr,int nargs, ...);
  VrArrayF64 sliceArraySpec(VrIndex row,VrIndex col );
  VrArrayF64 sliceArraySpec(VrIndex row);
  VrArrayF64 sliceArraySpec(VrIndex row,VrIndex col,VrIndex index_3);
}VrArrayF64;

typedef struct VrArrayF32{
  float *data;
  dim_type* dims;  
  int ndims;
  VrArrayF32(){
    data=NULL;
    dims=NULL;
    ndims=0;
  }
  VrArrayF32(float* data, int *dim_type, int ndims ):data(data),dims(dims),ndims(ndims){
  }
  dim_type numelSliced(VrIndex*,dim_type);
  dim_type* dimsSliced(VrIndex*, dim_type);
  VrArrayF32 operator()(int nargs, ...);
  VrArrayF32(dim_type ndims, dim_type* dims);
}VrArrayF32;

typedef struct VrArrayI64{
  long *data;
  dim_type* dims;  
  int ndims;
  dim_type numelSliced(VrIndex*,dim_type);
  dim_type* dimsSliced(VrIndex*, dim_type);
  VrArrayI64 operator()(int nargs, ...);
  VrArrayI64(dim_type ndims, dim_type* dims);
  VrArrayI64(){}
}VrArrayI64;

typedef struct VrArrayI32{
  int *data;
  dim_type* dims;  
  int ndims;
  dim_type numelSliced(VrIndex*,dim_type);
  dim_type* dimsSliced(VrIndex*, dim_type);
  VrArrayI32 operator()(int nargs, ...);
  VrArrayI32(dim_type ndims, dim_type* dims);
  VrArrayI32(){}
}VrArrayI32;

typedef struct VrArrayCF64{
  double complex  *data;
  dim_type* dims;  
  int ndims;
  dim_type numelSliced(VrIndex*,dim_type);
  dim_type* dimsSliced(VrIndex*, dim_type);
  VrArrayCF64 operator()(int nargs, ...);
  VrArrayCF64() {
    ndims =0;
    dims = NULL;
    data = NULL;
  }
  VrArrayCF64(dim_type, dim_type*);
}VrArrayCF64;

typedef struct VrArrayCF32{
  float complex *data;
  dim_type* dims;  
  int ndims;
  dim_type numelSliced(VrIndex*,dim_type);
  dim_type* dimsSliced(VrIndex*, dim_type);
  VrArrayCF32() {
    ndims =0;
    dims = NULL;
    data = NULL;
  }
  VrArrayCF32 operator()(int nargs, ...);
  VrArrayCF32(dim_type, dim_type*);
}VrArrayCF32;

typedef struct VrArrayCI64{
  long *data;
  dim_type* dims;  
  int ndims;
  dim_type numelSliced(VrIndex*,dim_type);
  dim_type* dimsSliced(VrIndex*, dim_type);
  VrArrayCI64 operator()(int nargs, ...);
}VrArrayCI64;

typedef struct VrArrayCI32{
  int *data;
  dim_type* dims;  
  int ndims;
  dim_type numelSliced(VrIndex*,dim_type);
  dim_type* dimsSliced(VrIndex*, dim_type);
  VrArrayCI32 operator()(int nargs, ...);
}VrArrayCI32;

typedef struct VrArrayB{
  bool *data;
  dim_type* dims;  
  int ndims;
}VrArrayB;

struct VrIndex{
	bool m_isRange;
	bool m_isArray;
  	VrArrayF64 arr;		
	union Val{
		dim_type const_val;
		dim_type range_val[3];
	}m_val;
	VrIndex(dim_type const_val):m_isRange(false),m_isArray(false){
		m_val.const_val = const_val;
	}
	VrIndex(dim_type start,dim_type stop,dim_type step){
		m_isRange = true;
		m_isArray = false;
		m_val.range_val[0] = start;
		m_val.range_val[1] = stop;
		m_val.range_val[2] = step;	
	}
	
	VrIndex(VrArrayF64 A) {	
			arr = A;
			m_isArray = true;
			m_isRange = false;
	}
	VrIndex() {
	}	
};
#endif
