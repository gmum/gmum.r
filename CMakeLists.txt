cmake_minimum_required(VERSION 2.6)
project(gmum.r)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
find_package(R)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/inst/include)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(CEC_SRC_DIR ${SRC_DIR}/cec)
set(GNG_SRC_DIR ${SRC_DIR}/gng)
set(SVM_SRC_DIR ${SRC_DIR}/svm)
set(SVM_LIGHT_SRC_DIR ${SRC_DIR}/svmlight)
set(UTILS_SRC_DIR ${SRC_DIR}/utils)
set(TESTS_DIR ${PROJECT_SOURCE_DIR}/tests/cpp)
set(TEST_DATA_PATH ${PROJECT_SOURCE_DIR}/inst/data_sets)

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${R_LD_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${R_LD_FLAGS_ONLY_LIBS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${R_CXX_FLAGS}")

include_directories(${INCLUDE_DIR} ${INCLUDE_DIR}/cec ${INCLUDE_DIR}/gng ${INCLUDE_DIR}/svm ${INCLUDE_DIR}/svmlight ${INCLUDE_DIR}/utils)
include_directories(${R_INCLUDE_DIR})
include_directories(${RCPP_INCLUDE_PATHS})

add_definitions(-DARMA_DONT_USE_CXX11 -DRCPP_INTERFACE)
add_definitions(-DTEST_DATA_PATH="${TEST_DATA_PATH}")

add_subdirectory(${SRC_DIR})
add_subdirectory(${TESTS_DIR})
