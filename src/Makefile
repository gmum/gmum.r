CFLAGS = -fpic -c
INCL_PATH = /usr/lib/R/site-library/RcppArmadillo/include:/usr/lib/R/site-library/Rcpp/include:/usr/share/R/include
LINK_PATH = /usr/lib/R/site-library/RcppArmadillo/libs:/usr/lib/R/site-library/Rcpp/libs:/usr/lib/R/lib
I_PATH = -I$(subst :, -I,$(INCL_PATH))
L_PATH = -L$(subst :, -L,$(LINK_PATH))

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
	g++ $(I_PATH) $(CFLAGS) $^ -o $@

hartigan.so : HartiganMaster.o
	g++ -shared $(L_PATH) $^ -o $@