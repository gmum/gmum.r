include ../../src/Makevars

# Explicit Makevars inheritances adn overrides
CXX := $(CXX)
CXXFLAGS := $(PKG_CXXFLAGS) -fPIC
CPPFLAGS := $(PKG_CPPFLAGS) -O2 -g -s
LDLIBS := $(PKG_LIBS) -lpthread -lgtest -lgtest_main -lpython2.7 \
	-llapack -lblas -larmadillo

###
# Compilation parameters
###

# Output executable
TEST_MAIN_BIN := run_tests

# Root paths
ROOT_INCLUDE_PATH := ../../inst/include
ROOT_SRC_PATH := ../../src
ROOT_TEST_PATH := .

# Branches that will be compiled
GMUMR_BRANCHES := cec #svm gng utils
TEST_BRANCHES := cec gng svm

# Additional cleaning paths
ADDITIONAL_CLEANING := *.graphml *.bin

# Generate helper scripts for extra tests (run_TESTTYPE_tests.sh)
HELPER_SCRIPT_PATTERN_1 := run_
HELPER_SCRIPT_PATTERN_2 := _tests.sh
PRIMARY_TESTS_NAME := primary
SECONDARY_TESTS_NAME := secondary

# GTest names patterns that should be separated from primary tests
# See: `./run_tests -h | grep -- --gtest_filter`
SECONDARY_TESTS_TYPES := *NumericTest*

###
#  includes
###
DEPENDENCIES_INCLUDES := -I /usr/include/boost

###
# Library compiled libraries
###
SVMLIGHT_INCLUDE_PATH := $(ROOT_INCLUDE_PATH)/svmlight
SVMLIGHT_SRC_PATH := $(ROOT_SRC_PATH)/svmlight
SVMLIGHT_OBJECT_FILENAMES := svm_common.o svm_hideo.o svm_learn.o

###
# Generic variables
###

