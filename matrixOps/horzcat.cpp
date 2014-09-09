#include "library_ops.hpp"
#include "horzcat.tpp"
template <class ArrayType> 
ArrayType horzcat(int nargs, ...){
}
template<>
VrArrayPtrF64 horzcat<VrArrayF64> (int nargs, ... ) {
va_list args;
va_start(args,nargs);
VrArrayPtrF64 arr =  horzcat<VrArrayF64,double,dim_type>(nargs,args);
va_end(args);
return arr;
}

template<>
VrArrayPtrF32 horzcat <VrArrayF32>(int nargs, ... ) {
va_list args;
va_start(args,nargs);
VrArrayPtrF32 arr =  horzcat<VrArrayF32,double,dim_type>(nargs,args);
va_end(args);
return arr;
}

template<>
VrArrayPtrCF32 horzcat <VrArrayCF32>(int nargs, ... ) {
va_list args;
va_start(args,nargs);
VrArrayPtrCF32 arr =  horzcat<VrArrayCF32,double,dim_type>(nargs,args);
va_end(args);
return arr;
}

template<>
VrArrayPtrCF64 horzcat <VrArrayCF64>(int nargs, ... ) {
va_list args;
va_start(args,nargs);
VrArrayPtrCF64 arr =  horzcat<VrArrayCF64,double,dim_type>(nargs,args);
va_end(args);
return arr;
}
