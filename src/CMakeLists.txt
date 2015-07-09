cmake_minimum_required(VERSION 2.6)
project(gmum.r)

file(GLOB GMUMR_SOURCES 
    "${SRC_DIR}/cec/*.cpp" "${SRC_DIR}/cec/*.c"
    "${SRC_DIR}/svm/*.cpp" "${SRC_DIR}/svm/*.c"
    "${SRC_DIR}/gng/*.cpp" "${SRC_DIR}/gng/*.c"
    "${SRC_DIR}/svmlight/*.cpp" "${SRC_DIR}/svmlight/*.c"
    "${SRC_DIR}/utils/*.cpp" "${SRC_DIR}/utils/*.c")

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${R_LD_FLAGS}") 
set(CMAKE_CXX_FLAGS "${R_CXX_FLAGS} -std=c++0x -Wall -pthread -pedantic -mtune=native -fpic -O2 -fstack-protector-strong -Wformat -Werror=format-security -D_FORTIFY_SOURCE=2")
add_definitions(-DNDEBUG -DNDEBUG_GMUM -DNDEBUG_GMUM_2 -DRCPP_INTERFACE -DARMA_64BIT_WORD)

include_directories(${GMUM_INCLUDE})
include_directories(${R_INCLUDE_DIR} ${RCPP_INCLUDE_PATHS})
link_directories(${R_LIBRARY_PATH})

add_library(gmum.r SHARED ${GMUMR_SOURCES})
target_link_libraries(gmum.r ${R_LIBRARY_DEPS})
set_target_properties(gmum.r PROPERTIES PREFIX "")
INSTALL(TARGETS gmum.r DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})
