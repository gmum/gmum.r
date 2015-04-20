cmake_minimum_required(VERSION 2.6)
project(gmum_tests)

file (GLOB TEST_SOURCES 
    "${TESTS_DIR}/cec/*.cpp"
    "${TESTS_DIR}/gng/*.cpp"
    "${TESTS_DIR}/svm/*.cpp")

file (GLOB GMUM_SOURCES 
    "${CEC_SRC_DIR}/*.cpp" "${CEC_SRC_DIR}/*.c"
    "${SVM_SRC_DIR}/*.cpp" "${SVM_SRC_DIR}/*.c"
    "${GNG_SRC_DIR}/*.cpp" "${GNG_SRC_DIR}/*.c"
    "${SVM_LIGHT_SRC_DIR}/*.cpp" "${SVM_LIGHT_SRC_DIR}/*.c"
    "${UTILS_SRC_DIR}/*.cpp" "${UTILS_SRC_DIR}/*.c")

link_directories(${R_LIBRARY_PATH})

add_executable(${PROJECT_NAME} ${GMUM_SOURCES} ${TEST_SOURCES})
target_link_libraries(${PROJECT_NAME} gtest gtest_main pthread ${R_LIBRARY_DEPS})
INSTALL(TARGETS gmum_tests DESTINATION ${TESTS_DIR})
