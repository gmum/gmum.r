library(testthat)

is.installed <- function(mypkg){
	is.element(mypkg, installed.packages()[,1])
}

if(!is.installed("klaR")){
	install.packages("klaR",repos='http://cran.us.r-project.org')
}

if(!is.installed("mlbench")){
  install.packages("mlbench",repos='http://cran.us.r-project.org')
}

if(!is.installed("rattle")){
	install.packages("rattle",repos='http://cran.us.r-project.org')
}

if(!is.installed("caret")){
	install.packages("caret",repos='http://cran.us.r-project.org')
}

if(!is.installed("e1071")){
	install.packages("e1071",repos='http://cran.us.r-project.org')
}

test_check('gmum.r')
