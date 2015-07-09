cmake_minimum_required(VERSION 2.6)
project(gmumr_tests)

file (GLOB TEST_SOURCES 
    "${TESTS_DIR}/cec/*.cpp"
    "${TESTS_DIR}/gng/*.cpp"
    "${TESTS_DIR}/svm/*.cpp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

include_directories(${GMUM_INCLUDE} ${R_INCLUDE_DIR} ${RCPP_INCLUDE_PATHS})
add_definitions(-DRCPP_INTERFACE -DTEST_DATA_PATH="${TEST_DATA_PATH}")

link_directories(${R_LIBRARY_PATH})
add_executable(gmumr_tests ${TEST_SOURCES}) 
target_link_libraries(gmumr_tests gmum.r gtest gtest_main pthread ${R_LIBRARY_DEPS})
