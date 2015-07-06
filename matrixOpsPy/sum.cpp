#include "library_ops.hpp"

double sum_scalar(VrArrayPtrF64 A) {
    return *(PyArray_DATA(PyArray_Sum(A,0,PyArray_Type(A),NULL));
} 

float sum_scalar(VrArrayPtrF32 A) {
    return *(PyArray_DATA(PyArray_Sum(A,0,PyArray_Type(A),NULL));
} 

double complex sum_scalar(VrArrayPtrCF64 A) {
    return *(PyArray_DATA(PyArray_Sum(A,0,PyArray_Type(A),NULL));
} 

float complex sum_scalar(VrArrayPtrCF32 A) {
    return *(PyArray_DATA(PyArray_Sum(A,0,PyArray_Type(A),NULL));
} 
