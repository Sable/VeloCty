#include<Python.h>
#include <numpy/ndarrayobject.h>
#include<helper.hpp>
#include<vrbinding.hpp>
#include<library_ops.hpp>
#include<cblas.h>
#include<matrix_ops.hpp>
static PyObject* testBasic(PyObject *self, PyObject *args) 
{
	printf("Successful \n");
	//Py_INCREF(Py_None);
	return PyLong_FromLong(1);	
}
static PyObject* testMmult(PyObject* self, PyObject *args) {
	PyArrayObject* obj1, *obj2;
	if(!PyArg_ParseTuple(args, "O!O!",&PyArray_Type, &obj1,&PyArray_Type, &obj2)){
		return NULL;
	}
	VrArrayF64 A = getVrArrayF64(obj1);
	VrArrayF64 B = getVrArrayF64(obj2);
	VrArrayF64 C = BlasDouble::mmult(CblasRowMajor,CblasNoTrans,CblasNoTrans,A,B);
	long *dims = static_cast<long*>(VR_MALLOC(sizeof(long)*C.ndims));
	for (int i = 0; i < C.ndims; i++) {
		dims[i] = C.dims[i];
	}
	PyArrayObject* ret = reinterpret_cast<PyArrayObject*>(PyArray_SimpleNewFromData(C.ndims, dims,NPY_FLOAT64,C.data));
	Py_INCREF(ret);
	return PyArray_Return(ret);
}

static PyObject* testMean(PyObject* self,PyObject *args) {
	PyArrayObject* obj1;
	if(!PyArg_ParseTuple(args, "O!",&PyArray_Type, &obj1)){
		return NULL;
	}
	VrArrayF64 A = getVrArrayF64(obj1);
	double val = mean(A);
	return PyFloat_FromDouble(val);
}

static PyObject* testMax(PyObject* self,PyObject *args) {
	PyArrayObject* obj1;
	if(!PyArg_ParseTuple(args, "O!",&PyArray_Type, &obj1)){
		return NULL;
	}
	VrArrayF64 A = getVrArrayF64(obj1);
	double val = max(A);
	return PyFloat_FromDouble(val);
}

static PyObject* testMin(PyObject* self, PyObject *args) {
	PyArrayObject* obj1;
	if(!PyArg_ParseTuple(args, "O!",&PyArray_Type, &obj1)){
		return NULL;
	}
	VrArrayF64 A = getVrArrayF64(obj1);
	double val = min(A);
	return PyFloat_FromDouble(val);
}

static PyObject* testSlice(PyObject* self, PyObject *args) {
	PyArrayObject* obj1;
	if(!PyArg_ParseTuple(args, "O!", &PyArray_Type, &obj1)){
		return NULL;
	}
	VrArrayF64 A = getVrArrayF64(obj1);
    VrArrayF64 B = ones(1,3);
	VrArrayF64 C = A.sliceArraySpec(VrIndex(1,3,1));
	long *dims = static_cast<long*>(VR_MALLOC(sizeof(long)*C.ndims));
	for (int i = 0; i < C.ndims; i++) {
		dims[i] = C.dims[i];
	}
	PyArrayObject* ret = reinterpret_cast<PyArrayObject*>(PyArray_SimpleNewFromData(C.ndims, dims,NPY_FLOAT64,C.data));
	Py_INCREF(ret);
    return PyArray_Return(ret);
}

static PyObject* testSetSlice(PyObject* self, PyObject *args) {
	PyArrayObject* obj1;
	if(!PyArg_ParseTuple(args, "O!", &PyArray_Type, &obj1)){
		return NULL;
	}
	VrArrayF64 A = getVrArrayF64(obj1);
    VrArrayF64 B = ones(2,4,3);
    A.setArraySliceSpec(B,VrIndex(1,5,1));
	VrArrayF64 C  = A; 
	long *dims = static_cast<long*>(VR_MALLOC(sizeof(long)*C.ndims));
	for (int i = 0; i < C.ndims; i++) {
		dims[i] = C.dims[i];
	}
	PyArrayObject* ret = reinterpret_cast<PyArrayObject*>(PyArray_SimpleNewFromData(C.ndims, dims,NPY_FLOAT64,C.data));
	Py_INCREF(ret);
    return PyArray_Return(ret);
}
static PyObject* testVertCat(PyObject* self, PyObject *args) {
	PyArrayObject* obj1, *obj2;
	if(!PyArg_ParseTuple(args, "O!O!",&PyArray_Type, &obj1,&PyArray_Type, &obj2)){
		return NULL;
	}
	VrArrayF64 A = getVrArrayF64(obj1);
	VrArrayF64 B = getVrArrayF64(obj2);
    VrArrayF64 C = vertcat<VrArrayF64>(2,A,B);
	long *dims = static_cast<long*>(VR_MALLOC(sizeof(long)*C.ndims));
	for (int i = 0; i < C.ndims; i++) {
		dims[i] = C.dims[i];
	}
	PyArrayObject* ret = reinterpret_cast<PyArrayObject*>(PyArray_SimpleNewFromData(C.ndims, dims,NPY_FLOAT64,C.data));
	Py_INCREF(ret);
	return PyArray_Return(ret);
}

