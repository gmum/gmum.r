cmake_minimum_required(VERSION 2.6)
project(gng)

file (GLOB GNG_SOURCES "${GNG_SRC_DIR}/*.cpp" "${GNG_SRC_DIR}/*.c")
file (GLOB GNG_HEADERS "${INCLUDE_DIR}/${PROJECT_NAME}/*.h" "${INCLUDE_DIR}/${PROJECT_NAME}/*.hpp") 

add_library(gng ${GNG_SOURCES} ${GNG_HEADERS})
