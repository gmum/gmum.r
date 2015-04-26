cmake_minimum_required(VERSION 2.6)
project(utils)

file (GLOB UTILS_SOURCES "${UTILS_SRC_DIR}/*.cpp" "${UTILS_SRC_DIR}/*.c")
file (GLOB UTILS_HEADERS "${INCLUDE_DIR}/${PROJECT_NAME}/*.h" "${INCLUDE_DIR}/${PROJECT_NAME}/*.hpp") 

add_library(utils ${UTILS_SOURCES} ${UTILS_HEADERS})
