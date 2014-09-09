#ifndef __VR_BINDING__
#define __VR_BINDING__
#define __MATLAB__
/* #define __PYTHON__ */
#ifdef __MATLAB__
#include<vrbinding_mat.hpp>
#elif defined __PYTHON__ 
#include<vrbinding_py.hpp>
#endif
#endif
