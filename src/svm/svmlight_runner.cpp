/**
 * @file        svmlight_runner.cpp
 * @brief       SVMLight implementation class of SVMHandler -- definitions
 * @author      Konrad Talik <konradtalik@gmail.com>
 * @copyright   GPLv3
 */


#include <string.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cassert>

#include "svmlight_runner.h"
#include "svm/log.h"
#include "svm_basic.h"

const std::string __file__ = "svmlight_runner.cpp";
const std::string __runner_class__ = "SVMLightRunner";
const std::string __debug_prefix__ = __file__ + "." + __runner_class__;


SVMLightRunner::SVMLightRunner() {
}


SVMLightRunner::~SVMLightRunner() {
    _clear();
}


bool SVMLightRunner::canHandle(SVMConfiguration &config) {
    /// @TODO SVMLightRunner::canHandle
    return true;
}


void SVMLightRunner::processRequest(SVMConfiguration &config) {
    char **argv;

    arma::mat unique_labels = arma::unique(config.target);
    config.neg_target = unique_labels[0];
    config.pos_target = unique_labels[1];

    if (!config.isPrediction()) {
        // Learning
        librarySVMLearnMain(0, argv, true, config);

    } else {
        // Predict
        librarySVMClassifyMain(0, argv, true, config);
        // Convert sign to label
        resultsToLabels(config);
    }
}


void SVMLightRunner::resultsToLabels(SVMConfiguration &config) {
    LOG(
        config.log,
        LogLevel::DEBUG,
        __debug_prefix__ + ".resultsToLabels() Started."
    );

    size_t n_docs = config.result.n_rows;
    double doc_result = 0;
    for (int i=0; i < n_docs; ++i) {
        doc_result = config.result[i];

        // Store only a class label
        //arma::vec doc_result_vec;
        //doc_result_vec << doc_result << arma::endr;
        //arma::vec result_sign_vec = arma::sign(doc_result_vec);
        //doc_result = result_sign_vec[0];

        // Store user-defined label
        if (doc_result < 0) {
            config.result[i] = config.neg_target;
        } else if (doc_result > 0) {
            config.result[i] = config.pos_target;
        } else {
            config.result[i] = 0;
        }
    }

    LOG(
        config.log,
        LogLevel::DEBUG,
        __debug_prefix__ + ".resultsToLabels() Done."
    );
}


void SVMLightRunner::_clear() {
    extern double *primal;
    primal = 0;
    extern double *dual;
    dual = 0;
    extern long precision_violations;
    precision_violations = 0;
    extern double *buffer;
    buffer = 0;
    extern long *nonoptimal;
    nonoptimal = 0;
    extern long smallroundcount;
    smallroundcount = 0;
    extern long roundnumber;
    roundnumber = 0;
    extern long kernel_cache_statistic;
    kernel_cache_statistic = 0;
}


/* Library functionalities wrappers */

int SVMLightRunner::librarySVMLearnMain(
    int argc, char **argv, bool use_gmumr, SVMConfiguration &config
) {
    LOG(
        config.log,
        LogLevel::DEBUG,
        __debug_prefix__ + ".librarySVMLearnMain() Started."
    );
    DOC **docs;  /* training examples */
    long totwords,totdoc,i;
    double *target;
    double *alpha_in=NULL;
    KERNEL_CACHE *kernel_cache;
    LEARN_PARM learn_parm;
    KERNEL_PARM kernel_parm;
    MODEL *model=(MODEL *)my_malloc(sizeof(MODEL));

    // GMUM.R changes {
    librarySVMLearnReadInputParameters(
        argc, argv, docfile, modelfile, restartfile, &verbosity, &learn_parm,
        &kernel_parm, use_gmumr, config
    );

    kernel_parm.kernel_type = static_cast<long int>(config.kernel_type);

    libraryReadDocuments(
        docfile, &docs, &target, &totwords, &totdoc, use_gmumr, config
    );
    // GMUM.R changes }

    if(restartfile[0]) alpha_in=read_alphas(restartfile,totdoc);

    if(kernel_parm.kernel_type == LINEAR) { /* don't need the cache */
      kernel_cache=NULL;
    }
    else {
      /* Always get a new kernel cache. It is not possible to use the
       * same cache for two different training runs */
      kernel_cache=kernel_cache_init(totdoc,learn_parm.kernel_cache_size);
    }

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

    /* Warning: The model contains references to the original data 'docs'.
       If you want to free the original data, and only keep the model, you 
       have to make a deep copy of 'model'. */
    /* deep_copy_of_model=copy_model(model); */
    // GMUM.R changes {
    if (!use_gmumr) {
        write_model(modelfile,model);
    } else {
        SVMLightModelToSVMConfiguration(model, config);
    }
    // GMUM.R changes }

    free(alpha_in);
    free_model(model,0);
    for(i=0;i<totdoc;i++) 
      free_example(docs[i],1);
    free(docs);
    free(target);

    LOG(
        config.log,
        LogLevel::DEBUG,
        __debug_prefix__ + ".librarySVMLearnMain() Done."
    );

    return(0);
}


