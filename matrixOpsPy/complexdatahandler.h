#ifndef __COMPLEX_DATA_HANDLER__
#define __COMPLEX_DATA_HANDLER__
#include<vrbinding.hpp>
//#include<complex.h>
#include "helper.hpp"
float complex *  mat_getComplexDataCF32(PyArrayObject*);
double complex * mat_getComplexDataCF64(PyArrayObject*);
void mat_setComplexDataCF32(PyArrayObject* arr, VrArrayPtrCF32 B);
void mat_setComplexDataCF64(PyArrayObject* arr, VrArrayPtrCF64 B);
#endif
