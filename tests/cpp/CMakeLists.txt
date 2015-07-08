cmake_minimum_required(VERSION 2.6)
project(gmum_tests)

file (GLOB TEST_SOURCES 
    "${TESTS_DIR}/cec/*.cpp"
    "${TESTS_DIR}/gng/*.cpp"
    "${TESTS_DIR}/svm/*.cpp")

include_directories(${GMUM_INCLUDE})
include_directories(${R_INCLUDE_DIR} ${RCPP_INCLUDE_PATHS})
add_definitions(-DRCPP_INTERFACE -DTEST_DATA_PATH="${TEST_DATA_PATH}")

link_directories(${R_LIBRARY_PATH})
add_executable(gmum_tests ${TEST_SOURCES} ${GMUM_SOURCES})

target_link_libraries(gmum_tests gtest gtest_main pthread ${R_LIBRARY_DEPS})
set_target_properties(gmum_tests PROPERTIES OUTPUT_NAME "run_tests")
INSTALL(TARGETS gmum_tests DESTINATION ${TESTS_DIR})