void SVMLightRunner::librarySVMLearnReadInputParameters(
    int argc, char *argv[], char *docfile, char *modelfile, char *restartfile,
    long *verbosity, LEARN_PARM *learn_parm, KERNEL_PARM *kernel_parm,
    bool use_gmumr, SVMConfiguration &config
) {
    LOG(
        config.log,
        LogLevel::DEBUG,
        __debug_prefix__ + ".librarySVMClassifyReadInputParameters() Started."
    );

    long i;
    char type[100];

    // GMUM.R changes {
    /* set default */
    strcpy (modelfile, "svm_model");
    strcpy (learn_parm->predfile, "trans_predictions");
    strcpy (learn_parm->alphafile, "");
    strcpy (restartfile, "");
    // SVMLight verbosity = 1 corresponds to GMUM.R verbosity = 5 (DEBUG)
    (*verbosity)=config.log.verbosity - 4;
    learn_parm->biased_hyperplane=1;
    learn_parm->sharedslack=0;
    learn_parm->remove_inconsistent=0;
    learn_parm->skip_final_opt_check=0;
    learn_parm->svm_maxqpsize=10;
    learn_parm->svm_newvarsinqp=0;
    learn_parm->svm_iter_to_shrink=-9999;
    learn_parm->kernel_cache_size=40;
    /* upper bound C on alphas */
    learn_parm->svm_c=config.C;
    learn_parm->eps=0.1;
    learn_parm->transduction_posratio=-1.0;
    /* factor to multiply C for positive examples */
    learn_parm->svm_costratio=1.0;

    /* FIXME: Find what this one does */
    learn_parm->svm_costratio_unlab=1.0;
    learn_parm->svm_unlabbound=1E-5;
    learn_parm->epsilon_crit=0.001;
    learn_parm->epsilon_a=1E-15;
    learn_parm->compute_loo=0;
    learn_parm->rho=1.0;
    learn_parm->xa_depth=0;
    kernel_parm->kernel_type=0;
    kernel_parm->poly_degree=3;
    kernel_parm->rbf_gamma=1.0;
    kernel_parm->coef_lin=1;
    kernel_parm->coef_const=1;
    strcpy(kernel_parm->custom,"empty");
    strcpy(type,"c");

    if (config.max_iter == -1)
        learn_parm->maxiter = 100000;
    else
        learn_parm->maxiter = config.max_iter;

    if (static_cast<long int>(config.kernel_type) == 3) {
        // sigmoid tanh(s a*b + c)
        // s = 1.0/highest_feature_index
        kernel_parm->coef_lin = 1.0/config.getDataDim();
        // c
        kernel_parm->coef_const = -1.0;
    }

    /* set userdefined */
    if (config.degree){
        kernel_parm->poly_degree = config.degree;
    }
    if (config.gamma){
        kernel_parm->rbf_gamma = config.gamma;
    }
    if (config.gamma){
        kernel_parm->coef_lin = config.gamma;
    }
    if (config.coef0){
        kernel_parm->coef_const = config.coef0;
    }

    //This is tricky - both LIBSVM and SVMLIGHT have same default eps.
    //However in general we should do things like that
    learn_parm->epsilon_crit=config.eps;


    // GMUM.R changes }
    if(!use_gmumr) {
        if(i>=argc) {
            printf("\nNot enough input parameters!\n\n");
            libraryWaitAnyKey();
            libraryPrintHelp();
            exit(0);
        }
        strcpy (docfile, argv[i]);
    }
    // GMUM.R changes }
    if((i+1)<argc) {
      strcpy (modelfile, argv[i+1]);
    }
    if(learn_parm->svm_iter_to_shrink == -9999) {
      if(kernel_parm->kernel_type == LINEAR) 
        learn_parm->svm_iter_to_shrink=2;
      else
        learn_parm->svm_iter_to_shrink=100;
    }
    if(strcmp(type,"c")==0) {
      learn_parm->type=CLASSIFICATION;
    }
    else if(strcmp(type,"r")==0) {
      learn_parm->type=REGRESSION;
    }
    else if(strcmp(type,"p")==0) {
      learn_parm->type=RANKING;
    }
    else if(strcmp(type,"o")==0) {
      learn_parm->type=OPTIMIZATION;
    }
    else if(strcmp(type,"s")==0) {
      learn_parm->type=OPTIMIZATION;
      learn_parm->sharedslack=1;
    }
    else {
      printf("\nUnknown type '%s': Valid types are 'c' (classification), 'r' regession, and 'p' preference ranking.\n",type);
      libraryWaitAnyKey();
      libraryPrintHelp();
      exit(0);
    }    
    if((learn_parm->skip_final_opt_check) 
       && (kernel_parm->kernel_type == LINEAR)) {
      printf("\nIt does not make sense to skip the final optimality check for linear kernels.\n\n");
      learn_parm->skip_final_opt_check=0;
    }    
    if((learn_parm->skip_final_opt_check) 
       && (learn_parm->remove_inconsistent)) {
      printf("\nIt is necessary to do the final optimality check when removing inconsistent \nexamples.\n");
      libraryWaitAnyKey();
      libraryPrintHelp();
      exit(0);
    }    
    if((learn_parm->svm_maxqpsize<2)) {
      printf("\nMaximum size of QP-subproblems not in valid range: %ld [2..]\n",learn_parm->svm_maxqpsize); 
      libraryWaitAnyKey();
      libraryPrintHelp();
      exit(0);
    }
    if((learn_parm->svm_maxqpsize<learn_parm->svm_newvarsinqp)) {
      printf("\nMaximum size of QP-subproblems [%ld] must be larger than the number of\n",learn_parm->svm_maxqpsize); 
      printf("new variables [%ld] entering the working set in each iteration.\n",learn_parm->svm_newvarsinqp); 
      libraryWaitAnyKey();
      libraryPrintHelp();
      exit(0);
    }
    if(learn_parm->svm_iter_to_shrink<1) {
      printf("\nMaximum number of iterations for shrinking not in valid range: %ld [1,..]\n",learn_parm->svm_iter_to_shrink);
      libraryWaitAnyKey();
      libraryPrintHelp();
      exit(0);
    }
    if(learn_parm->svm_c<0) {
      printf("\nThe C parameter must be greater than zero!\n\n");
      libraryWaitAnyKey();
      libraryPrintHelp();
      exit(0);
    }
    if(learn_parm->transduction_posratio>1) {
      printf("\nThe fraction of unlabeled examples to classify as positives must\n");
      printf("be less than 1.0 !!!\n\n");
      libraryWaitAnyKey();
      libraryPrintHelp();
      exit(0);
    }
    if(learn_parm->svm_costratio<=0) {
      printf("\nThe COSTRATIO parameter must be greater than zero!\n\n");
      libraryWaitAnyKey();
      libraryPrintHelp();
      exit(0);
    }
    if(learn_parm->epsilon_crit<=0) {
      printf("\nThe epsilon parameter must be greater than zero!\n\n");
      libraryWaitAnyKey();
      libraryPrintHelp();
      exit(0);
    }
    if(learn_parm->rho<0) {
      printf("\nThe parameter rho for xi/alpha-estimates and leave-one-out pruning must\n");
      printf("be greater than zero (typically 1.0 or 2.0, see T. Joachims, Estimating the\n");
      printf("Generalization Performance of an SVM Efficiently, ICML, 2000.)!\n\n");
      libraryWaitAnyKey();
      libraryPrintHelp();
      exit(0);
    }
    if((learn_parm->xa_depth<0) || (learn_parm->xa_depth>100)) {
      printf("\nThe parameter depth for ext. xi/alpha-estimates must be in [0..100] (zero\n");
      printf("for switching to the conventional xa/estimates described in T. Joachims,\n");
      printf("Estimating the Generalization Performance of an SVM Efficiently, ICML, 2000.)\n");
      libraryWaitAnyKey();
      libraryPrintHelp();
      exit(0);
    }
}




