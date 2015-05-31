##
# GMUM Library SWIG Makefile sketch
##

# Path to our python interface
GMUMPY_PATH := gmumpy
# Path for compiled gmumpy .so and .py library files
GMUMPY_CORE_PATH := gmumpy/core

# Path for SWIG-generated sources and objects
SWIG_OUTPUT_PATH := swig
LIB_OUTPUT_PATH := lib

# GMUM paths
SRC_PATH := src
INCLUDE_PATH := include

GMUM_INCLUDES := -I include -I include/svm -I include/cec 
PYTHON_INCLUDES := -I /usr/include/python2.7

CEC_SOURCES := cec/assignment.cpp cec/cec_configuration.cpp cec/centroids_assignment.cpp cec/hartigan.cpp cec/cluster.cpp cec/cec.cpp cec/kmeanspp_assignment.cpp cec/random_assignment.cpp
SVM_SOURCES := svm/svm_basic.cpp

INCLUDES := $(PYTHON_INCLUDES) $(GMUM_INCLUDES) -I interface

CEC_OBJECTS := $(patsubst %.cpp, $(LIB_OUTPUT_PATH)/%.o, $(CEC_SOURCES))
SVM_OBJECTS := $(patsubst %.cpp, $(LIB_OUTPUT_PATH)/%.o, $(SVM_SOURCES))

CEC_WRAPPERS := $(patsubst %.cpp, $(SWIG_OUTPUT_PATH)/%_wrap.cpp, $(CEC_SOURCES))
SVM_WRAPPERS := $(patsubst %.cpp, $(SWIG_OUTPUT_PATH)/%_wrap.cpp, $(SVM_SOURCES))

CEC_WRAPPERS_OBJECTS := $(patsubst %.cpp, $(SWIG_OUTPUT_PATH)/%_wrap.o, $(CEC_SOURCES))
SVM_WRAPPERS_OBJECTS := $(patsubst %.cpp, $(SWIG_OUTPUT_PATH)/%_wrap.o, $(SVM_SOURCES))

CEC_SHARED_OBJECTS := $(patsubst %.cpp, $(GMUMPY_CORE_PATH)/%.so, $(CEC_SOURCES))
SVM_SHARED_OBJECTS := $(patsubst %.cpp, $(GMUMPY_CORE_PATH)/%.so, $(SVM_SOURCES))

CEC_PYTHON := $(patsubst %.cpp, $(GMUMPY_CORE_PATH)/%.py, $(CEC_SOURCES))
SVM_PYTHON := $(patsubst %.cpp, $(GMUMPY_CORE_PATH)/%.py, $(SVM_SOURCES))

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
	@echo Variables: $(GMUMPY_PATH)
	mkdir -p $(GMUMPY_CORE_PATH)
	touch $(GMUMPY_CORE_PATH)/__init__.py
	mkdir -p $(LIB_OUTPUT_PATH)/{,cec,gng,svm}
	mkdir -p $(SWIG_OUTPUT_PATH)/{,cec,gng,svm}

clean:
	rm -rf $(GMUMPY_CORE_PATH)/* $(LIB_OUTPUT_PATH)/* $(SWIG_OUTPUT_PATH)/*
