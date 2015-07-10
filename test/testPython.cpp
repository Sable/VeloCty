#include<Python.h>
#include <numpy/ndarrayobject.h>
#include<numpy/arrayscalars.h>
#include<helper.hpp>
#include<vrbinding.hpp>
#include<library_ops.hpp>
#include<cblas.h>
// #include<matrix_ops.hpp>

// static PyObject* testCheckDims(PyObject *self, PyObject *args) {
//     PyArrayObject *obj1;
//     PyArg_ParseTuple(args, "O!",&PyArray_Type, &obj1);
//     VrArrayF64 A = getVrArrayF64(obj1);
//     std::cout<<"Start Value"<<checkDimStart_spec<VrArrayF64>(A,-3,4)<<std::endl;
//     Py_INCREF(Py_None);
//     return Py_None;
// }

// static PyObject* testBasic(PyObject *self, PyObject *args) 
// {
//     PyArrayObject *obj1;
// 	printf("Successful \n");
//     PyArg_ParseTuple(args, "O!",&PyArray_Type, &obj1);
//     VrArrayPtrI32 C = getVrArrayI32(obj1);
// 	long *dims = static_cast<long*>(VR_MALLOC(sizeof(long)*C.ndims));
// 	for (int i = 0; i < C.ndims; i++) {
// 		dims[i] = C.dims[i];
//         printf("%ld\n",dims[i]);
// 	}
// 	PyArrayObject* ret = reinterpret_cast<PyArrayObject*>(PyArray_SimpleNewFromData(C.ndims, dims,NPY_INT32,C.data));
// 	return PyArray_Return(ret);	
// }
// static PyObject* testMmult(PyObject* self, PyObject *args) {
// 	PyArrayObject* obj1, *obj2;
// 	if(!PyArg_ParseTuple(args, "O!O!",&PyArray_Type, &obj1,&PyArray_Type, &obj2)){
// 		return NULL;
// 	}
// 	VrArrayF64 A = getVrArrayF64(obj1);
// 	VrArrayF64 B = getVrArrayF64(obj2);
// 	VrArrayF64 C = BlasDouble::mmult(CblasRowMajor,CblasNoTrans,CblasNoTrans,A,B);
// 	long *dims = static_cast<long*>(VR_MALLOC(sizeof(long)*C.ndims));
// 	for (int i = 0; i < C.ndims; i++) {
// 		dims[i] = C.dims[i];
// 	}
// 	PyArrayObject* ret = reinterpret_cast<PyArrayObject*>(PyArray_SimpleNewFromData(C.ndims, dims,NPY_FLOAT64,C.data));
// 	Py_INCREF(ret);
// 	return PyArray_Return(ret);
// }

// static PyObject* testIndexPtr(PyObject* self,PyObject *args) {
//     PyArrayObject *obj1;
//     PyArg_ParseTuple(args, "O!",&PyArray_Type, &obj1);
//     VrArrayF64 A = getVrArrayF64(obj1);
//     printf("%d %d\n",A.ndims, A.dims[1]);
//     VrArrayF64 C = getIndexPtr<VrArrayF64>(A,1,1);
// 	long *dims = static_cast<long*>(VR_MALLOC(sizeof(long)*C.ndims));
// 	for (int i = 0; i < C.ndims; i++) {
// 		dims[i] = C.dims[i];
// 	}
// 	PyArrayObject* ret = reinterpret_cast<PyArrayObject*>(PyArray_SimpleNewFromData(C.ndims, dims,NPY_FLOAT64,C.data));
// 	Py_INCREF(ret);
// 	return PyArray_Return(ret);	
//     // Py_INCREF(Py_None);
//     // return Py_None;
// }
// static PyObject* testMean(PyObject* self,PyObject *args) {
// 	PyArrayObject* obj1;
// 	if(!PyArg_ParseTuple(args, "O!",&PyArray_Type, &obj1)){
// 		return NULL;
// 	}
// 	VrArrayF64 A = getVrArrayF64(obj1);
// 	double val = mean_scalar(A);
// 	return PyFloat_FromDouble(val);
// }

// static PyObject* testMax(PyObject* self,PyObject *args) {
// 	PyArrayObject* obj1;
// 	if(!PyArg_ParseTuple(args, "O!",&PyArray_Type, &obj1)){
// 		return NULL;
// 	}
// 	VrArrayF64 A = getVrArrayF64(obj1);
// 	double val = max_scalar(A);
// 	return PyFloat_FromDouble(val);
// }

// static PyObject* testMin(PyObject* self, PyObject *args) {
// 	PyArrayObject* obj1;
// 	if(!PyArg_ParseTuple(args, "O!",&PyArray_Type, &obj1)){
// 		return NULL;
// 	}
// 	VrArrayF64 A = getVrArrayF64(obj1);
// 	double val = min_scalar(A);
// 	return PyFloat_FromDouble(val);
// }

