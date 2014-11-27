GMUMR_SRC_DIR = ../../src
SVMLIGHT_DIR := ../../inst/include/svmlight

include ../../src/Makevars

# Root paths
ROOT_INCLUDE_PATH := ../../inst/include
ROOT_INCLUDE := -I $(ROOT_INCLUDE_PATH)

## Separate include dirs for transparency
# CEC
CEC_INCLUDE := -I $(ROOT_INCLUDE_PATH)/cec
# GNG
GNG_INCLUDE :=      -I $(ROOT_INCLUDE_PATH)/gng
GNG_TEST_OUTPUTS := *.graphml *.bin
# SVM Wrapper
SVM_INCLUDE :=      -I ../../inst/include/svm
SVMLIGHT_INCLUDE := -I $(SVMLIGHT_DIR)
# Root directory (to allow explicit including f.e `svmlight/svm_learn.h`)
INCLUDE_DIRS :=     $(ROOT_INCLUDE) $(CEC_INCLUDE) $(SVM_INCLUDE) $(GNG_INCLUDE) -I ../../ -I ..

OBJECTS :=          $(patsubst %.cpp, %.o, $(SOURCES))

SVMLIGHT_LIBS :=    $(SVMLIGHT_INCLUDE)/svm_learn.o $(SVMLIGHT_INCLUDE)/svm_common.o $(SVMLIGHT_INCLUDE)/svm_hideo.o

# Clean paths
TEST_OUTPUTS :=		*.graphml *.bin

CXX := $(CXX)
CPPFLAGS := $(INCLUDE_DIRS) $(PKG_CPPFLAGS)
CXXFLAGS := $(PKG_CXXFLAGS) -g -O2 -s -pthread
LDLIBS := $(PKG_LIBS) $(LDLIBS) -O1 -larmadillo -O1 -llapack -lblas -lgtest -lgtest_main

TESTS = gng_basic_integration_tests gng_parts_tests svm_2_e_test \
	svm_libsvm_test svm_utils_test cec_big_data_tests \
	cec_cluster_trace_only_test cec_ellipse_gauss_test \
	cec_mouse_1_spherical_test cec_mouse_1_test cec_onlineTest \
	cec_simple_1_test #cec_test_reader

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

cec_big_data_tests:
	$(CXX) cec/big_data_tests.cc $(GMUMR_SRC_DIR)/cec/*.cpp $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

cec_cluster_trace_only_test:
	$(CXX) cec/clusterTraceOnlyTest.cc $(GMUMR_SRC_DIR)/cec/*.cpp $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

cec_ellipse_gauss_test:
	$(CXX) cec/ellipse_gauss_test.cc $(GMUMR_SRC_DIR)/cec/*.cpp $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

cec_mouse_1_spherical_test:
	$(CXX) cec/mouse_1_spherical_test.cc $(GMUMR_SRC_DIR)/cec/*.cpp $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

cec_mouse_1_test:
	$(CXX) cec/mouse_1_test.cc $(GMUMR_SRC_DIR)/cec/*.cpp $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

cec_onlineTest:
	$(CXX) cec/onlineTest.cc $(GMUMR_SRC_DIR)/cec/*.cpp $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

cec_simple_1_test:
	$(CXX) cec/simple_1_test.cc $(GMUMR_SRC_DIR)/cec/*.cpp $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

cec_test_reader:
	$(CXX) cec/testReader.cc $(GMUMR_SRC_DIR)/cec/*.cpp $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

gng_basic_integration_tests: $(GMUMR_SRC_DIR)/tinythread/tinythread.o
	$(CXX) gng/basic_integration_tests.cc $(GMUMR_SRC_DIR)/gng/*.cpp $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

gng_parts_tests: $(GMUMR_SRC_DIR)/tinythread/tinythread.o
	$(CXX) gng/parts_tests.cc $(GMUMR_SRC_DIR)/gng/*.cpp $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

svm_2_e_test:
	$(CXX) svm/2_e_test.cc $(GMUMR_SRC_DIR)/svm/svm_basic.cpp $(GMUMR_SRC_DIR)/svm/libsvm_runner.cpp $(GMUMR_SRC_DIR)/svm/svm.cpp $(GMUMR_SRC_DIR)/svm/two_e_svm_pre.cpp $(GMUMR_SRC_DIR)/svm/two_e_svm_post.cpp $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

svm_libsvm_test:
	$(CXX) svm/libsvm_test.cc $(GMUMR_SRC_DIR)/svm/svm_basic.cpp $(GMUMR_SRC_DIR)/svm/libsvm_runner.cpp $(GMUMR_SRC_DIR)/svm/svm.cpp $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

svm_svmlight_runner_unittest: $(OBJECTS) $(SVMLIGHT_DIR)/svm_common.o $(SVMLIGHT_DIR)/svm_learn.o $(SVMLIGHT_DIR)/svm_hideo.o
	$(CXX) $(CPPFLAGS) svm/svmlight_runner_unittest.cc $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

svm_utils_test:
	$(CXX) svm/svm_utils_test.cc $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

