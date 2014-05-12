/**
 * @file        svmlight_runner.cpp
 * @brief       SVMLight implementation class of SVMHandler -- definitions
 * @author      Konrad Talik <konradtalik@gmail.com>
 * @copyright   MIT
 */



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

// Documented in the parent class
SVMResult SVMLightRunner::processRequest(
    SVMConfiguration data,
    SVMResult result
) {
    /// @todo Content
    return result;
}

// Documented in the parent class
bool SVMLightRunner::canHandle( SVMConfiguration data ) {
    /// @todo Content
    return true;
}

/**
 * This method is using training_file and test_file to perform one simple
 * SVMLight task: train and test. Result is an output of SVMLight's
 * `svm_classify` command.
 * @param   training_file   a file with a training data in SVMLight-familiar
 *                          format
 * @param   test_file       a file with a test data in SVMLight-familiar format
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

/** @name Library functionalities wrappers
 *  Below methods are direct library functionalities wrappers. They are
 *  designed to perform the same actions as corresponding command do.
 */
/// @{

/**
 * @param   arguments   command line arguments of `svm_learn` command
 */
std::string SVMLightRunner::librarySVMLearn( std::string arguments ) {
    std::string command = "svm_learn ";
    command += arguments;
    return gmum::execute_command( command );
}

/**
 * @param   arguments   command line arguments of `svm_classify` command
 */
std::string SVMLightRunner::librarySVMClassify( std::string arguments ) {
    std::string command = "svm_classify ";
    command += arguments;
    return gmum::execute_command( command );
}

/// @}

/**
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
}*/
