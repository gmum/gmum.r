cmake_minimum_required(VERSION 2.6)
project(svm)

file (GLOB SVM_SOURCES "${SVM_SRC_DIR}/*.cpp" "${SVM_SRC_DIR}/*.c")
file (GLOB SVM_HEADERS "${INCLUDE_DIR}/${PROJECT_NAME}/*.h" "${INCLUDE_DIR}/${PROJECT_NAME}/*.hpp") 

add_library(svm ${SVM_SOURCES} ${SVM_HEADERS})
