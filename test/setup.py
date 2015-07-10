from distutils.core import setup, Extension
import glob
import os
source_files = ['testPython.cpp']
#for file in glob.glob('/home/sable/sjagda/thesis/VeloCty/matrixOpsPy/*.cpp') :
#	source_files.append(file)
extraObjects = ['/home/sable/sjagda/open_blas/lib/libopenblas.so','/home/sable/sjagda/OpenBLAS/libopenblas.a']
module1 = Extension('testPython', sources = source_files,include_dirs = ['/home/sable/sjagda/boehmgc/include','/home/sable/sjagda/thesis/VeloCty/matrixOpsPy/','/home/sable/sjagda/Poco/include/','/home/sable/sjagda/open_blas/lib_lapack/include/','/home/sable/sjagda/open_blas/include/','/home/sable/sjagda/OpenBLAS'],
			libraries = ['gfortran','gc'],
			extra_objects = extraObjects,
			library_dirs = ['~/Poco/lib/','/home/sable/sjagda/boehmgc/lib/'],define_macros = [('HAVE_LAPACK_CONFIG_H',None),('VR_MINIMAL',None)])
setup(name = 'PackageName', version = 1.0, description = 'Test module', ext_modules = [module1])
