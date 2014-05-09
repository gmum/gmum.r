ifndef GTEST
$(error GTEST is not set, add to your .bashrc GTEST env. variable pointing to gtest directory without slash at the end)
else

include src/Makevars
CXX := $(CXX) -isystem $(GTEST)/include
CXXFLAGS := -g -O2 -s -pthread
LDLIBS := -larmadillo
USER_DIR = ../../src

TESTS = ClusterTest firstTest onlineTest ellipse_gauss_test simple_1_test mouse_1_test

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
	@echo $(RCPPFLAGS)
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

#%.o : %.cc
#	$(CXX) $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -c

ClusterTest : gtest_main.a 
	$(CXX) ClusterTest.cc  src/Cluster.cpp  $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ 

firstTest : gtest_main.a
	$(CXX) firstTest.cc  $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ 

onlineTest: gtest_main.a 
	$(CXX) onlineTest.cc src/Cluster.hpp src/Cluster.cpp $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ 

ellipse_gauss_test: gtest_main.a 
	$(CXX) ellipse_gauss_test.cc src/CEC.hpp src/CEC.cpp src/Cluster.hpp src/Cluster.cpp src/Hartigan.hpp src/Hartigan.cpp src/random_assignment.hpp src/random_assignment.cpp  $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ 

mouse_1_test: gtest_main.a 
	$(CXX) mouse_1_test.cc src/CEC.hpp src/CEC.cpp src/Cluster.hpp src/Cluster.cpp src/Hartigan.hpp src/Hartigan.cpp src/random_assignment.hpp src/random_assignment.cpp  $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ 

simple_1_test: gtest_main.a 
	$(CXX) simple_1_test.cc src/CEC.hpp src/CEC.cpp src/Cluster.hpp src/Cluster.cpp src/Hartigan.hpp src/Hartigan.cpp  $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ 

endif
