ifndef GTEST
$(error GTEST is not set, add to your .bashrc GTEST env. variable pointing to gtest directory without slash at the end)
else

include src/Makevars
CXX := $(CXX) -isystem $(GTEST)/include
CPPFLAGS := $(PKG_CPPFLAGS)
CXXFLAGS := $(PKG_CXXFLAGS) -g -O2 -s -pthread
LDLIBS := $(PKG_LIBS) $(LDLIBS) -O1 -larmadillo -O1 -llapack -lblas
USER_DIR = ../../src

TESTS = libsvm_test

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


libsvm_test: gtest_main.a 
	$(CXX) libsvm_test.cc src/svm_basic.cpp src/libsvm_runner.cpp src/svm.cpp $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ 

svm_utils_test: gtest_main.a 
	$(CXX) svm_utils_test.cc $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ 




endif