static PyObject* testHorzcat(PyObject* self, PyObject *args) {
	PyArrayObject* obj1, *obj2;
	if(!PyArg_ParseTuple(args, "O!O!",&PyArray_Type, &obj1,&PyArray_Type, &obj2)){
		return NULL;
	}
	VrArrayF64 A = getVrArrayF64(obj1);
	VrArrayF64 B = getVrArrayF64(obj2);
    VrArrayF64 C = horzcat<VrArrayF64>(2,A,B);
	long *dims = static_cast<long*>(VR_MALLOC(sizeof(long)*C.ndims));
	for (int i = 0; i < C.ndims; i++) {
		dims[i] = C.dims[i];
	}
	PyArrayObject* ret = reinterpret_cast<PyArrayObject*>(PyArray_SimpleNewFromData(C.ndims, dims,NPY_FLOAT64,C.data));
	Py_INCREF(ret);
	return PyArray_Return(ret);
}

static PyObject* testZeros(PyObject* self, PyObject *args) {
    VrArrayF64 C = zeros_double(1,3);
	long *dims = static_cast<long*>(VR_MALLOC(sizeof(long)*C.ndims));
	for (int i = 0; i < C.ndims; i++) {
		dims[i] = C.dims[i];
	}
	PyArrayObject* ret = reinterpret_cast<PyArrayObject*>(PyArray_SimpleNewFromData(C.ndims, dims,NPY_FLOAT64,C.data));
	Py_INCREF(ret);
	return PyArray_Return(ret);
}

static PyObject* testZeros_int(PyObject* self, PyObject *args) {
    VrArrayI32 C = zeros_int(1,3);
	long *dims = static_cast<long*>(VR_MALLOC(sizeof(long)*C.ndims));
	for (int i = 0; i < C.ndims; i++) {
		dims[i] = C.dims[i];
	}
	PyArrayObject* ret = reinterpret_cast<PyArrayObject*>(PyArray_SimpleNewFromData(C.ndims, dims,NPY_INT32,C.data));
	Py_INCREF(ret);
	return PyArray_Return(ret);
}

static PyObject* testOnes(PyObject* self, PyObject *args) {
    VrArrayF64 C = ones(1,3);
	long *dims = static_cast<long*>(VR_MALLOC(sizeof(long)*C.ndims));
	for (int i = 0; i < C.ndims; i++) {
		dims[i] = C.dims[i];
	}
	PyArrayObject* ret = reinterpret_cast<PyArrayObject*>(PyArray_SimpleNewFromData(C.ndims, dims,NPY_FLOAT64,C.data));
	Py_INCREF(ret);
	return PyArray_Return(ret);
}

static PyObject* testIndexVal(PyObject* self, PyObject *args) {
	PyArrayObject* obj1;
	if(!PyArg_ParseTuple(args, "O!",&PyArray_Type, &obj1)){
		return NULL;
	}
	VrArrayF64 A = getVrArrayF64(obj1);
    return PyLong_FromLong(getIndexVal<VrArrayPtrF64>(A,2,VrIndex(-1),VrIndex(-2)));
    
}
static PyMethodDef testPythonMethods[] = 
{
	{"testBasic",testBasic,METH_VARARGS,"testing whether everything compiles."},
	{"testMmult",testMmult,METH_VARARGS,"testing mmult"},
	{"testSlice",testSlice,METH_VARARGS,"testing Slice"},
	{"testSetSlice",testSetSlice,METH_VARARGS,"testing Slice Set"},
	{"testMean", testMean, METH_VARARGS,"Testing mean"},
	{"testMax", testMax,METH_VARARGS, "Testing max"},
	{"testMin", testMin,METH_VARARGS, "Testing min"},
	{"testVertCat", testVertCat,METH_VARARGS, "Testing vertcat"},
	{"testHorzcat", testHorzcat,METH_VARARGS, "Testing Horzcat"},
	{"testZeros", testZeros, METH_VARARGS, "Testing Zeros"},
	{"testZeros_int", testZeros_int, METH_VARARGS, "Testing Zeros"},
	{"testOnes", testOnes, METH_VARARGS, "Testing Ones"},
	{"testIndexVal", testIndexVal, METH_VARARGS, "Testing IndexVal"},
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