int SVMLightRunner::librarySVMClassifyMain(
    int argc, char **argv, bool use_gmumr, SVMConfiguration &config
) {
    LOG(
        config.log,
        LogLevel::DEBUG,
        __debug_prefix__ + ".librarySVMClassifyMain() Started."
    );
    DOC *doc;   /* test example */
    WORD *words;
    long max_docs,max_words_doc,lld;
    long totdoc=0,queryid,slackid;
    long correct=0,incorrect=0,no_accuracy=0;
    long res_a=0,res_b=0,res_c=0,res_d=0,wnum,pred_format;
    long j;
    double t1,runtime=0;
    double dist,doc_label,costfactor;
    char *line,*comment; 
    FILE *predfl,*docfl;
    MODEL *model; 

    // GMUM.R changes {
    librarySVMClassifyReadInputParameters(
        argc, argv, docfile, modelfile, predictionsfile, &verbosity,
        &pred_format, use_gmumr, config);

    if (!use_gmumr) {
        nol_ll(docfile,&max_docs,&max_words_doc,&lld); /* scan size of input file */
        lld+=2;

        line = (char *)my_malloc(sizeof(char)*lld);
    } else {
        max_docs = config.target.n_rows;
        max_words_doc = config.getDataDim();
        config.result = arma::zeros<arma::vec>(max_docs);
        // Prevent writing to the file
        pred_format = -1;
        // lld used only for file reading
    }
    max_words_doc+=2;
    words = (WORD *)my_malloc(sizeof(WORD)*(max_words_doc+10));
    // GMUM.R changes }

    model=libraryReadModel(modelfile, use_gmumr, config);
    // GMUM.R changes }

    if(model->kernel_parm.kernel_type == 0) { /* linear kernel */
      /* compute weight vector */
      add_weight_vector_to_linear_model(model);
    }
    
    if(verbosity>=2) {
      printf("Classifying test examples.."); fflush(stdout);
    }

    // GMUM.R changes {
    bool newline;
    if (!use_gmumr) {
        if ((predfl = fopen (predictionsfile, "w")) == NULL)
        { perror (predictionsfile); exit (1); }
        if ((docfl = fopen (docfile, "r")) == NULL)
        { perror (docfile); exit (1); }

        newline = (!feof(docfl)) && fgets(line,(int)lld,docfl);
    } else {
        newline = false;
        if (totdoc < config.getDataExamplesNumber()) {
            newline = true;
            std::string str = SVMConfigurationToSVMLightLearnInputLine(config, totdoc);
            line = new char[str.size() + 1];
            std::copy(str.begin(), str.end(), line);
            line[str.size()] = '\0';
        }
    }
    while(newline) {
      if (use_gmumr) {
            std::string stringline = "";
      }
      // GMUM.R changes }
      if(line[0] == '#') continue;  /* line contains comments */
      parse_document(line,words,&doc_label,&queryid,&slackid,&costfactor,&wnum,
             max_words_doc,&comment);
      totdoc++;
      if(model->kernel_parm.kernel_type == 0) {   /* linear kernel */
        for(j=0;(words[j]).wnum != 0;j++) {  /* Check if feature numbers   */
      if((words[j]).wnum>model->totwords) /* are not larger than in     */
        (words[j]).wnum=0;               /* model. Remove feature if   */
        }                                        /* necessary.                 */
        doc = create_example(-1,0,0,0.0,create_svector(words,comment,1.0));
        t1=get_runtime();
        dist=classify_example_linear(model,doc);
        runtime+=(get_runtime()-t1);
        free_example(doc,1);
      }
      else {                             /* non-linear kernel */
        doc = create_example(-1,0,0,0.0,create_svector(words,comment,1.0));
        t1=get_runtime();
        dist=classify_example(model,doc);
        runtime+=(get_runtime()-t1);
        free_example(doc,1);
      }
      if(dist>0) {
        if(pred_format==0) { /* old weired output format */
      fprintf(predfl,"%.8g:+1 %.8g:-1\n",dist,-dist);
        }
        if(doc_label>0) correct++; else incorrect++;
        if(doc_label>0) res_a++; else res_b++;
      }
      else {
        if(pred_format==0) { /* old weired output format */
      fprintf(predfl,"%.8g:-1 %.8g:+1\n",-dist,dist);
        }
        if(doc_label<0) correct++; else incorrect++;
        if(doc_label>0) res_c++; else res_d++;
      }
      if(pred_format==1) { /* output the value of decision function */
        fprintf(predfl,"%.8g\n",dist);
      }
      if((int)(0.01+(doc_label*doc_label)) != 1)
        { no_accuracy=1; } /* test data is not binary labeled */
      if(verbosity>=2) {
        if(totdoc % 100 == 0) {
      printf("%ld..",totdoc); fflush(stdout);
        }
      }
      // GMUM.R changes {
      if (!use_gmumr) {
          newline = (!feof(docfl)) && fgets(line,(int)lld,docfl);
      } else {
          newline = false;
          // Store prediction result in config
          config.result[totdoc-1] = dist;
          // Read next line
          if (totdoc < config.getDataExamplesNumber()) {
              newline = true;
              std::string str = SVMConfigurationToSVMLightLearnInputLine(config, totdoc);
              line = new char[str.size() + 1];
              std::copy(str.begin(), str.end(), line);
              line[str.size()] = '\0';
          }
      }
    }
    if (!use_gmumr) {
        fclose(predfl);
        fclose(docfl);
        free(line);
    }
    // GMUM.R changes }
    free(words);
    free_model(model,1);

    if(verbosity>=2) {
      printf("done\n");

  /*   Note by Gary Boone                     Date: 29 April 2000        */
  /*      o Timing is inaccurate. The timer has 0.01 second resolution.  */
  /*        Because classification of a single vector takes less than    */
  /*        0.01 secs, the timer was underflowing.                       */
      printf("Runtime (without IO) in cpu-seconds: %.2f\n",
         (float)(runtime/100.0));

    }
    if((!no_accuracy) && (verbosity>=1)) {
      printf("Accuracy on test set: %.2f%% (%ld correct, %ld incorrect, %ld total)\n",(float)(correct)*100.0/totdoc,correct,incorrect,totdoc);
      printf("Precision/recall on test set: %.2f%%/%.2f%%\n",(float)(res_a)*100.0/(res_a+res_b),(float)(res_a)*100.0/(res_a+res_c));
    }

    return(0);
}


