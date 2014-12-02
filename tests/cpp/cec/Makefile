ifndef GTEST
$(error GTEST is not set, add to your .bashrc GTEST env. variable pointing to gtest directory without slash at the end)
else

GMUMR_ROOT_DIR = ../../..

INCLUDE_ROOT_DIR = $(GMUMR_ROOT_DIR)/inst/include
SOURCES_ROOT_DIR = $(GMUMR_ROOT_DIR)/src

CEC_INCLUDE_DIR = $(INCLUDE_ROOT_DIR)/cec
CEC_SOURCES_DIR = $(SOURCES_ROOT_DIR)/cec

CEC_SOURCES = $(wildcard $(CEC_SOURCES_DIR)/*.cpp)
SOURCES = $(CEC_SOURCES)
OBJECTS = $(patsubst %.cpp, %.o, $(SOURCES))

include $(SOURCES_ROOT_DIR)/Makevars

CXX := $(CXX) -isystem $(GTEST)/include
CPPFLAGS := -I$(INCLUDE_ROOT_DIR) -I$(CEC_INCLUDE_DIR) $(PKG_CPPFLAGS)
CXXFLAGS := $(PKG_CXXFLAGS) -g -O2 -s -pthread
LDLIBS := $(PKG_LIBS) $(LDLIBS) -O1 -larmadillo -O1 -llapack -lblas

TESTS = onlineTest ellipse_gauss_test simple_1_test mouse_1_test clusterTraceOnlyTest mouse_1_spherical_test 

GTEST_HEADERS = $(GTEST)/include/gtest/*.h \
                $(GTEST)/include/gtest/internal/*.h

all : $(TESTS)

debug:
	@echo $(CXX)
	@echo $(CXXFLAGS)
	@echo $(CPPFLAGS)
	@echo $(ARFLAGS)
	@echo $(AR)
echo :
	@echo $(R_HOME)
	@echo $(RCPPFLAGTESTS)
	@echo $(RLDFLAGS)
	@echo $(RCPPINCL)
	@echo $(RCPPLIBS)
	@echo $(RCPPARMAINCL)
	@echo $(CXX)
	@echo $(CPPFLAGS)
	@echo $(CXXFLAGS)
	@echo $(LDLIBS)

clean :
	rm -f $(TESTS) gtest.a gtest_main.a *.o

.PHONY : echo clean 

GTEST_SRCS_ = $(GTEST)/src/*.cc $(GTEST)/src/*.h $(GTEST_HEADERS)

gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST) $(CXXFLAGS) -c \
            $(GTEST)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST) $(CXXFLAGS) -c \
            $(GTEST)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

#%.o : %.cpp
#	$(CXX) $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -c

onlineTest: gtest_main.a 
	$(CXX) onlineTest.cpp $(CEC_SOURCES_DIR)/cluster.cpp $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ 

clusterTraceOnlyTest: gtest_main.a 
	$(CXX) clusterTraceOnlyTest.cpp $(CEC_SOURCES_DIR)/cluster.cpp $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ 

ellipse_gauss_test: gtest_main.a 
	$(CXX) ellipse_gauss_test.cpp cluster_reader.cpp $(CEC_SOURCES_DIR)/cec.cpp $(CEC_SOURCES_DIR)/cecConfiguration.cpp $(CEC_SOURCES_DIR)/cluster.cpp $(CEC_SOURCES_DIR)/hartigan.cpp $(CEC_SOURCES_DIR)/randomAssignment.cpp $(CEC_SOURCES_DIR)/assignment.cpp $(CEC_SOURCES_DIR)/clusterCustomFunction.cpp $(CEC_SOURCES_DIR)/kmeansppAssignment.cpp $(CEC_SOURCES_DIR)/centroidsAssignment.cpp $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ 

mouse_1_test: gtest_main.a 
	$(CXX) mouse_1_test.cpp cluster_reader.cpp $(CEC_SOURCES_DIR)/cec.cpp $(CEC_SOURCES_DIR)/cecConfiguration.cpp $(CEC_SOURCES_DIR)/cluster.cpp $(CEC_SOURCES_DIR)/clusterCustomFunction.cpp $(CEC_SOURCES_DIR)/hartigan.cpp $(CEC_SOURCES_DIR)/randomAssignment.cpp $(CEC_SOURCES_DIR)/kmeansppAssignment.cpp $(CEC_SOURCES_DIR)/centroidsAssignment.cpp  $(CEC_SOURCES_DIR)/assignment.cpp $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ 

mouse_1_spherical_test: gtest_main.a 
	$(CXX) mouse_1_spherical_test.cpp cluster_reader.cpp $(CEC_SOURCES_DIR)/cec.cpp $(CEC_SOURCES_DIR)/cecConfiguration.cpp $(CEC_SOURCES_DIR)/cluster.cpp $(CEC_SOURCES_DIR)/clusterCustomFunction.cpp $(CEC_SOURCES_DIR)/hartigan.cpp $(CEC_SOURCES_DIR)/randomAssignment.cpp $(CEC_SOURCES_DIR)/kmeansppAssignment.cpp $(CEC_SOURCES_DIR)/centroidsAssignment.cpp $(CEC_SOURCES_DIR)/assignment.cpp $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ 

simple_1_test: gtest_main.a 
	$(CXX) simple_1_test.cpp cluster_reader.cpp $(CEC_SOURCES_DIR)/cec.cpp $(CEC_SOURCES_DIR)/cecConfiguration.cpp $(CEC_SOURCES_DIR)/cluster.cpp $(CEC_SOURCES_DIR)/clusterCustomFunction.cpp $(CEC_SOURCES_DIR)/hartigan.cpp $(CEC_SOURCES_DIR)/assignment.cpp $(CEC_SOURCES_DIR)/kmeansppAssignment.cpp $(CEC_SOURCES_DIR)/centroidsAssignment.cpp $(CEC_SOURCES_DIR)/randomAssignment.cpp $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ 

#big_data_tests: gtest_main.a
#	$(CXX) big_data_tests.cpp cluster_reader.cpp $(CEC_SOURCES_DIR)/cec.cpp $(CEC_SOURCES_DIR)/cecConfiguration.cpp $(CEC_SOURCES_DIR)/cluster.cpp $(CEC_SOURCES_DIR)/hartigan.cpp $(CEC_SOURCES_DIR)/clusterCustomFunction.cpp $(CEC_SOURCES_DIR)/randomAssignment.cpp $(CEC_SOURCES_DIR)/kmeansppAssignment.cpp $(CEC_SOURCES_DIR)/centroidsAssignment.cpp $(CEC_SOURCES_DIR)/assignment.cpp $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ 

endif
