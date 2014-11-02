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

bool SVMLightRunner::canHandle(SVMConfiguration &config) {
    /// @TODO SVMLightRunner::canHandle
    return true;
}

void SVMLightRunner::processRequest(SVMConfiguration &config) {
    if (!config.isPrediction()) {
        char **argv;
        librarySVMLearnMain(0, argv, true, config);
    }
}

/* Library functionalities wrappers */

void   wait_any_key();
void   print_help();

char docfile[200];           /* file with training examples */
char modelfile[200];         /* file for resulting classifier */
char restartfile[200];       /* file with initial alphas */

int SVMLightRunner::librarySVMLearnMain(
    int argc, char **argv, bool use_gmumr, SVMConfiguration &config
) {
    DOC **docs;  /* training examples */
    long totwords,totdoc,i;
    double *target;
    double *alpha_in=NULL;
    KERNEL_CACHE *kernel_cache;
    LEARN_PARM learn_parm;
    KERNEL_PARM kernel_parm;
    MODEL *model=(MODEL *)my_malloc(sizeof(MODEL));

    // GMUM.R changes {
    libraryReadInputParameters(
        argc, argv, docfile, modelfile, restartfile, &verbosity, &learn_parm,
        &kernel_parm, use_gmumr, config
    );
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
    write_model(modelfile,model);

    free(alpha_in);
    free_model(model,0);
    for(i=0;i<totdoc;i++) 
      free_example(docs[i],1);
    free(docs);
    free(target);

    return(0);
}

