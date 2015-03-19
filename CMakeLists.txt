cmake_minimum_required(VERSION 2.6)
project(gmum.r)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/inst/include)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(CEC_SRC_DIR ${SRC_DIR}/cec)
set(GNG_SRC_DIR ${SRC_DIR}/gng)
set(SVM_SRC_DIR ${SRC_DIR}/svm)
set(SVM_LIGHT_SRC_DIR ${SRC_DIR}/svmlight)
set(UTILS_SRC_DIR ${SRC_DIR}/utils)
set(TESTS_DIR ${PROJECT_SOURCE_DIR}/tests/cpp)
set(DEP_LIBS lapack blas R pcre lzma bz2 z rt dl m)
set(DEP_LIBS_PATHS /usr/lib/R/lib)

include_directories(${INCLUDE_DIR} ${INCLUDE_DIR}/cec ${INCLUDE_DIR}/gng ${INCLUDE_DIR}/svm ${INCLUDE_DIR}/svmlight ${INCLUDE_DIR}/utils)
include_directories(/usr/share/R/include /usr/local/lib/R/site-library/Rcpp/include /usr/local/lib/R/site-library/RcppArmadillo/include /usr/local/lib/R/site-library/BH/include)

add_definitions(-DARMA_DONT_USE_CXX11 -DRCPP_INTERFACE)

add_subdirectory(${SRC_DIR})
add_subdirectory(${TESTS_DIR})
