
R_HOME := 		$(shell R RHOME)
RCPPFLAGS := 		$(shell $(R_HOME)/bin/R CMD config --cppflags)
RLDFLAGS := 		$(shell $(R_HOME)/bin/R CMD config --ldflags)
RCPPINCL := 		$(shell echo 'Rcpp:::CxxFlags()' | $(R_HOME)/bin/R --vanilla --slave)
RCPPLIBS := 		$(shell echo 'Rcpp:::LdFlags()'  | $(R_HOME)/bin/R --vanilla --slave)
RCPPARMAINCL :=		$(shell echo 'RcppArmadillo:::CxxFlags()' | $(R_HOME)/bin/R --vanilla --slave)	


CXX := 			$(shell $(R_HOME)/bin/R CMD config CXX)
CPPFLAGS := 		-Wall $(shell $(R_HOME)/bin/R CMD config CPPFLAGS) -isystem $(GTEST)/include
CXXFLAGS := 		$(RCPPFLAGS) $(RCPPINCL) $(RCPPARMAINCL) $(RINSIDEINCL) $(shell $(R_HOME)/bin/R CMD config CXXFLAGS) -g -Wall -Wextra -pthread -fpic -c
LDLIBS := 		$(RLDFLAGS) $(RRPATH) $(RBLAS) $(RLAPACK) $(RCPPLIBS) $(RINSIDELIBS) 



all : hartigan.so

clean :
	rm *.o
	rm *.so

.PHONY : clean #R-standalone

#R-standalone : HartiganMaster.cpp
#	export PKG_LIBS=$(LINK_PATH)
#	export PKG_CXXFLAGS=$(INCL_PATH)
#	R CMD SHLIB $^

%.o : %.cpp
	g++ $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

hartigan.so : HartiganMaster.o
	g++ -shared -c Hartigan.cpp $^ -o $@ $(LDLIBS) $(CPPFLAGS) $(CXXFLAGS) 
