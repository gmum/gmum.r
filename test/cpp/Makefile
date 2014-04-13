ifndef GTEST
$(error GTEST is not set, add to your .bashrc GTEST env. variable pointing to gtest directory without slash at the end)
else

include src/Makevars
CXX := $(CXX) -isystem $(GTEST)/include

USER_DIR = ../../src

TESTS = ClusterTest firstTest onlineTest ellipse_gauss_test

GTEST_HEADERS = $(GTEST)/include/gtest/*.h \
                $(GTEST)/include/gtest/internal/*.h

all : $(TESTS)

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

%.o : %.cc
	$(CXX) $(RFLAGS) $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -c

ClusterTest : ClusterTest.o src/Cluster.o gtest_main.a
	$(CXX) $(RFLAGS) $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

firstTest : firstTest.o gtest_main.a
	$(CXX) $(RFLAGS) $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

onlineTest: onlineTest.o src/Cluster.o gtest_main.a
	$(CXX) $(RFLAGS) $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

ellipse_gauss_test: ellipse_gauss_test.o gtest_main.a cluster_reader.hpp clustering_comparator.hpp src/Hartigan.o src/Cluster.o src/random_assignment.o src/CEC.o
	$(CXX) $(RFLAGS) $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

endif