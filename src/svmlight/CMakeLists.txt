cmake_minimum_required(VERSION 2.6)
project(svmlight)

file (GLOB SVM_LIGHT_SOURCES "${SVM_LIGHT_SRC_DIR}/*.cpp" "${SVM_LIGHT_SRC_DIR}/*.c")
file (GLOB SVM_LIGHT_HEADERS "${INCLUDE_DIR}/${PROJECT_NAME}/*.h" "${INCLUDE_DIR}/${PROJECT_NAME}/*.hpp") 

add_library(svmlight ${SVM_LIGHT_SOURCES} ${SVM_LIGHT_HEADERS})