void SVMLightRunner::librarySVMClassifyReadInputParameters(
    int argc, char **argv, char *docfile, char *modelfile,
    char *predictionsfile, long int *verbosity, long int *pred_format,
    bool use_gmumr, SVMConfiguration &config
) {
    LOG(
        config.log,
        LogLevel::DEBUG,
        __debug_prefix__ + ".librarySVMClassifyReadInputParameters() Started."
    );
    long i;

    // GMUM.R changes {
    if (!use_gmumr) {
        /* set default */
        strcpy (modelfile, "svm_model");
        strcpy (predictionsfile, "svm_predictions"); 
        (*verbosity)=2;
    } else {
        // SVMLight verbosity = 1 corresponds to GMUM.R verbosity = 5 (DEBUG)
        (*verbosity) = config.log.verbosity - 4;
    }
    // GMUM.R changes }
    (*pred_format)=1;

    for(i=1;(i<argc) && ((argv[i])[0] == '-');i++) {
      switch ((argv[i])[1]) 
        { 
        case 'h': libraryPrintHelp(); exit(0);
        case 'v': i++; (*verbosity)=atol(argv[i]); break;
        case 'f': i++; (*pred_format)=atol(argv[i]); break;
        default: printf("\nUnrecognized option %s!\n\n",argv[i]);
             libraryPrintHelp();
             exit(0);
        }
    }
    // GMUM.R changes {
    if(!use_gmumr) {
        if((i+1)>=argc) {
          printf("\nNot enough input parameters!\n\n");
          libraryPrintHelp();
          exit(0);
        }
        strcpy (docfile, argv[i]);
        strcpy (modelfile, argv[i+1]);
        if((i+2)<argc) {
          strcpy (predictionsfile, argv[i+2]);
        }
    }
    // GMUM.R changes }
    if(((*pred_format) != 0) && ((*pred_format) != 1)) {
      printf("\nOutput format can only take the values 0 or 1!\n\n");
      libraryPrintHelp();
      exit(0);
    }
}

