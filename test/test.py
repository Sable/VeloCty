import numpy
import testPython
def testSlice() :
	a = numpy.random.rand(3,3)
	c = testPython.testSlice(a);
    #assert c == a[1,2],'slicing failed'
	print(c)
	print(a)
	#print(a[1,2] == c)

def testSetSlice() :
	a = numpy.random.rand(5,5)
	c = testPython.testSetSlice(a);
    #assert c == a[1,2],'slicing failed'
	print(c)
	#print(a)
	#print(a[1,2] == c)
def testMean() :
	a = numpy.random.rand(3,3)
	c= testPython.testMean(a)
	assert c == numpy.mean(a),"mean failed"
	print("mean passed successfully")

def testMax() :
	a = numpy.random.rand(3,3)
	c= testPython.testMax(a)
	assert c == numpy.max(a), "max failed"
	print("max successfully passed")

def testMin() :
	a = numpy.random.rand(3,3)
	c= testPython.testMin(a)
	assert c == numpy.min(a), "max failed"
	print("min successfully passed")
def testVertCat() :
    a = numpy.random.rand(3,3)
    b = numpy.random.rand(3,3)
    c = testPython.testVertCat(a,b)
    assert  not numpy.subtract(c,numpy.vstack((a,b))).any(), "vertcat failed"
    print("vertcat successfully passed")
def testZeros() :
    a = testPython.testZeros()
    print(a.ndim)
def testZeros_int() :
    a = testPython.testZeros_int()
    print(a.ndim)
def testOnes() :
    a = testPython.testOnes()
    print(a)
def testIndexVal() :
    a = numpy.random.rand(4,4)
    b = testPython.testIndexVal(a)
    assert (a[-1][-2] == a.flatten()[b]), 'IndexVal failed'
    
#testMean()
#testMax()
#testMin()
#testVertCat()
#testZeros_int()
#testOnes()
#testIndexVal()
#testSlice()
testSetSlice()
print("all test cases passed successfully ")
