cmake_minimum_required(VERSION 2.6)
project(gmum.r)

add_subdirectory(${CEC_SRC_DIR})
add_subdirectory(${GNG_SRC_DIR})
add_subdirectory(${SVM_SRC_DIR})
add_subdirectory(${SVM_LIGHT_SRC_DIR})
add_subdirectory(${UTILS_SRC_DIR})

link_directories(${DEP_LIBS_PATHS})

add_library(gmum.r SHARED root.cpp)
target_link_libraries(gmum.r cec gng svm svmlight utils ${DEP_LIBS})
INSTALL(TARGETS gmum.r DESTINATION ${SRC_DIR})
