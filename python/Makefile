##
# GMUM Library SWIG Makefile sketch
##

# Path to .i files
INTERFACE_PATH := interface

# Path for compiled gmumpy .so and .py library files
PYTHON_MODULES_PATH := gmumpy

# Path for SWIG-generated sources and objects
SWIG_OUTPUT_PATH := swig

# GMUM paths
SRC_PATH := src

GMUM_INCLUDES := -I include -I include/svm -I include/cec 
PYTHON_INCLUDES := -I /usr/include/python2.7

CEC_SOURCES := cec/assignment.cpp cec/cec_configuration.cpp cec/centroids_assignment.cpp cec/hartigan.cpp cec/cluster.cpp cec/cec.cpp cec/kmeanspp_assignment.cpp cec/random_assignment.cpp
SVM_SOURCES := svm/svm_basic.cpp

INCLUDES := $(PYTHON_INCLUDES) $(GMUM_INCLUDES) -I interface

CEC_OBJECTS := $(patsubst %.cpp, swig/%.o, $(CEC_SOURCES))
SVM_OBJECTS := $(patsubst %.cpp, swig/%.o, $(SVM_SOURCES))

all: cec svm
.PHONY: all

cec: cec.py $(CEC_OBJECTS)
	g++ -shared $(CEC_OBJECTS) $(SWIG_OUTPUT_PATH)/$@_wrap.o -o $(PYTHON_MODULES_PATH)/_$@.so -llapack -lblas -larmadillo 
svm: svm.py $(SVM_OBJECTS)
	g++ -shared $(SVM_OBJECTS) $(SWIG_OUTPUT_PATH)/$@_wrap.o -o $(PYTHON_MODULES_PATH)/_$@.so

swig/%.o:
	g++ -O2 -fPIC -std=c++0x -c $(SRC_PATH)/$*.cpp -o $(SWIG_OUTPUT_PATH)/$*.o $(GMUM_INCLUDES)

%.py: init
	swig -c++ -outdir $(PYTHON_MODULES_PATH) -python -o $(SWIG_OUTPUT_PATH)/$*_wrap.cxx $(INTERFACE_PATH)/$*.i
	g++ -O2 -fPIC -c $(SWIG_OUTPUT_PATH)/$*_wrap.cxx -o $(SWIG_OUTPUT_PATH)/$*_wrap.o $(INCLUDES)

init:
	mkdir -p swig/cec swig/gng swig/svm
	touch gmumpy/__init__.py

clean:
	rm gmumpy/*.py gmumpy/*.pyc gmumpy/*.so swig/*.o swig/*.cxx
