cmake_minimum_required(VERSION 3.0)
project(gmumr_tests)

file (GLOB TEST_SOURCES 
    "${TESTS_DIR}/cec/*.cpp"
    "${TESTS_DIR}/gng/*.cpp"
    "${TESTS_DIR}/svm/*.cpp")
set(CMAKE_CXX_FLAGS "${R_CXX_FLAGS} -std=c++0x")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${R_LD_FLAGS}")

add_definitions(-DTEST_DATA_PATH="${TEST_DATA_PATH}")
add_executable(gmumr_tests ${TEST_SOURCES}) 
target_link_libraries(gmumr_tests gmum.r gtest gtest_main pthread)
