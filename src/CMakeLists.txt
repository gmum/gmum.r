cmake_minimum_required(VERSION 2.6)
project(gmum_lib)

include_directories(${GMUM_INCLUDE})
include_directories(${R_INCLUDE_DIR} ${RCPP_INCLUDE_PATHS})
link_directories(${R_LIBRARY_PATH})
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${R_LD_FLAGS}") 
add_definitions(-DNDEBUG  -DNDEBUG_GMUM -DNDEBUG_GMUM_2 -DRCPP_INTERFACE -DARMA_64BIT_WORD)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread -pedantic -mtune=native -fpic -g -O2 -fstack-protector-strong -Wformat -Werror=format-security -D_FORTIFY_SOURCE=2 -g")

add_library(gmum.r SHARED ${GMUM_SOURCES})
target_link_libraries(gmum.r ${R_LIBRARY_DEPS})
set_target_properties(gmum.r PROPERTIES PREFIX "")
INSTALL(TARGETS gmum.r DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})