void SVMLightRunner::libraryReadInputParameters(
    int argc, char *argv[], char *docfile, char *modelfile, char *restartfile,
    long *verbosity, LEARN_PARM *learn_parm, KERNEL_PARM *kernel_parm,
    bool use_gmumr, SVMConfiguration &config
) {
    long i;
    char type[100];
  
    /* set default */
    strcpy (modelfile, "svm_model");
    strcpy (learn_parm->predfile, "trans_predictions");
    strcpy (learn_parm->alphafile, "");
    strcpy (restartfile, "");
    (*verbosity)=1;
    learn_parm->biased_hyperplane=1;
    learn_parm->sharedslack=0;
    learn_parm->remove_inconsistent=0;
    learn_parm->skip_final_opt_check=0;
    learn_parm->svm_maxqpsize=10;
    learn_parm->svm_newvarsinqp=0;
    learn_parm->svm_iter_to_shrink=-9999;
    learn_parm->maxiter=100000;
    learn_parm->kernel_cache_size=40;
    learn_parm->svm_c=0.0;
    learn_parm->eps=0.1;
    learn_parm->transduction_posratio=-1.0;
    learn_parm->svm_costratio=1.0;
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

    for(i=1;(i<argc) && ((argv[i])[0] == '-');i++) {
      switch ((argv[i])[1]) 
        { 
        case '?': print_help(); exit(0);
        case 'z': i++; strcpy(type,argv[i]); break;
        case 'v': i++; (*verbosity)=atol(argv[i]); break;
        case 'b': i++; learn_parm->biased_hyperplane=atol(argv[i]); break;
        case 'i': i++; learn_parm->remove_inconsistent=atol(argv[i]); break;
        case 'f': i++; learn_parm->skip_final_opt_check=!atol(argv[i]); break;
        case 'q': i++; learn_parm->svm_maxqpsize=atol(argv[i]); break;
        case 'n': i++; learn_parm->svm_newvarsinqp=atol(argv[i]); break;
        case '#': i++; learn_parm->maxiter=atol(argv[i]); break;
        case 'h': i++; learn_parm->svm_iter_to_shrink=atol(argv[i]); break;
        case 'm': i++; learn_parm->kernel_cache_size=atol(argv[i]); break;
        case 'c': i++; learn_parm->svm_c=atof(argv[i]); break;
        case 'w': i++; learn_parm->eps=atof(argv[i]); break;
        case 'p': i++; learn_parm->transduction_posratio=atof(argv[i]); break;
        case 'j': i++; learn_parm->svm_costratio=atof(argv[i]); break;
        case 'e': i++; learn_parm->epsilon_crit=atof(argv[i]); break;
        case 'o': i++; learn_parm->rho=atof(argv[i]); break;
        case 'k': i++; learn_parm->xa_depth=atol(argv[i]); break;
        case 'x': i++; learn_parm->compute_loo=atol(argv[i]); break;
        case 't': i++; kernel_parm->kernel_type=atol(argv[i]); break;
        case 'd': i++; kernel_parm->poly_degree=atol(argv[i]); break;
        case 'g': i++; kernel_parm->rbf_gamma=atof(argv[i]); break;
        case 's': i++; kernel_parm->coef_lin=atof(argv[i]); break;
        case 'r': i++; kernel_parm->coef_const=atof(argv[i]); break;
        case 'u': i++; strcpy(kernel_parm->custom,argv[i]); break;
        case 'l': i++; strcpy(learn_parm->predfile,argv[i]); break;
        case 'a': i++; strcpy(learn_parm->alphafile,argv[i]); break;
        case 'y': i++; strcpy(restartfile,argv[i]); break;
        default: printf("\nUnrecognized option %s!\n\n",argv[i]);
	         print_help();
	         exit(0);
        }
    }
    // GMUM.R changes {
    if(!use_gmumr) {
        if(i>=argc) {
            printf("\nNot enough input parameters!\n\n");
            wait_any_key();
            print_help();
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
      wait_any_key();
      print_help();
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
      wait_any_key();
      print_help();
      exit(0);
    }    
    if((learn_parm->svm_maxqpsize<2)) {
      printf("\nMaximum size of QP-subproblems not in valid range: %ld [2..]\n",learn_parm->svm_maxqpsize); 
      wait_any_key();
      print_help();
      exit(0);
    }
    if((learn_parm->svm_maxqpsize<learn_parm->svm_newvarsinqp)) {
      printf("\nMaximum size of QP-subproblems [%ld] must be larger than the number of\n",learn_parm->svm_maxqpsize); 
      printf("new variables [%ld] entering the working set in each iteration.\n",learn_parm->svm_newvarsinqp); 
      wait_any_key();
      print_help();
      exit(0);
    }
    if(learn_parm->svm_iter_to_shrink<1) {
      printf("\nMaximum number of iterations for shrinking not in valid range: %ld [1,..]\n",learn_parm->svm_iter_to_shrink);
      wait_any_key();
      print_help();
      exit(0);
    }
    if(learn_parm->svm_c<0) {
      printf("\nThe C parameter must be greater than zero!\n\n");
      wait_any_key();
      print_help();
      exit(0);
    }
    if(learn_parm->transduction_posratio>1) {
      printf("\nThe fraction of unlabeled examples to classify as positives must\n");
      printf("be less than 1.0 !!!\n\n");
      wait_any_key();
      print_help();
      exit(0);
    }
    if(learn_parm->svm_costratio<=0) {
      printf("\nThe COSTRATIO parameter must be greater than zero!\n\n");
      wait_any_key();
      print_help();
      exit(0);
    }
    if(learn_parm->epsilon_crit<=0) {
      printf("\nThe epsilon parameter must be greater than zero!\n\n");
      wait_any_key();
      print_help();
      exit(0);
    }
    if(learn_parm->rho<0) {
      printf("\nThe parameter rho for xi/alpha-estimates and leave-one-out pruning must\n");
      printf("be greater than zero (typically 1.0 or 2.0, see T. Joachims, Estimating the\n");
      printf("Generalization Performance of an SVM Efficiently, ICML, 2000.)!\n\n");
      wait_any_key();
      print_help();
      exit(0);
    }
    if((learn_parm->xa_depth<0) || (learn_parm->xa_depth>100)) {
      printf("\nThe parameter depth for ext. xi/alpha-estimates must be in [0..100] (zero\n");
      printf("for switching to the conventional xa/estimates described in T. Joachims,\n");
      printf("Estimating the Generalization Performance of an SVM Efficiently, ICML, 2000.)\n");
      wait_any_key();
      print_help();
      exit(0);
    }
}

void SVMLightRunner::libraryReadDocuments (
    char *docfile, DOC ***docs, double **label, long int *totwords,
    long int *totdoc, bool use_gmumr, SVMConfiguration &config
) {
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
        max_words_doc = config.data.n_cols;
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
            //std::string stringline = "";
            //line = (char*)stringline.c_str();
            line = svmConfigurationToSVMLightInputLine(config, dnum);
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
        printf("LINE: %s\n",line);
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
              line = svmConfigurationToSVMLightInputLine(config, dnum);
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


char * SVMLightRunner::svmConfigurationToSVMLightInputLine(
    SVMConfiguration &config, long int line_num
) {
    std::string line_string = "";

    std::ostringstream ss;
    ss << config.target[line_num];
    for (long int j = 1; j <= config.data.n_cols; ++j) {
        ss << ' ' << j << ':' << config.data(line_num, j-1);
    }
    ss << std::endl;
    ss << "  ";
    line_string = ss.str();

    return (char*)line_string.c_str();
}

void wait_any_key()
{
  printf("\n(more)\n");
  (void)getc(stdin);
}

void print_help()
{
  printf("\nSVM-light %s: Support Vector Machine, learning module     %s\n",VERSION,VERSION_DATE);
  copyright_notice();
}

