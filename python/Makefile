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

CEC_SOURCES := cec/cec_configuration.cpp cec/hartigan.cpp
SVM_SOURCES := svm/svm_basic.cpp

INCLUDES := $(PYTHON_INCLUDES) $(GMUM_INCLUDES)

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

cec: $(CEC_SHARED_OBJECTS)
	@echo "Make cec..."

svm: $(SVM_SHARED_OBJECTS)
	@echo "Make svm..."

$(GMUMPY_CORE_PATH)/%.so: $(LIB_OUTPUT_PATH)/%.o $(SWIG_OUTPUT_PATH)/%_wrap.o
	@echo "Make" $@ "..." $*
	$(eval branch := $(word 1, $(subst /, ,$*)))
	$(eval filename := $(word 2, %/$(subst /, ,$*)))
	$(eval result_so := $(GMUMPY_CORE_PATH)/_$(branch)_$(filename).so)
	g++ -shared $(LIB_OUTPUT_PATH)/$*.o $(SWIG_OUTPUT_PATH)/$*_wrap.o -o $(result_so)

$(LIB_OUTPUT_PATH)/%.o:
	@echo "Make" $@ "..."
	g++ -O2 -fPIC -larmadillo -c $(SRC_PATH)/$*.cpp -o $(LIB_OUTPUT_PATH)/$*.o $(GMUM_INCLUDES)

$(SWIG_OUTPUT_PATH)/%_wrap.o: $(GMUMPY_CORE_PATH)/%.py
	@echo "Make" $@ "..."
	g++ -O2 -fPIC -larmadillo -c $(SWIG_OUTPUT_PATH)/$*_wrap.cpp -o $(SWIG_OUTPUT_PATH)/$*_wrap.o $(INCLUDES)

# TODO: Standarize headers to *.hpp...

$(GMUMPY_CORE_PATH)/%.py: init $(INCLUDE_PATH)/%.h
	@echo Make $@ AND $(SWIG_OUTPUT_PATH)/$*_wrap.cpp...
	swig -c++ -module $(subst /,_,$*) -outdir $(GMUMPY_CORE_PATH) -python -o $(SWIG_OUTPUT_PATH)/$*_wrap.cpp $(INCLUDE_PATH)/$*.h

$(GMUMPY_CORE_PATH)/%.py: init $(INCLUDE_PATH)/%.hpp
	@echo Make $@ AND $(SWIG_OUTPUT_PATH)/$*_wrap.cpp...
	swig -c++ -module $(subst /,_,$*) -outdir $(GMUMPY_CORE_PATH) -python -o $(SWIG_OUTPUT_PATH)/$*_wrap.cpp $(INCLUDE_PATH)/$*.hpp

init:
	@echo Variables: $(GMUMPY_PATH)
	mkdir -p $(GMUMPY_CORE_PATH)
	touch $(GMUMPY_CORE_PATH)/__init__.py
	mkdir -p $(LIB_OUTPUT_PATH)/{,cec,gng,svm}
	mkdir -p $(SWIG_OUTPUT_PATH)/{,cec,gng,svm}

clean:
	rm -rf $(GMUMPY_CORE_PATH)/* $(LIB_OUTPUT_PATH)/* $(SWIG_OUTPUT_PATH)/*
