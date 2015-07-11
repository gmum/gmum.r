# Defines the following:
#  R_COMMAND           - Path to R command
#  R_HOME              - Path to 'R home', as reported by R
#  R_INCLUDE_DIR       - Path to R include directories
#  R_CXX_FLAGS         - Rcpp and RcppArmadillo CXX flags (include directories)
#  R_LD_FLAGS          - -llib1 -llib2 -llib3 flags for linker used by R
#  RCPPARMADILLO_CXX_FLAGS - CXX flags required by RcppArmadillo

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
    
    if(R_LD_FLAGS)
        message("-- R_LD_FLAGS found: ${R_LD_FLAGS}")
    else(R_LD_FLAGS)
        message(FATAL_ERROR "R_LD_FLAGS not found" )
    endif(R_LD_FLAGS)
 
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

    set(R_LD_FLAGS "${R_LD_FLAGS} ${R_LIBRARY_LAPACK} ${R_LIBRARY_BLAS}" CACHE TYPE STRING)
    set(R_CXX_FLAGS "${RCPP_CXX_FLAGS} ${RCPPARMADILLO_CXX_FLAGS}" CACHE TYPE STRING)
else()
    message(SEND_ERROR "FindR.cmake requires the following variables to be set: R_COMMAND")
endif()