// static PyObject* testSlice(PyObject* self, PyObject *args) {
// 	PyArrayObject* obj1;
// 	if(!PyArg_ParseTuple(args, "O!", &PyArray_Type, &obj1)){
// 		return NULL;
// 	}
// 	VrArrayF64 A = getVrArrayF64(obj1);
//     VrArrayF64 B = ones_double(1,3);
// 	VrArrayF64 C = A.sliceArraySpec(VrIndex(1,3,1));
// 	long *dims = static_cast<long*>(VR_MALLOC(sizeof(long)*C.ndims));
// 	for (int i = 0; i < C.ndims; i++) {
// 		dims[i] = C.dims[i];
// 	}
// 	PyArrayObject* ret = reinterpret_cast<PyArrayObject*>(PyArray_SimpleNewFromData(C.ndims, dims,NPY_FLOAT64,C.data));
// 	Py_INCREF(ret);
//     return PyArray_Return(ret);
// }

// static PyObject* testSetSlice(PyObject* self, PyObject *args) {
// 	PyArrayObject* obj1;
// 	if(!PyArg_ParseTuple(args, "O!", &PyArray_Type, &obj1)){
// 		return NULL;
// 	}
// 	VrArrayF64 A = getVrArrayF64(obj1);
//     VrArrayF64 B = ones_double(2,4,3);
//     A.setArraySliceSpec(B,VrIndex(1,5,1));
// 	VrArrayF64 C  = A; 
// 	long *dims = static_cast<long*>(VR_MALLOC(sizeof(long)*C.ndims));
// 	for (int i = 0; i < C.ndims; i++) {
// 		dims[i] = C.dims[i];
// 	}
// 	PyArrayObject* ret = reinterpret_cast<PyArrayObject*>(PyArray_SimpleNewFromData(C.ndims, dims,NPY_FLOAT64,C.data));
// 	Py_INCREF(ret);
//     return PyArray_Return(ret);
// }
// static PyObject* testVertCat(PyObject* self, PyObject *args) {
// 	PyArrayObject* obj1, *obj2;
// 	if(!PyArg_ParseTuple(args, "O!O!",&PyArray_Type, &obj1,&PyArray_Type, &obj2)){
// 		return NULL;
// 	}
// 	VrArrayF64 A = getVrArrayF64(obj1);
// 	VrArrayF64 B = getVrArrayF64(obj2);
//     VrArrayF64 C = vertcat<VrArrayF64>(2,A,B);
// 	long *dims = static_cast<long*>(VR_MALLOC(sizeof(long)*C.ndims));
// 	for (int i = 0; i < C.ndims; i++) {
// 		dims[i] = C.dims[i];
// 	}
// 	PyArrayObject* ret = reinterpret_cast<PyArrayObject*>(PyArray_SimpleNewFromData(C.ndims, dims,NPY_FLOAT64,C.data));
// 	Py_INCREF(ret);
// 	return PyArray_Return(ret);
// }

// static PyObject* testHorzcat(PyObject* self, PyObject *args) {
// 	PyArrayObject* obj1, *obj2;
// 	if(!PyArg_ParseTuple(args, "O!O!",&PyArray_Type, &obj1,&PyArray_Type, &obj2)){
// 		return NULL;
// 	}
// 	VrArrayF64 A = getVrArrayF64(obj1);
// 	VrArrayF64 B = getVrArrayF64(obj2);
//     VrArrayF64 C = horzcat<VrArrayF64>(2,A,B);
// 	long *dims = static_cast<long*>(VR_MALLOC(sizeof(long)*C.ndims));
// 	for (int i = 0; i < C.ndims; i++) {
// 		dims[i] = C.dims[i];
// 	}
// 	PyArrayObject* ret = reinterpret_cast<PyArrayObject*>(PyArray_SimpleNewFromData(C.ndims, dims,NPY_FLOAT64,C.data));
// 	Py_INCREF(ret);
// 	return PyArray_Return(ret);
// }

// static PyObject* testZeros(PyObject* self, PyObject *args) {
//     VrArrayF32 C = zeros_float(2,3,3);
// 	long *dims = static_cast<long*>(VR_MALLOC(sizeof(long)*C.ndims));
// 	for (int i = 0; i < C.ndims; i++) {
// 		dims[i] = C.dims[i];
// 	}
// 	PyArrayObject* ret = reinterpret_cast<PyArrayObject*>(PyArray_SimpleNewFromData(C.ndims, dims,NPY_FLOAT32,C.data));
// 	Py_INCREF(ret);
// 	return PyArray_Return(ret);
// }

