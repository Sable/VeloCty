#include"complexdatahandler.h"
#include<iostream>
float complex  * mat_getComplexDataCF32(mxArray* arr){
    int numel=mxGetNumberOfElements(arr);
    float complex  *out=(float complex *)mxMalloc(sizeof(float complex)*numel);
    float* real= (float*)mxGetData(arr);
    float *imag=(float*)mxGetImagData(arr);
#ifdef DEBUG    
	std::cout<<"in get complex data"<<std::endl;	
#endif
    for(int i=0;i<numel;i++){
     // out[2*i]=real[i];
      //out[2*i+1]=imag[i];
	out[i]=real[i]+imag[i]*I;
#ifdef DEBUG
      std::cout<<real[i]<<" "<<imag[i]<<std::endl;	
#endif
    }
    return out;
  
}
double complex * mat_getComplexDataCF64(mxArray* arr){

  int numel=mxGetNumberOfElements(arr);
  double complex  *out=(double complex *)mxMalloc(sizeof(double complex )*numel);
    double* real= (double*)mxGetData(arr);
    double *imag=(double*)mxGetImagData(arr);
#ifdef DEBUG
      std::cout<<"new matrix "<<std::endl;	
#endif
    for(int i=0;i<numel;i++){
	out[i]=real[i]+imag[i]*I;
#ifdef DEBUG
      std::cout<<out[2*i]<<" "<<out[2*i+1]<<std::endl;	
#endif
    }
    return out;

}
void mat_setComplexDataCF64(mxArray* arr, VrArrayPtrCF64 B){

  double * real=mxGetPr(arr);
  double * imag=mxGetPi(arr);
	for(int i=0;i<getNumElem(VR_GET_DIMS_CF64(B),VR_GET_NDIMS_F64(B));i++){
		real[i]=(double)creal(VR_GET_DATA_CF64(B)[i]);	
		imag[i]=(double)cimag(VR_GET_DATA_CF64(B)[i]);
	}
}


void mat_setComplexDataCF32(mxArray* arr, VrArrayPtrCF32 B){

  float * real=(float*)mxGetData(arr);
  float * imag=(float*)mxGetImagData(arr);
	for(int i=0;i<getNumElem(VR_GET_DIMS_CF32(B),VR_GET_NDIMS_CF32(B));i++){
		real[i]=creal(VR_GET_DATA_CF32(B)[i]);	
		imag[i]=cimag(VR_GET_DATA_CF32(B)[i]);
	}
}
