#ifndef __COMPLEX_DATA_HANDLER__
#define __COMPLEX_DATA_HANDLER__
#include<vrbinding.hpp>
#include "helper.hpp"
float complex *  mat_getComplexDataCF32(mxArray*);
double complex * mat_getComplexDataCF64(mxArray*);
void mat_setComplexDataCF32(mxArray* arr, VrArrayPtrCF32 B);
void mat_setComplexDataCF64(mxArray* arr, VrArrayPtrCF64 B);
#endif
