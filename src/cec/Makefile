CFLAGS = -std=c++11 -fpic -c
INCL_PATH = /usr/lib/R/site-library/Rcpp/include:/usr/share/R/include
LINK_PATH = /usr/lib/R/site-library/Rcpp/libs:/usr/lib/R/lib
I_PATH = -I$(subst :, -I,$(INCL_PATH))
L_PATH = -L$(subst :, -L,$(LINK_PATH))

all : hartigan.so

clear :
	rm *.o
	rm *.so

.PHONY : R-standalone clear

R-standalone : hartigan.cpp
	export PKG_LIBS=$(LINK_PATH)
	export PKG_CXXFLAGS=$(INCL_PATH)
	R CMD SHLIB $^

%.o : %.cpp
	g++ $(I_PATH) $(CFLAGS) $^ -o $@

hartigan.so : hartigan.o
	g++ -shared $(L_PATH) $^ -o $@