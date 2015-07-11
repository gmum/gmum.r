cmake_minimum_required(VERSION 3.0)
project(gmum.r)

file(GLOB GMUMR_SOURCES 
    "${SRC_DIR}/cec/*.cpp" "${SRC_DIR}/cec/*.c"
    "${SRC_DIR}/svm/*.cpp" "${SRC_DIR}/svm/*.c"
    "${SRC_DIR}/gng/*.cpp" "${SRC_DIR}/gng/*.c"
    "${SRC_DIR}/svmlight/*.cpp" "${SRC_DIR}/svmlight/*.c"
    "${SRC_DIR}/utils/*.cpp" "${SRC_DIR}/utils/*.c")

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${R_LD_FLAGS}") 
set(CMAKE_CXX_FLAGS "${R_CXX_FLAGS} -std=c++0x -pthread -mtune=native -fpic -O2")

if(APPLE)
    set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
endif()

add_library(gmum.r SHARED ${GMUMR_SOURCES})
set_target_properties(gmum.r PROPERTIES PREFIX "")
INSTALL(TARGETS gmum.r DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})
