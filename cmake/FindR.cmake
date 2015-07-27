# Defines the following:
#  R_INCLUDE_DIR       - Path to R include directories
#  R_CXX_FLAGS         - R library compiler flags (include directories)
#  R_LD_FLAGS          - R library linker flags 
#  R_LIBS              - R libraries
#  R_LIB_DIRS          - R library link directories

function(GetItemsWithNoPrefix resultVar prefix)
    set(result)
    foreach(ITR ${ARGN})
        if(NOT ITR MATCHES "^${prefix}.*")
            list(APPEND result ${ITR})
        endif()
    endforeach()
    set(${resultVar} ${result} PARENT_SCOPE)
endfunction()

function(GetItemsWithPrefix resultVar prefix)
    set(result)
    foreach(ITR ${ARGN})
        if(ITR MATCHES "^${prefix}.*")
            list(APPEND result ${ITR})
        endif()
    endforeach()
    set(${resultVar} ${result} PARENT_SCOPE)
endfunction()

function(RemovePrefix resultVar prefix)
    set(result)
    foreach(ITR ${ARGN})
        if(ITR MATCHES "^${prefix}.*")
            STRING(REGEX REPLACE "^${prefix}" "" ITR ${ITR})
        endif()
        list(APPEND result ${ITR})
    endforeach()
    set(${resultVar} ${result} PARENT_SCOPE)
endfunction()

set(TEMP_CMAKE_FIND_APPBUNDLE ${CMAKE_FIND_APPBUNDLE})
set(CMAKE_FIND_APPBUNDLE "NEVER")
find_program(R_COMMAND R DOC "R executable.")
set(CMAKE_FIND_APPBUNDLE ${TEMP_CMAKE_FIND_APPBUNDLE})

if(R_COMMAND)
    execute_process(WORKING_DIRECTORY .
        COMMAND ${R_COMMAND} RHOME 
        OUTPUT_VARIABLE R_ROOT_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(R_HOME ${R_ROOT_DIR} CACHE PATH "R home directory obtained from R RHOME")

    if(R_HOME)
        message("-- R_HOME found: ${R_HOME}")
    else(R_HOME)
        message(FATAL_ERROR "R_HOME not found" )
    endif(R_HOME)
   
    EXECUTE_PROCESS(
        COMMAND ${R_COMMAND} "--slave" "--no-save" "-e" "cat(R.home('include'))"
        OUTPUT_VARIABLE R_INCLUDE_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE)

    if(R_INCLUDE_DIR)
        message("-- R_INCLUDE_DIR found: ${R_INCLUDE_DIR}")
    else(R_INCLUDE_DIR)
        message(FATAL_ERROR "R_INCLUDE_DIR not found" )
    endif(R_INCLUDE_DIR)

    EXECUTE_PROCESS(
        COMMAND ${R_COMMAND} CMD config --ldflags
        OUTPUT_VARIABLE R_LD_FLAGS
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    
    if(NOT R_LD_FLAGS)
        message(FATAL_ERROR "R_LD_FLAGS not found" )
    endif(NOT R_LD_FLAGS)

    EXECUTE_PROCESS(
        COMMAND ${R_COMMAND} "CMD" "config" "LAPACK_LIBS"
        OUTPUT_VARIABLE R_LIBRARY_LAPACK
        OUTPUT_STRIP_TRAILING_WHITESPACE)

    if(R_LIBRARY_LAPACK)
        message("-- R_LIBRARY_LAPACK found: ${R_LIBRARY_LAPACK}")
    else(R_LIBRARY_LAPACK)
        message(FATAL_ERROR "R_LIBRARY_LAPACK not found" )
    endif(R_LIBRARY_LAPACK)

    EXECUTE_PROCESS(
        COMMAND ${R_COMMAND} "CMD" "config" "BLAS_LIBS"
        OUTPUT_VARIABLE R_LIBRARY_BLAS
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    
    if(R_LIBRARY_BLAS)
        message("-- R_LIBRARY_BLAS found: ${R_LIBRARY_BLAS}")
    else(R_LIBRARY_BLAS)
        message(FATAL_ERROR "R_LIBRARY_BLAS not found" )
    endif(R_LIBRARY_BLAS)

    set(R_LD_FLAGS "${R_LD_FLAGS} ${R_LIBRARY_LAPACK} ${R_LIBRARY_BLAS}")
    SEPARATE_ARGUMENTS(R_LD_FLAGS)

    GetItemsWithPrefix(R_LIBS "-l" ${R_LD_FLAGS}) 
    RemovePrefix(R_LIBS "-l" ${R_LIBS}) 
    message(STATUS "R_LIBS: ${R_LIBS}")
    GetItemsWithNoPrefix(R_LD_FLAGS "-l" ${R_LD_FLAGS}) 

    GetItemsWithPrefix(R_LIB_DIRS "-L" ${R_LD_FLAGS}) 
    RemovePrefix(R_LIB_DIRS "-L" ${R_LIB_DIRS})
    message(STATUS "R_LIB_DIRS: ${R_LIB_DIRS}")
    GetItemsWithNoPrefix(R_LD_FLAGS "-L" ${R_LD_FLAGS}) 

    string (REPLACE ";" " " R_LD_FLAGS "${R_LD_FLAGS}")

    EXECUTE_PROCESS(
        COMMAND ${R_COMMAND} --vanilla --slave -e "RcppArmadillo:::CxxFlags()"
        OUTPUT_VARIABLE RCPPARMADILLO_CXX_FLAGS
        OUTPUT_STRIP_TRAILING_WHITESPACE)

    if(RCPPARMADILLO_CXX_FLAGS)
        message("-- RCPPARMADILLO_CXX_FLAGS found: ${RCPPARMADILLO_CXX_FLAGS}")
    else(RCPPARMADILLO_CXX_FLAGS)
        message(FATAL_ERROR "RCPPARMADILLO_CXX_FLAGS not found" )
    endif(RCPPARMADILLO_CXX_FLAGS)

    EXECUTE_PROCESS(
        COMMAND ${R_COMMAND} --vanilla --slave -e "Rcpp:::CxxFlags()"
        OUTPUT_VARIABLE RCPP_CXX_FLAGS
        OUTPUT_STRIP_TRAILING_WHITESPACE)

    if(RCPP_CXX_FLAGS)
        message("-- RCPP_CXX_FLAGS found: ${RCPP_CXX_FLAGS}")
    else(RCPP_CXX_FLAGS)
        message(FATAL_ERROR "RCPP_CXX_FLAGS not found" )
    endif(RCPP_CXX_FLAGS)
 
    message(STATUS "R_LD_FLAGS: ${R_LD_FLAGS}")
    set(R_CXX_FLAGS "${RCPP_CXX_FLAGS} ${RCPPARMADILLO_CXX_FLAGS}" CACHE TYPE STRING)
    message(STATUS "R_CXX_FLAGS: ${R_CXX_FLAGS}")
else()
    message(SEND_ERROR "FindR.cmake requires the following variables to be set: R_COMMAND")
endif()