MODEL * SVMLightRunner::libraryReadModel(
    char *modelfile, bool use_gmumr, SVMConfiguration &config
) {
    LOG(
        config.log,
        LogLevel::DEBUG,
        __debug_prefix__ + ".libraryReadModel() Started."
    );
    FILE *modelfl;
    long i,queryid,slackid;
    double costfactor;
    long max_sv,max_words,ll,wpos;
    char *line,*comment;
    WORD *words;
    char version_buffer[100];
    MODEL *model;

    if(verbosity>=1) {
      printf("Reading model..."); fflush(stdout);
    }

    // GMUM.R changes {
    model = (MODEL *)my_malloc(sizeof(MODEL));

    if (!use_gmumr) {
        nol_ll(modelfile,&max_sv,&max_words,&ll); /* scan size of model file */
        max_words+=2;
        ll+=2;

        words = (WORD *)my_malloc(sizeof(WORD)*(max_words+10));
        line = (char *)my_malloc(sizeof(char)*ll);

        if ((modelfl = fopen (modelfile, "r")) == NULL)
        { perror (modelfile); exit (1); }

        fscanf(modelfl,"SVM-light Version %s\n",version_buffer);
        if(strcmp(version_buffer,VERSION)) {
          perror ("Version of model-file does not match version of svm_classify!"); 
          exit (1); 
        }
        fscanf(modelfl,"%ld%*[^\n]\n", &model->kernel_parm.kernel_type);  
        fscanf(modelfl,"%ld%*[^\n]\n", &model->kernel_parm.poly_degree);
        fscanf(modelfl,"%lf%*[^\n]\n", &model->kernel_parm.rbf_gamma);
        fscanf(modelfl,"%lf%*[^\n]\n", &model->kernel_parm.coef_lin);
        fscanf(modelfl,"%lf%*[^\n]\n", &model->kernel_parm.coef_const);
        fscanf(modelfl,"%[^#]%*[^\n]\n", model->kernel_parm.custom);

        fscanf(modelfl,"%ld%*[^\n]\n", &model->totwords);
        fscanf(modelfl,"%ld%*[^\n]\n", &model->totdoc);
        fscanf(modelfl,"%ld%*[^\n]\n", &model->sv_num);
        fscanf(modelfl,"%lf%*[^\n]\n", &model->b);
    } else { // use_gmumr
        max_words = config.getDataDim();
        words = (WORD *)my_malloc(sizeof(WORD)*(max_words+10));

        LOG(
            config.log,
            LogLevel::DEBUG,
            __debug_prefix__ + ".libraryReadModel() Converting config to model..."
        );

        /* 0=linear, 1=poly, 2=rbf, 3=sigmoid, 4=custom -- same as GMUM.R! */
        model->kernel_parm.kernel_type = static_cast<long int>(config.kernel_type);
        // -d int      -> parameter d in polynomial kernel
        model->kernel_parm.poly_degree = config.degree;
        // -g float    -> parameter gamma in rbf kernel
        model->kernel_parm.rbf_gamma = config.gamma;
        // -s float    -> parameter s in sigmoid/poly kernel
        model->kernel_parm.coef_lin = config.gamma;
        // -r float    -> parameter c in sigmoid/poly kernel
        model->kernel_parm.coef_const = config.coef0;
        // -u string   -> parameter of user defined kernel
        char kernel_parm_custom[50] = "empty";
        char * model_kernel_parm_custom = model->kernel_parm.custom;
        model_kernel_parm_custom = kernel_parm_custom;
        // highest feature index
        model->totwords = config.getDataDim();
        // number of training documents
        model->totdoc = config.target.n_rows;
        // number of support vectors plus 1 (!)
        model->sv_num = config.l + 1;
        /* Threshold b (has opposite sign than SVMClient::predict())
         * In svm_common.c:57 in double classify_example_linear():
         *     return(sum-model->b);
         */
        model->b = - config.b;

        LOG(
            config.log,
            LogLevel::DEBUG,
            __debug_prefix__ + ".libraryReadModel() Converting config done."
        );
    }
    // GMUM.R changes }

    model->supvec = (DOC **)my_malloc(sizeof(DOC *)*model->sv_num);
    model->alpha = (double *)my_malloc(sizeof(double)*model->sv_num);
    model->index=NULL;
    model->lin_weights=NULL;

    // GMUM.R changes {
    if (!use_gmumr) {
        for(i=1;i<model->sv_num;i++) {
          fgets(line,(int)ll,modelfl);
          if(!parse_document(line,words,&(model->alpha[i]),&queryid,&slackid,
                     &costfactor,&wpos,max_words,&comment)) {
            printf("\nParsing error while reading model file in SV %ld!\n%s",
               i,line);
            exit(1);
          }
          model->supvec[i] = create_example(-1,
                            0,0,
                            0.0,
                            create_svector(words,comment,1.0));
        }
        fclose(modelfl);
        free(line);
    } else {
        for(i = 1; i < model->sv_num; ++i) {
            line = SVMConfigurationToSVMLightModelSVLine(config, i-1);
            if(!parse_document(line,words,&(model->alpha[i]),&queryid,&slackid,
                       &costfactor,&wpos,max_words,&comment)) {
              printf("\nParsing error while reading model file in SV %ld!\n%s",
                 i,line);
              exit(1);
            }
            model->supvec[i] = create_example(-1,
                              0,0,
                              0.0,
                              create_svector(words,comment,1.0));
        }
    }
    // GMUM.R changes }
    free(words);
    if(verbosity>=1) {
      fprintf(stdout, "OK. (%d support vectors read)\n",(int)(model->sv_num-1));
    }

    LOG(
        config.log,
        LogLevel::DEBUG,
        __debug_prefix__ + ".libraryReadModel() Done."
    );

    return(model);
}

