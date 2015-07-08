cmake_minimum_required(VERSION 2.6)
project(gmum.r)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
find_package(R)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/inst/include)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(TESTS_DIR ${PROJECT_SOURCE_DIR}/tests/cpp)
set(TEST_DATA_PATH ${PROJECT_SOURCE_DIR}/inst/data_sets)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${R_CXX_FLAGS} -std=c++0x")

file(GLOB GMUM_SOURCES 
    "${SRC_DIR}/cec/*.cpp" "${SRC_DIR}/cec/*.c"
    "${SRC_DIR}/svm/*.cpp" "${SRC_DIR}/svm/*.c"
    "${SRC_DIR}/gng/*.cpp" "${SRC_DIR}/gng/*.c"
    "${SRC_DIR}/svmlight/*.cpp" "${SRC_DIR}/svmlight/*.c"
    "${SRC_DIR}/utils/*.cpp" "${SRC_DIR}/utils/*.c")

set(GMUM_INCLUDE ${INCLUDE_DIR} ${INCLUDE_DIR}/cec ${INCLUDE_DIR}/gng ${INCLUDE_DIR}/svm ${INCLUDE_DIR}/svmlight ${INCLUDE_DIR}/utils)

add_subdirectory(${SRC_DIR})
add_subdirectory(${TESTS_DIR})
