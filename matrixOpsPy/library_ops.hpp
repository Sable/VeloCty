#ifndef __LIBRARY_OPS_H
#define __LIBRARY_OPS_H
#include<climits>
#include "vrbinding.hpp"
#include "vr_data.hpp"
#include "helper.hpp"
#include "math.h"
#include<vector>
#include<cassert>
#include<stdarg.h>
#include<iostream>
#include<dim-handler.hpp>
#include<library_ops.tpp>
#include<negative_index_handler.tpp>
#include<boundcheck.tpp>
#define mod(x,y) ((long)x%(long)y)
bool isVector(dim_type* dims,dim_type ndims);
bool isScalar(dim_type* dims,dim_type ndims);
VrArrayPtrF64 floor(VrArrayPtrF64 A);
double norm(VrArrayPtrF64 arr);
double randi(double max);
double randn();
VrArrayPtrF64 randn(dim_type);
VrArrayPtrF64 randn(dim_type,dim_type);
double eye();
bool any(VrArrayPtrF64);
VrArrayPtrF64 eye(dim_type m);
VrArrayPtrF64 randi(double max,dim_type,dim_type);
long length(VrArrayPtrF64);
VrArrayPtrF64 zeros_double(int,...);
VrArrayPtrI32 zeros_int(int,...);
VrArrayPtrF64 ones(int,...);
VrArrayPtrF64 abs(VrArrayPtrF64);
VrArrayPtrF32 abs(VrArrayPtrF32);
VrArrayPtrF64 cos(VrArrayPtrF64);
VrArrayPtrF32 cos(VrArrayPtrF32);
VrArrayPtrF64 sin(VrArrayPtrF64);
VrArrayPtrF32 sin(VrArrayPtrF32);
// VrArrayF64* tan(VrArrayF64*);
// VrArrayF32* tan(VrArrayF32*);
// VrArrayF64* tan(VrArrayF64*);
// VrArrayF32* tan(VrArrayF32*);
VrArrayPtrF64 pow(VrArrayPtrF64 vec,double val);
VrArrayPtrF32 pow(VrArrayPtrF32 vec, double  val);
double max(VrArrayPtrF64 );
float max(VrArrayPtrF32 );
double complex max(VrArrayPtrCF64 );
float complex max(VrArrayPtrCF32 );
double  min(VrArrayPtrF64 );
float min(VrArrayPtrF32 );
double complex min(VrArrayPtrCF64 );
float complex min(VrArrayPtrCF32 );
double sum(VrArrayPtrF64 A);
float sum(VrArrayPtrF32 A);
float complex sum(VrArrayPtrCF32 A);
double complex sum(VrArrayPtrCF64 A);
double mean(VrArrayPtrF64 A);
float mean(VrArrayPtrF32 A);
template<class ArrayType>
inline long getIndexVal(ArrayType arr, int nIndices, ...);
template<class ArrayType>
inline long getIndexVal(ArrayType arr, long, long);
template<class ArrayType>
inline long getIndexVal(ArrayType arr,long );
VrArrayPtrF64 prod(VrArrayPtrF64 A);
VrArrayPtrF32 prod(VrArrayPtrF32 A);
VrArrayPtrCF32 prod(VrArrayPtrCF32 A);
VrArrayPtrCF64 prod(VrArrayPtrCF64 A);
double complex i();
double pi();
VrIndex range(int val);
VrIndex range(int start, int stop, int step=1);
VrArrayPtrF64 colon(dim_type start , dim_type stop );
VrArrayPtrF64 colon(dim_type start , dim_type step,dim_type stop );
VrArrayPtrF64 sqrt(VrArrayPtrF64 A);
VrArrayPtrF32 sqrt(VrArrayPtrF32 A);
VrArrayPtrF64 rand(int nargs, ...);
template<class ArrayType>
ArrayType horzcat(int ndims, ...);
template<class ArrayType>
ArrayType vertcat(int ndims, ...);
VrArrayPtrF64 exp(VrArrayPtrF64 A);
VrArrayPtrF32 exp(VrArrayPtrF32 A);
VrArrayPtrCF64 cexp(VrArrayPtrCF64 A);
VrArrayPtrCF32 exp(VrArrayPtrCF32 A);
float complex cexp(float complex scalVal);
dim_type ndims(VrArrayPtrF64 arr);
dim_type ndims(VrArrayPtrF32 arr);
dim_type ndims(VrArrayPtrCF64 arr);
dim_type ndims(VrArrayPtrCF32 arr);
dim_type* size(VrArrayPtrF64 arr) ;
dim_type size(VrArrayPtrF64 arr,dim_type dimIndex);
dim_type* size(VrArrayPtrCF64 arr);
dim_type size(VrArrayPtrCF64 arr,dim_type dimIndex);
dim_type* size(VrArrayPtrF32 arr);
dim_type size(VrArrayPtrF32 arr,dim_type dimIndex);
dim_type* size(VrArrayPtrCF32 arr);
dim_type size(VrArrayPtrCF32 arr,dim_type dimIndex);
//dim_type flatten(dim_type * dims, dim_type start, dim_type ndims);
//bool exceedsMaxIndex(dim_type nIndices, VrIndex * indices, dim_type *dims,dim_type ndims);
//bool checkOneIndices(VrIndex *indices, int start,int ndims);
//dim_type getMinIndex(dim_type nIndices, VrIndex * indices, dim_type *dims);

#endif