void SVMLightRunner::libraryReadDocuments (
    char *docfile, DOC ***docs, double **label, long int *totwords,
    long int *totdoc, bool use_gmumr, SVMConfiguration &config
) {
    LOG(
        config.log,
        LogLevel::DEBUG,
        __debug_prefix__ + ".libraryReadDocuments() Started."
    );

    char *line,*comment;
    WORD *words;
    long dnum=0,wpos,dpos=0,dneg=0,dunlab=0,queryid,slackid,max_docs;
    long max_words_doc, ll;
    double doc_label,costfactor;
    FILE *docfl;

    if(verbosity>=1) {
      printf("Scanning examples..."); fflush(stdout);
    }
    // GMUM.R changes {
    if (!use_gmumr) {
        nol_ll(docfile,&max_docs,&max_words_doc,&ll); /* scan size of input file */
    } else {
        max_docs = config.target.n_rows;
        max_words_doc = config.getDataDim();
        // ll used only for file reading
    }
    // GMUM.R changes }
    max_words_doc+=2;
    ll+=2;
    max_docs+=2;
    if(verbosity>=1) {
      printf("done\n"); fflush(stdout);
    }

    (*docs) = (DOC **)my_malloc(sizeof(DOC *)*max_docs);    /* feature vectors */
    (*label) = (double *)my_malloc(sizeof(double)*max_docs); /* target values */
    // GMUM.R changes {
    if (!use_gmumr) {
        line = (char *)my_malloc(sizeof(char)*ll);

        if ((docfl = fopen (docfile, "r")) == NULL)
        { perror (docfile); exit (1); }
    }
    // GMUM.R changes }

    words = (WORD *)my_malloc(sizeof(WORD)*(max_words_doc+10));
    if(verbosity>=1) {
      printf("Reading examples into memory..."); fflush(stdout);
    }
    dnum=0;
    (*totwords)=0;
    // GMUM.R changes {
    bool newline;
    if (!use_gmumr) {
        newline = (!feof(docfl)) && fgets(line,(int)ll,docfl);
    } else {
        newline = false;
        if (dnum < config.target.n_rows) {
            newline = true;
            std::string str = SVMConfigurationToSVMLightLearnInputLine(config, dnum);
            line = new char[str.size() + 1];
            std::copy(str.begin(), str.end(), line);
            line[str.size()] = '\0';
        }
    }
    while(newline) {
      if (use_gmumr) {
            std::string stringline = "";
      }
      // GMUM.R changes }
      if(line[0] == '#') continue;  /* line contains comments */
      if(!parse_document(line,words,&doc_label,&queryid,&slackid,&costfactor,
                 &wpos,max_words_doc,&comment)) {
        printf("\nParsing error in line %ld!\n%s",dnum,line);
        exit(1);
      }
      (*label)[dnum]=doc_label;
      /* printf("docnum=%ld: Class=%f ",dnum,doc_label); */
      if(doc_label > 0) dpos++;
      if (doc_label < 0) dneg++;
      if (doc_label == 0) dunlab++;
      if((wpos>1) && ((words[wpos-2]).wnum>(*totwords))) 
        (*totwords)=(words[wpos-2]).wnum;
      if((*totwords) > MAXFEATNUM) {
        printf("\nMaximum feature number exceeds limit defined in MAXFEATNUM!\n");
        exit(1);
      }
      (*docs)[dnum] = create_example(dnum,queryid,slackid,costfactor,
                     create_svector(words,comment,1.0));
      /* printf("\nNorm=%f\n",((*docs)[dnum]->fvec)->twonorm_sq);  */
      dnum++;  
      if(verbosity>=1) {
        if((dnum % 100) == 0) {
      printf("%ld..",dnum); fflush(stdout);
        }
      }
      // GMUM.R changes {
      if (!use_gmumr) {
          newline = (!feof(docfl)) && fgets(line,(int)ll,docfl);
      } else {
          newline = false;
          if (dnum < config.target.n_rows) {
              newline = true;
              std::string str = SVMConfigurationToSVMLightLearnInputLine(config, dnum);
              line = new char[str.size() + 1];
              std::copy(str.begin(), str.end(), line);
              line[str.size()] = '\0';
          }
      }
      // GMUM.R changes }
    } 

    if (!use_gmumr) {
        fclose(docfl);
        free(line);
    };
    free(words);
    if(verbosity>=1) {
      fprintf(stdout, "OK. (%ld examples read)\n", dnum);
    }
    (*totdoc)=dnum;
}


