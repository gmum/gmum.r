include ../../src/Makevars

# Explicit Makevars inheritances adn overrides
CXX := $(CXX)
CPPFLAGS := $(PKG_CPPFLAGS)
CXXFLAGS := $(PKG_CXXFLAGS) -O2 -g -s
LDLIBS := $(PKG_LIBS) -O1 -larmadillo -O1 -llapack -lblas -lgtest -lgtest_main

###
# Compilation parameters
###

# Output executable
TEST_ALL_BIN := test_all

# Root paths
ROOT_INCLUDE_PATH := ../../inst/include
ROOT_SRC_PATH := ../../src
ROOT_TEST_PATH := .

# Branches that will be compiled
GMUMR_BRANCHES := gng svm tinythread
TEST_BRANCHES := gng svm

# Additional cleaning paths
ADDITIONAL_CLEANING := *.graphml *.bin

###
# Generic variables
###

SOURCES_WILDCARD := $(patsubst %, $(ROOT_SRC_PATH)/%/*.cpp, $(GMUMR_BRANCHES))
TEST_SOURCES_WILDCARD := $(patsubst %, $(ROOT_TEST_PATH)/%/*.cc, $(TEST_BRANCHES))

SOURCES := $(wildcard $(SOURCES_WILDCARD))
TEST_SOURCES := $(wildcard $(TEST_SOURCES_WILDCARD))

OBJECTS := $(patsubst %.cpp, %.o, $(SOURCES))
TEST_OBJECTS := $(patsubst %.cc, %.o, $(TEST_SOURCES))

###
# Includes
###

# Root directory (to allow explicit including f.e `#include "svm/svm_basic.h"`)
ROOT_INCLUDE := -I $(ROOT_INCLUDE_PATH)

# Generic branch includes
BRANCH_INCLUDES := $(patsubst %, -I $(ROOT_INCLUDE_PATH)/%, $(GMUMR_BRANCHES))

# Sum up
INCLUDES := $(ROOT_INCLUDE) $(BRANCH_INCLUDES)

###
# Internally included libs
###
SVMLIGHT_INCLUDE := $(ROOT_INCLUDE_PATH)/svmlight
SVMLIGHT_LIBS := $(SVMLIGHT_INCLUDE)/svm_learn.o $(SVMLIGHT_INCLUDE)/svm_common.o $(SVMLIGHT_INCLUDE)/svm_hideo.o

CPPFLAGS := $(CPPFLAGS) $(INCLUDES)

###
# Compilation rules
###

all: $(TEST_ALL_BIN)

clean :
	rm -f $(TEST_ALL_BIN) $(TEST_OBJECTS) $(OBJECTS) $(ADDITIONAL_CLEANING)

test_all: $(TEST_OBJECTS) $(OBJECTS)
	@echo Linking all tests...
	$(CXX) $^ -lgtest_main -o $@ $(LDLIBS)

%.o : %.cc %.cpp %.hpp
	@echo Compiling $@ ...
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

