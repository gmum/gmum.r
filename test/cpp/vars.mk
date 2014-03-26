OBJDIR = .obj/
CXXFLAGS +=-O2 -s -Wall -std=c++0x
GITIGNORE = ../.gitignore
INCL_PATH = /usr/lib/R/site-library/RcppArmadillo/include:/usr/lib/R/site-library/Rcpp/include:/usr/share/R/include
LINK_PATH = /usr/lib/R/site-library/RcppArmadillo/libs:/usr/lib/R/site-library/Rcpp/libs:/usr/lib/R/lib
I_PATH = -I$(subst :, -I,$(INCL_PATH))
L_PATH = -L$(subst :, -L,$(LINK_PATH))

define \n


endef