std::string SVMLightRunner::SVMConfigurationToSVMLightLearnInputLine(
    SVMConfiguration &config, long int line_num
) {
    LOG(
        config.log,
        LogLevel::DEBUG,
        __debug_prefix__ + ".SVMConfigurationToSVMLightLearnInputLine() Started."
    );

    std::string line_string = "";

    std::ostringstream ss;
    double target_value = config.target[line_num];


    // Handle user-defined labels
    if (target_value == config.neg_target) {
        ss << -1;
    } else if (target_value == config.pos_target) {
        ss << 1;
    } else if (!target_value) {
        ss << 0;
    }

    // Optional feature: cost :)

    if (config.use_example_weights || (config.use_class_weights && target_value)) {
    	double weight = 1.0;
    	if(config.use_example_weights){
    		weight *= config.example_weights[line_num];
    	}
    	if(config.use_class_weights){
    		weight *= (target_value == config.pos_target? config.class_weights[1] : config.class_weights[0]);
    	}

        ss << " cost:" << std::setprecision(8) << weight;

    }

    // Matrix type handling
    if (config.isSparse()) {
    	int current_row = 0;
        arma::sp_mat::iterator begin = config.getSparseData().begin_col(line_num);
        arma::sp_mat::iterator end = config.getSparseData().end_col(line_num);
        for (arma::sp_mat::iterator it = begin; it != end; ++it) {
            ss << ' ' << it.row() + 1 << ':' << std::setprecision(8);
            current_row = it.row();
            ss << *it;
        }
        //Always output last row
        if(current_row != config.getSparseData().n_rows-1){
        	ss << ' ' << config.getSparseData().n_rows << ':' << std::setprecision(8);
        	ss << config.getSparseData()(config.getSparseData().n_rows-1, line_num);
        }

    } else {
        for (long int i = 1; i <= config.data.n_cols; ++i) {
            ss << ' ' << i << ':' << std::setprecision(8);
            ss << config.data(line_num, i-1);
        }
    }

    ss << std::endl;
    line_string = ss.str();

    return line_string;
}


