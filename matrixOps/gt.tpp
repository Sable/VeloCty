template<class ArrayType>
VrArrayPtrB gt(ArrayType A, ArrayType B) {
    int numelA = getNumElem(A.dims, A.ndims);  
    int numelB = getNumElem(B.dims, B.ndims);  
    VrArrayPtrB out = static_cast<VrArrayPtrB>(VR_MALLOC(sizeof(numelA)*sizeof(bool)));
    out.dims = static_cast<dim_type*>(VR_MALLOC(sizeof(A.ndims) * sizeof(dim_type)));
    out.ndims = A.ndims;
    if(numelA != numelB) {
        VR_PRINT_ERR("number of elements of the array do not match");
    }
    for(int i = 0; i < numelA; i++) {
        out.data[i] = A.data[i] > B.data[i]; 
    }
    return out;
}

template<class ArrayType>
VrArrayPtrB gt(ArrayType A,double B) {
    int numelA = getNumElem(A.dims, A.ndims);  
    VrArrayPtrB out;
    out.data = static_cast<bool*>(VR_MALLOC(numelA*sizeof(bool)));
    out.dims = static_cast<dim_type*>(VR_MALLOC((A.ndims) * sizeof(dim_type)));
    out.ndims = A.ndims;
    memcpy(out.dims,A.dims,sizeof(dim_type)*A.ndims);
    for(int i = 0; i < numelA; i++) {
        if(A.data[i] > B) {
            out.data[i] = true; 
        }
        else {
            out.data[i] = false;
        }
    }
    return out;
}

