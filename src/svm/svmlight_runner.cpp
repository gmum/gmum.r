/**
 * @file        svmlight_runner.cpp
 * @brief       SVMLight implementation class of SVMHandler -- definitions
 * @author      Konrad Talik <konradtalik@gmail.com>
 * @copyright   GPLv3
 */


#include <iostream>
#include <string>
#include <vector>

#include "svmlight_runner.h"
#include "utils.h"


SVMLightRunner::SVMLightRunner() {
}

SVMLightRunner::~SVMLightRunner() {
}

void SVMLightRunner::processRequest( SVMConfiguration &conf ) {
    /// @todo Content
}

bool SVMLightRunner::canHandle( SVMConfiguration &conf ) {
    /// @todo Content
    return true;
}

std::string SVMLightRunner::processSVMLightFiles(
    std::string training_file,
    std::string test_file
) {
    std::string results;

    std::cout << "\n01\n";
    librarySVMLearn();
    //    training_file + " " + SVMLightRunner_temp_model_file );
    std::cout << "\n1\n";
    librarySVMClassify(
        test_file + " " + SVMLightRunner_temp_model_file
        + " " + SVMLightRunner_temp_output_file );
    std::cout << "\n2\n";

    std::string printing_command = "cat";
    results = gmum::execute_command(
        printing_command + " " + SVMLightRunner_temp_output_file );
    std::cout << "\n3\n";

    std::string cleaning_command = "rm";
    std::cout << "\n4\n";
    gmum::execute_command(
        cleaning_command + " " + SVMLightRunner_temp_model_file
        + " " + SVMLightRunner_temp_output_file );

    return results;
}

std::string SVMLightRunner::librarySVMLearn() {
    /*
    svm_learn_classification(
        docs, target, totdoc, totwords, &learn_parm, &kernel_parm,
        kernel_cache, model, alpha_in);
    */

    /*
    std::string command = "svm_learn ";
    command += arguments;
    return gmum::execute_command( command );
    */
    int argc = 0;
    char *argv = new char[2];

    std::cout << "[SVMLightRunner] Declaring docfile...\n";

    char *docfile = new char[200];
    std::string strdocfile = "../../data/svm/svmlight_training_example";
    docfile = (char *)strdocfile.c_str();

    std::cout << "[SVMLightRunner] Declaring model...\n";

    char *modelfile = new char[200];
    std::string strmodelfile = "gmumr_svmlight_model";
    modelfile = (char *)strmodelfile.c_str();

    std::cout << "[SVMLightRunner] Reading documents...\n";

    DOC **docs;  /* training examples */
    long totwords,totdoc,i;
    double *target;
    double *alpha_in=NULL;
    KERNEL_CACHE *kernel_cache;
    LEARN_PARM learn_parm;
    KERNEL_PARM kernel_parm;
    MODEL *model=(MODEL *)my_malloc(sizeof(MODEL));
    
    /* set default */
    //strcpy (modelfile, "svm_model");
    //strcpy (learn_parm.predfile, "trans_predictions");
    strcpy (learn_parm.alphafile, "");
    learn_parm.biased_hyperplane=1;
    learn_parm.sharedslack=0;
    learn_parm.remove_inconsistent=0;
    learn_parm.skip_final_opt_check=0;
    learn_parm.svm_maxqpsize=10;
    learn_parm.svm_newvarsinqp=0;
    learn_parm.svm_iter_to_shrink=-9999;
    learn_parm.maxiter=100000;
    learn_parm.kernel_cache_size=40;
    learn_parm.svm_c=0.0;
    learn_parm.eps=0.1;
    learn_parm.transduction_posratio=-1.0;
    learn_parm.svm_costratio=1.0;
    learn_parm.svm_costratio_unlab=1.0;
    learn_parm.svm_unlabbound=1E-5;
    learn_parm.epsilon_crit=0.001;
    learn_parm.epsilon_a=1E-15;
    learn_parm.compute_loo=0;
    learn_parm.rho=1.0;
    learn_parm.xa_depth=0;
    kernel_parm.kernel_type=0;
    kernel_parm.poly_degree=3;
    kernel_parm.rbf_gamma=1.0;
    kernel_parm.coef_lin=1;
    kernel_parm.coef_const=1;
    strcpy(kernel_parm.custom,"empty");
    learn_parm.type = CLASSIFICATION;

    /*
    read_input_parameters(argc,argv,docfile,modelfile,restartfile,&verbosity,
            &learn_parm,&kernel_parm);
    */

    read_documents(docfile,&docs,&target,&totwords,&totdoc);
    // if(restartfile[0]) alpha_in=read_alphas(restartfile,totdoc);

    if(kernel_parm.kernel_type == LINEAR) { /* don't need the cache */
    kernel_cache=NULL;
    }
    else {
    /* Always get a new kernel cache. It is not possible to use the
       same cache for two different training runs */
    kernel_cache=kernel_cache_init(totdoc,learn_parm.kernel_cache_size);
    }

    //

    if(learn_parm.type == CLASSIFICATION) {
    svm_learn_classification(docs,target,totdoc,totwords,&learn_parm,
                 &kernel_parm,kernel_cache,model,alpha_in);
    }
    else if(learn_parm.type == REGRESSION) {
    svm_learn_regression(docs,target,totdoc,totwords,&learn_parm,
             &kernel_parm,&kernel_cache,model);
    }
    else if(learn_parm.type == RANKING) {
    svm_learn_ranking(docs,target,totdoc,totwords,&learn_parm,
              &kernel_parm,&kernel_cache,model);
    }
    else if(learn_parm.type == OPTIMIZATION) {
    svm_learn_optimization(docs,target,totdoc,totwords,&learn_parm,
               &kernel_parm,kernel_cache,model,alpha_in);
    }

    if(kernel_cache) {
    /* Free the memory used for the cache. */
    kernel_cache_cleanup(kernel_cache);
    }

    //

    /* Warning: The model contains references to the original data 'docs'.
     If you want to free the original data, and only keep the model, you
     have to make a deep copy of 'model'. */
    /* deep_copy_of_model=copy_model(model); */
    write_model(modelfile,model);

    free(alpha_in);
    free_model(model,0);
    for(i=0;i<totdoc;i++)
    free_example(docs[i],1);
    free(docs);
    free(target);

    std::string result;

    return result;
}

int SVMLightRunner::librarySVMLearnMain(int argc, char *argv[]) {
    return 0;
}

std::string SVMLightRunner::librarySVMClassify( std::string arguments ) {
    std::string command = "svm_classify ";
    command += arguments;
    return gmum::execute_command( command );
}

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

std::vector < std::string > SVMLightRunner::test_svmlight_files() {

    std::string training_file_str = "../../data/svm/svmlight_training_example";
    std::string test_file_str = training_file_str;
    std::cout << "\n0\n";
    std::string results = processSVMLightFiles(
        training_file_str,
        test_file_str
    );
    std::vector< std::string > results_vector = gmum::split_lines(results);

    return results_vector;
}
