cmake_minimum_required(VERSION 2.6)
project(cec)
file (GLOB CEC_SOURCES "${CEC_SRC_DIR}/*.cpp" "${CEC_SRC_DIR}/*.c")
file (GLOB CEC_HEADERS "${INCLUDE_DIR}/${PROJECT_NAME}/*.h" "${INCLUDE_DIR}/${PROJECT_NAME}/*.hpp") 

add_library(cec ${CEC_SOURCES} ${CEC_HEADERS})
