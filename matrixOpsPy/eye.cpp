#include<library_ops.hpp>
#include<eye.tpp>
double eye() {
	return 1;
}
VrArrayPtrF64 eye(dim_type m) {
	return eye<VrArrayPtrF64,double>(m);
} 