// static PyObject* testZeros_int(PyObject* self, PyObject *args) {
//     VrArrayI32 C = zeros_int(1,3);
// 	long *dims = static_cast<long*>(VR_MALLOC(sizeof(long)*C.ndims));
// 	for (int i = 0; i < C.ndims; i++) {
// 		dims[i] = C.dims[i];
// 	}
// 	PyArrayObject* ret = reinterpret_cast<PyArrayObject*>(PyArray_SimpleNewFromData(C.ndims, dims,NPY_INT32,C.data));
// 	Py_INCREF(ret);
// 	return PyArray_Return(ret);
// }

// static PyObject* testOnes(PyObject* self, PyObject *args) {
//     VrArrayF64 C = ones_double(1,3);
// 	long *dims = static_cast<long*>(VR_MALLOC(sizeof(long)*C.ndims));
// 	for (int i = 0; i < C.ndims; i++) {
// 		dims[i] = C.dims[i];
// 	}
// 	PyArrayObject* ret = reinterpret_cast<PyArrayObject*>(PyArray_SimpleNewFromData(C.ndims, dims,NPY_FLOAT64,C.data));
// 	Py_INCREF(ret);
// 	return PyArray_Return(ret);
// }

// static PyObject* testIndexVal(PyObject* self, PyObject *args) {
// 	PyArrayObject* obj1;
// 	if(!PyArg_ParseTuple(args, "O!",&PyArray_Type, &obj1)){
// 		return NULL;
// 	}
// 	VrArrayF64 A = getVrArrayF64(obj1);
//     return PyLong_FromLong(getIndexVal_spec<VrArrayPtrF64>(0,A,1,2,0));
    
// }

static PyObject* testSum(PyObject* self, PyObject *args) {
	PyObject* obj1;
	if(!PyArg_ParseTuple(args, "O!",&PyArray_Type, &obj1)){
		return NULL;
	}
    // long dims[]  = {2,2};
    
    // obj1 = PyArray_ZEROS(2, dims,NPY_FLOAT64, 0);
    
    // PyObject *sum = PyArray_Sum((PyArrayObject*)obj1, NPY_MAXDIMS, PyArray_DESCR(obj1)->type_num, NULL);
    // void * data = PyArray_DATA(sum);
    // double * floatData = (double*) data;
    // printf("%f\n",((PyDoubleScalarObject*)sum)->obval);
    // printf("%f\n",*(double*)((PyArrayObject*)sum)->data);
   // PyArray_ScalarAsCtype(sum, (void*)floatData);
    // printf("%f",*floatData);
    double ret = sum_scalar<double>((PyArrayObject*)obj1);
    // printf("%f\n", ret);
    return PyFloat_FromDouble(ret);
    // Py_INCREF(Py_None);
    
	// return PyArray_Return(ret);
    
}

static PyMethodDef testPythonMethods[] = 
{
	// {"testBasic",testBasic,METH_VARARGS,"testing whether everything compiles."},
	// {"testMmult",testMmult,METH_VARARGS,"testing mmult"},
	// {"testSlice",testSlice,METH_VARARGS,"testing Slice"},
	// {"testSetSlice",testSetSlice,METH_VARARGS,"testing Slice Set"},
	// {"testMean", testMean, METH_VARARGS,"Testing mean"},
	// {"testMax", testMax,METH_VARARGS, "Testing max"},
	// {"testMin", testMin,METH_VARARGS, "Testing min"},
	// {"testVertCat", testVertCat,METH_VARARGS, "Testing vertcat"},
	// {"testHorzcat", testHorzcat,METH_VARARGS, "Testing Horzcat"},
	// {"testZeros", testZeros, METH_VARARGS, "Testing Zeros"},
	// {"testZeros_int", testZeros_int, METH_VARARGS, "Testing Zeros"},
	// {"testOnes", testOnes, METH_VARARGS, "Testing Ones"},
	// {"testIndexVal", testIndexVal, METH_VARARGS, "Testing IndexVal"},
	// {"testIndexPtr", testIndexPtr, METH_VARARGS, "Testing IndexPtr"},
	// {"testCheckDims", testCheckDims, METH_VARARGS, "Testing IndexPtr"},
	{"testSum", testSum, METH_VARARGS, "Testing IndexPtr"},
	{NULL,NULL,0,NULL}
};
 static struct PyModuleDef testPythonModule = {
	PyModuleDef_HEAD_INIT,
	"testPythonModule",
	NULL,
	-1,
	testPythonMethods,
};

PyMODINIT_FUNC
PyInit_testPython(void){
	 PyObject* m = PyModule_Create(&testPythonModule);
	import_array();
	return m;
}