# Compilation
SOURCES_WILDCARD := $(patsubst %, $(ROOT_SRC_PATH)/%/*.cpp, $(GMUMR_BRANCHES))
TEST_SOURCES_WILDCARD := $(patsubst %, $(ROOT_TEST_PATH)/%/*.cpp, $(TEST_BRANCHES))

# TODO: Standarize headers to *.hpp PLZ...

HEADERS_WILDCARD_H := $(patsubst %, $(ROOT_INCLUDE_PATH)/%/*.h, $(GMUMR_BRANCHES))
HEADERS_WILDCARD_HPP := $(patsubst %, $(ROOT_INCLUDE_PATH)/%/*.hpp, $(GMUMR_BRANCHES))
HEADERS_H := $(wildcard $(HEADERS_WILDCARD_H))
HEADERS_HPP := $(wildcard $(HEADERS_WILDCARD_HPP))
HEADERS :=  $(HEADERS_H) $(HEADERS_HPP)

SOURCES := $(wildcard $(SOURCES_WILDCARD))
TEST_SOURCES := $(wildcard $(TEST_SOURCES_WILDCARD))

OBJECTS := $(patsubst %.cpp, %.o, $(SOURCES))
TEST_OBJECTS := $(patsubst %.cpp, %.o, $(TEST_SOURCES))

# Externally compiled libraries objects
SVMLIGHT_OBJECT_PATHS := $(patsubst %, $(SVMLIGHT_INCLUDE_PATH)/%, $(SVMLIGHT_OBJECT_FILENAMES))

# Sum up compiled libraries objects
COMPILED_LIBRARIES_OBJECTS := $(SVMLIGHT_OBJECT_PATHS)

# Helper scripts
PRIMARY_TESTS_SCRIPT := $(patsubst %, $(HELPER_SCRIPT_PATTERN_1)%$(HELPER_SCRIPT_PATTERN_2), $(PRIMARY_TESTS_NAME))
SECONDARY_TESTS_SCRIPT := $(patsubst %, $(HELPER_SCRIPT_PATTERN_1)%$(HELPER_SCRIPT_PATTERN_2), $(SECONDARY_TESTS_NAME))

###
# Includes
###

# Root directory (to allow explicit including f.e `#include "svm/svm_basic.h"`)
ROOT_INCLUDE := -I $(ROOT_INCLUDE_PATH)

# Generic branch includes
BRANCH_INCLUDES := $(patsubst %, -I $(ROOT_INCLUDE_PATH)/%, $(GMUMR_BRANCHES))

# Sum up external and compiled libraries includes
SVMLIGHT_INCLUDE := -I $(SVMLIGHT_INCLUDE_PATH)
COMPILED_LIBRARIES_INCLUDES := $(SVMLIGHT_INCLUDE)
LIBRARIES_INCLUDES := $(COMPILED_LIBRARIES_INCLUDES) $(DEPENDENCIES_INCLUDES)

# Sum up all
INCLUDES := $(ROOT_INCLUDE) $(BRANCH_INCLUDES) $(LIBRARIES_INCLUDES)

###
# SWIG parameters
###

# Path to our python package
GMUMPY_PATH := ../../python/gmumpy

# Our custom interfaces
SWIG_INTERFACES := ../../python/interface
INCLUDES := $(INCLUDES) -I $(SWIG_INTERFACES)

# GMUM.py shared object
GMUMPY_SO := $(GMUMPY_PATH)/_core.so

# Path for SWIG-generated sources and objects
SWIG_OUTPUT_PATH := ../../python/swig

PYTHON_INCLUDES := -I /usr/include/python2.7
GMUMPY_INCLUDES := $(INCLUDES) $(PYTHON_INCLUDES)

CEC_SOURCES := cec/cec_configuration.cpp cec/hartigan.cpp
SVM_SOURCES := svm/svm_basic.cpp

PYTHON_INTERFACE := $(ROOT_SRC_PATH)/python_interface.i

# TODO: Standarize headers to *.hpp PLZ...

WRAPPERS_H := $(patsubst $(ROOT_INCLUDE_PATH)/%.h, $(ROOT_SRC_PATH)/%_wrap.cxx, $(HEADERS_H))
WRAPPERS_HPP := $(patsubst $(ROOT_INCLUDE_PATH)/%.hpp, $(ROOT_SRC_PATH)/%_wrap.cxx, $(HEADERS_HPP))
WRAPPERS := $(WRAPPERS_H) $(WRAPPERS_HPP)

###
# Compilation rules
###

.PHONY: $(TEST_MAIN_BIN) python debug clean clean-python

###
# Managing Projects with GNU Make, Third Edition, Chapter 12
# By Robert Mecklenburg
# November 2004, O'Reilly Media
###

# Try `make debug SOURCES` or `make debug INCLUDES` etc.
debug:
	$(foreach v,$(V) $(MAKECMDGOALS), \
		$(if $(filter debug,$v),,$(warning $v = $($v))))

## Mecklenburg

CPPFLAGS := $(CPPFLAGS) $(INCLUDES)
CFLAGS := $(CPPFLAGS)

clean:
	rm -f $(TEST_MAIN_BIN) $(TEST_OBJECTS) $(COMPILED_LIBRARIES_OBJECTS)\
	$(PRIMARY_TESTS_SCRIPT) $(SECONDARY_TESTS_SCRIPT) $(ADDITIONAL_CLEANING)

$(TEST_MAIN_BIN): $(TEST_OBJECTS) $(OBJECTS) $(COMPILED_LIBRARIES_OBJECTS) helper_scripts
	@echo Linking all tests...
	$(CXX) $^ -lgtest_main -o $@ $(LDLIBS)
	@echo ... Done!
	@echo See running options with ./run_tests -h
	@echo or use helper scripts: $(PRIMARY_TESTS_SCRIPT) $(SECONDARY_TESTS_SCRIPT)

%.o: %.cpp %.hpp
	@echo Compiling $@ ...
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@ 

helper_scripts:
	@echo Making helper scripts...
	make helper_script filename="$(PRIMARY_TESTS_SCRIPT)" pattern=-$(SECONDARY_TESTS_TYPES)
	make helper_script filename="$(SECONDARY_TESTS_SCRIPT)" pattern=$(SECONDARY_TESTS_TYPES)

helper_script:
	$(shell printf "#!/usr/bin/env bash\n./run_tests --gtest_filter=$(pattern)\n" > $(filename))
	$(shell chmod +x $(filename))

###
# SVMLight
###

$(SVMLIGHT_INCLUDE_PATH)/%.o:
	cd $(SVMLIGHT_INCLUDE_PATH) && make $*.o

###
# SWIG: Compilation rules
###

python: $(OBJECTS) $(COMPILED_LIBRARIES_OBJECTS)
	mkdir -p $(SWIG_OUTPUT_PATH)/{,cec,gng,svm}
	rm -f $(PYTHON_INTERFACE)
	# Collect headers and create SWIG interface
	$(foreach b, $(GMUMR_BRANCHES), \
		cat $(ROOT_INCLUDE_PATH)/$b/*.hpp >> $(PYTHON_INTERFACE);)
	# Collect our custom interfaces
	cat $(SWIG_INTERFACES)/*.i >> $(PYTHON_INTERFACE)
	# Generate SWIG C++ wrapping code...
	swig -c++ -module core -outdir $(GMUMPY_PATH) -python \
		-I$(SWIG_INTERFACES) \
		-o $(ROOT_SRC_PATH)/python_interface.cxx $(PYTHON_INTERFACE)
	# Compile SWIG C++ wrapping code...
	g++ $(CPPFLAGS) $(CXXFLAGS) -O2 -fPIC \
		-c $(ROOT_SRC_PATH)/python_interface.cxx \
		-o $(ROOT_SRC_PATH)/python_interface.o $(GMUMPY_INCLUDES)
	# Link shared object
	$(CXX) -fPIC -shared $(LDLIBS) \
		$(ROOT_SRC_PATH)/python_interface.o $(OBJECTS) $(COMPILED_LIBRARIES_OBJECTS) \
		-o $(GMUMPY_SO)


clean-python:
	rm -f $(ROOT_SRC_PATH)/python*
	rm -rf $(GMUMPY_PATH)/{_core.so, core.py}


