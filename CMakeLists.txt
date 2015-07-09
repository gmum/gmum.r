cmake_minimum_required(VERSION 2.6)
project(gmumr_project)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
find_package(R)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/inst/include CACHE PATH "Path to include directory")
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src CACHE PATH "Path to gmum.r library source files")
set(TESTS_DIR ${PROJECT_SOURCE_DIR}/tests/cpp CACHE PATH "Path to C++ tests directory")
set(TEST_DATA_PATH ${PROJECT_SOURCE_DIR}/inst/data_sets CACHE PATH "Path to test input data directory")
set(GMUM_INCLUDE ${INCLUDE_DIR} ${INCLUDE_DIR}/cec ${INCLUDE_DIR}/gng ${INCLUDE_DIR}/svm ${INCLUDE_DIR}/svmlight ${INCLUDE_DIR}/utils CACHE TYPE LIST)

add_subdirectory(${SRC_DIR})
add_subdirectory(${TESTS_DIR})
