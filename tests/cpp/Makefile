GMUMR_SRC_DIR = ../../src
SVMLIGHT_DIR := ../../inst/include/svmlight

include ../../src/Makevars

## Separate include dirs for transparency
# SVM Wrapper
SVM_INCLUDE :=      -I ../../inst/include/svm
SVMLIGHT_INCLUDE := -I $(SVMLIGHT_DIR)
# GNG
GNG_INCLUDE :=      -I ../../inst/include/gng
GNG_TEST_OUTPUTS := *.graphml *.bin
# Root directory (to allow explicit including f.e `svmlight/svm_learn.h`)
INCLUDE_ROOT :=     -I ../../inst/include
INCLUDE_DIRS :=     $(INCLUDE_ROOT) $(SVM_INCLUDE) $(GNG_INCLUDE) -I ../../ -I ..

## Source dirs
# SVM Wrapper
SVM_SOURCES :=      $(wildcard $(GMUMR_SRC_DIR)/svm/*.cpp)
#SOURCES :=          $(SVM_SOURCES)
OBJECTS :=          $(patsubst %.cpp, %.o, $(SOURCES))

SVMLIGHT_LIBS :=    $(SVMLIGHT_INCLUDE)/svm_learn.o $(SVMLIGHT_INCLUDE)/svm_common.o $(SVMLIGHT_INCLUDE)/svm_hideo.o

# Clean paths
TEST_OUTPUTS :=		*.graphml *.bin

CXX := $(CXX)
CPPFLAGS := $(INCLUDE_DIRS) $(PKG_CPPFLAGS)
CXXFLAGS := $(PKG_CXXFLAGS) -g -O2 -s -pthread
LDLIBS := $(PKG_LIBS) $(LDLIBS) -O1 -larmadillo -O1 -llapack -lblas -lgtest -lgtest_main

TESTS = libsvm_test svm_utils_test 2_e_test gng_basic_integration_tests gng_parts_tests

all : $(TESTS)

debug:
	@echo $(CXX)
	@echo $(CXXFLAGS)
	@echo $(CPPFLAGS)
	@echo $(ARFLAGS)
	@echo $(AR)
echo :
	@echo $(R_HOME)
	@echo $(RCPPFLAGS)
	@echo $(RLDFLAGS)
	@echo $(RCPPINCL)
	@echo $(RCPPLIBS)
	@echo $(RCPPARMAINCL)
	@echo $(CXX)
	@echo $(CPPFLAGS)
	@echo $(CXXFLAGS)
	@echo $(LDLIBS)

# TODO: Clean objects
clean :
	rm -f $(TESTS) $(TEST_OUTPUTS)

.PHONY : echo clean

#%.o : %.cc
#	$(CXX) $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -c

%.o : %.cpp %.h
	$(CXX) $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -c

libsvm_test:
	$(CXX) svm/libsvm_test.cc $(GMUMR_SRC_DIR)/svm/svm_basic.cpp $(GMUMR_SRC_DIR)/svm/libsvm_runner.cpp $(GMUMR_SRC_DIR)/svm/svm.cpp $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

svm_utils_test:
	$(CXX) svm/svm_utils_test.cc $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

2_e_test:
	$(CXX) svm/2_e_test.cc $(GMUMR_SRC_DIR)/svm/svm_basic.cpp $(GMUMR_SRC_DIR)/svm/libsvm_runner.cpp $(GMUMR_SRC_DIR)/svm/svm.cpp $(GMUMR_SRC_DIR)/svm/two_e_svm_pre.cpp $(GMUMR_SRC_DIR)/svm/two_e_svm_post.cpp $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

svmlight_runner_unittest: $(OBJECTS) $(SVMLIGHT_DIR)/svm_common.o $(SVMLIGHT_DIR)/svm_learn.o $(SVMLIGHT_DIR)/svm_hideo.o
	$(CXX) $(CPPFLAGS) svm/svmlight_runner_unittest.cc $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

gng_basic_integration_tests: $(GMUMR_SRC_DIR)/tinythread/tinythread.o
	$(CXX) gng/basic_integration_tests.cpp $(GMUMR_SRC_DIR)/gng/*.cpp $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

gng_parts_tests: $(GMUMR_SRC_DIR)/tinythread/tinythread.o
	$(CXX) gng/parts_tests.cpp $(GMUMR_SRC_DIR)/gng/*.cpp $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