char * SVMLightRunner::SVMConfigurationToSVMLightModelSVLine(
    SVMConfiguration &config, long int line_num
) {
    LOG(
        config.log,
        LogLevel::DEBUG,
        __debug_prefix__ + ".SVMConfigurationToSVMLightModelSVLine() Started."
    );

    std::string line_string = "";

    std::ostringstream ss;
    ss << std::setprecision(32) << config.alpha_y[line_num];
    for (long int i = 1; i <= config.support_vectors.n_rows; ++i) {
        ss << ' ' << i << ':' << std::setprecision(8) << config.support_vectors(i-1, line_num);
    }
    ss << " #" << std::endl;
    line_string = ss.str();

    LOG(
        config.log,
        LogLevel::DEBUG,
        __debug_prefix__ + ".SVMConfigurationToSVMLightModelSVLine() Done."
    );

    return (char*)line_string.c_str();
}


// TODO: Remove
char ** SVMLightRunner::SVMConfigurationToSVMLightModelFile(
    SVMConfiguration &config
) {
    char **result;
    std::string line_string = "";
    //result = malloc(sizeof(char *) * number_of_lines);
}


void SVMLightRunner::SVMLightModelToSVMConfiguration(
    MODEL *model, SVMConfiguration &config
) {
    LOG(
        config.log,
        LogLevel::DEBUG,
        __debug_prefix__ + ".SVMLightModelToSVMConfiguration() Started."
    );

    long i, j, k;
    SVECTOR *v;

    /* 0=linear, 1=poly, 2=rbf, 3=sigmoid, 4=custom -- same as GMUM.R! */
    config.kernel_type = static_cast<KernelType>(model->kernel_parm.kernel_type);
    // -d int      -> parameter d in polynomial kernel
    config.degree = model->kernel_parm.poly_degree;
    // -g float    -> parameter gamma in rbf kernel
    config.gamma = model->kernel_parm.rbf_gamma;
    // -s float    -> parameter s in sigmoid/poly kernel
    config.gamma = model->kernel_parm.coef_lin;
    // -r float    -> parameter c in sigmoid/poly kernel
    config.coef0 = model->kernel_parm.coef_const;
    // -u string   -> parameter of user defined kernel
    config.kernel_parm_custom = model->kernel_parm.custom;
    // highest feature index - no assignment to read-only data
    //config->data.n_cols = model->totwords;
    // number of training documents - no assignment to read-only data
    //config->target.n_rows = model->totdoc;
    // number of support vectors plus 1 (!)
    config.l = model->sv_num - 1;
    // Threshold b (has opposite sign than SVMClient::predict()
    // NOTE: see libraryReadModel()
    config.b = - model->b;

    config.alpha_y = arma::zeros<arma::vec>(config.l);
    config.support_vectors = \
        arma::zeros<arma::sp_mat>(config.l, model->totwords);

    for (i = 1; i < model->sv_num; i++) {
      for (v = model->supvec[i]->fvec; v; v=v->next) {
        config.alpha_y(i - 1) = model->alpha[i]*v->factor;
        for (j = 0; (v->words[j]).wnum; j++) {
            k = (v->words[j]).wnum - 1;
            config.support_vectors(i - 1, k) = v->words[j].weight;
        }
      }
    }
    config.support_vectors = config.support_vectors.t();
    config.w = (config.alpha_y.t() * config.support_vectors.t()).t();
    LOG(
        config.log,
        LogLevel::DEBUG,
        __debug_prefix__ + ".SVMLightModelToSVMConfiguration() Done."
    );
}

void SVMLightRunner::libraryWaitAnyKey()
{
  printf("\n(more)\n");
  (void)getc(stdin);
}


void SVMLightRunner::libraryPrintHelp()
{
  printf("\nSVM-light %s: Support Vector Machine, learning module     %s\n",VERSION,VERSION_DATE);
  copyright_notice();
}

