/// svmlight_runner.cpp

#include <string>
#include <vector>

extern "C" {
    #include "includes/svmlight/svm_common.h"
    #include "includes/svmlight/svm_learn.h"
}

#include "svmlight_runner.h"
#include "utils.h"


SVMLightRunner::SVMLightRunner() {
}

SVMLightRunner::~SVMLightRunner() {
}

SVMResult SVMLightRunner::processRequest(
    SVMConfiguration data,
    SVMResult result
) {
    // TODO
    return result;
}

// NOTE: Empty!
bool SVMLightRunner::canHandle( SVMConfiguration data ) {
    // TODO
    return true;
}

/**
 *  processSVMLightFiles description.
 */
std::string SVMLightRunner::processSVMLightFiles(
    std::string training_file,
    std::string test_file
) {
    std::string results;

    librarySVMLearn(
        training_file + " " + SVMLightRunner_temp_model_file );
    librarySVMClassify(
        test_file + " " + SVMLightRunner_temp_model_file
        + " " + SVMLightRunner_temp_output_file );

    std::string printing_command = "cat";
    results = gmum::execute_command(
        printing_command + " " + SVMLightRunner_temp_output_file );

    std::string cleaning_command = "rm";
    gmum::execute_command(
        cleaning_command + " " + SVMLightRunner_temp_model_file
        + " " + SVMLightRunner_temp_output_file );

    return results;
}

/** @defgroup library Library methods/scripts wrappers
 *  Those methods are explicit library wrappers
 *  @{
 */

std::string SVMLightRunner::librarySVMLearn( std::string arguments ) {
    std::string command = "svm_learn ";
    command += arguments;
    return gmum::execute_command( command );
}

std::string SVMLightRunner::librarySVMClassify( std::string arguments ) {
    std::string command = "svm_classify ";
    command += arguments;
    return gmum::execute_command( command );
}

/**@}*/

SEXP svmlight_process_files(
    SEXP training_file,
    SEXP test_file
) {
    std::string training_file_str = Rcpp::as<std::string>(training_file);
    std::string test_file_str = Rcpp::as<std::string>(test_file);
    SVMLightRunner svmlight_runner;
    std::string results = svmlight_runner.processSVMLightFiles(
        training_file_str,
        test_file_str
    );
    //Rcpp::CharacterVector x = Rcpp::CharacterVector::create(results);
    std::vector< std::string > results_vector = gmum::split_lines(results);
    return Rcpp::wrap(results_vector);
}
