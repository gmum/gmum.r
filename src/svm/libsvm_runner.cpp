/*
 * LibSVMRunner.cpp
 *
 *  Created on: Apr 7, 2014
 *      Author: sacherus
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "libsvm_runner.h"
#include "svm_basic.h"
#include "svm_utils.h"

#define Malloc(type,n) (type *)malloc((n)*sizeof(type))
svm_parameter get_default_params();


LibSVMRunner::LibSVMRunner() {
	// TODO Auto-generated constructor stub
}

LibSVMRunner::~LibSVMRunner() {
	// TODO Auto-generated destructor stub
}

void LibSVMRunner::processRequest(SVMConfiguration& config) {

//	Training
	if (!config.isPrediction()) {
		svm_parameter* param = configuration_to_problem(config);
		prob.l = config.target.n_rows;
		svm_node** node = armatlib(config.data);
		prob.y = vectlib(config.target);
		prob.x = node;
		save_model_to_config(config, param, prob);
		//Xk is already transposed
		//examples x dim
		arma::vec arma_coefs = SvmUtils::arrtoarmavec(config.sv_coef, config.l);
		//DIM W: (nsv x 1)^T x nsv x dim = 1 x dim
		config.w = (arma_coefs.t() * config.arma_SV).t();
	} else {
		arma_prediction(config);
	}
}

bool LibSVMRunner::canHandle(SVMConfiguration& config) {
	return config.library == LIBSVM;
}

bool LibSVMRunner::save_model_to_config(SVMConfiguration& config,
		svm_parameter* param, svm_problem& problem) {

	const char *error_msg;

	error_msg = svm_check_parameter(&prob, param, config.log);

	if (error_msg) {
		LOG(config.log, LogLevel::ERR, "ERROR: " + to_string(error_msg))
		return false;
	}
	int* nr = Malloc(int, 1);
	int* nclasses = Malloc(int, 1);

	model = svm_train(&prob, param, config.log);
	*nr = model->l; //support vectors
	*nclasses = model->nr_class;
	config.nr_class = model->nr_class;
	config.l = model->l;
	//TODO: don't keep support vectors as svm node, remember when Staszek wasn't happy about it?
	config.sv_coef = (double **) malloc(model->nr_class * sizeof(double*));
	for (int i = 0; i < config.nr_class - 1; i++) {
		config.sv_coef[i] = (double *) malloc(model->l * sizeof(double));
		memcpy(config.sv_coef[i * config.l], model->sv_coef[i],
				config.l * sizeof(double*));
	}

	config.rho = (double *) malloc(
			config.nr_class * (config.nr_class - 1) / 2 * sizeof(double));
	memcpy(config.rho, model->rho,
			config.nr_class * (config.nr_class - 1) / 2 * sizeof(double));

	config.sv_indices = (int*) malloc(config.l * sizeof(int));
	svm_get_sv_indices(model, config.sv_indices, config.log);

	int dim = config.data.n_cols;
	ASSERT(dim > 0);
	config.arma_SV = SvmUtils::libtoarma(model->SV, config.l, dim);

	// config.SV = (svm_node **) malloc(config.l * sizeof(svm_node*));
	// for (int i = 0; i < config.l; i++) {
	// 	config.SV[i] = (svm_node*) malloc(sizeof(svm_node));
	// 	memcpy(config.SV, model->SV, sizeof(svm_node));
	// }

	//	TODO: WTF!!!!!???
	if (config.svm_type < 2) {
		config.label = (int *) malloc(*nclasses * sizeof(int));
		config.nSV = (int *) malloc(*nclasses * sizeof(int));
		memcpy(config.label, model->label, *nclasses * sizeof(int));
		memcpy(config.nSV, model->nSV, *nclasses * sizeof(int));
	}

	svm_destroy_param(param,config.log);
	svm_free_and_destroy_model(&model,config.log);

	return true;
}

svm_model* LibSVMRunner::load_model_from_config(SVMConfiguration& config,
		svm_parameter* param) {

	const char *error_msg;
	error_msg = svm_check_parameter(&prob, param,config.log);

	if (error_msg) {
		LOG(config.log, LogLevel::ERR, "ERROR: " + to_string(error_msg))
		return 0;
	}

	model = Malloc(svm_model, 1);

	model->l = config.l; //support vectors
	model->nr_class = config.nr_class;
	model->param = *param;

	//TODO: don't keep support vectors as svm node, remember when Staszek wasn't happy about it?
	model->sv_coef = (double **) malloc(model->nr_class * sizeof(double*));
	for (int i = 0; i < config.nr_class - 1; i++) {
		model->sv_coef[i] = (double *) malloc(model->l * sizeof(double));
		memcpy(model->sv_coef[i * config.l], config.sv_coef[i],
				config.l * sizeof(double*));
	}

	// model->SV = (svm_node **) malloc(config.l * sizeof(svm_node*));
	// for (int i = 0; i < config.l; i++) {
	// 	model->SV[i] = (svm_node*) malloc(sizeof(svm_node));
	// 	memcpy(model->SV, config.SV, sizeof(svm_node));
	// }

	model->SV = armatlib(config.arma_SV);

	model->rho = (double *) malloc(
			config.nr_class * (config.nr_class - 1) / 2 * sizeof(double));

	memcpy(model->rho, config.rho,
			config.nr_class * (config.nr_class - 1) / 2 * sizeof(double));

	model->free_sv = 1;

//TODO: check if neccessery
//	model->sv_indices =(int*) malloc( config.l * sizeof(int));
//	model->sv_indicessvm_get_sv_indices(model, config.sv_indices);

	if (config.svm_type < 2) {
		model->label = (int *) malloc(config.nr_class * sizeof(int));
		model->nSV = (int *) malloc(config.nr_class * sizeof(int));
		memcpy(model->label, config.label, config.nr_class * sizeof(int));
		memcpy(model->nSV, config.nSV, config.nr_class * sizeof(int));
	}

	return model;
}

svm_parameter* LibSVMRunner::configuration_to_problem(
		SVMConfiguration& config) {
	svm_parameter* param;
	param = Malloc(svm_parameter, 1);
	param->svm_type = config.svm_type;
	// param->kernel_type = config.kernel_type;
	param->degree = config.degree;
	param->gamma = config.gamma;	// 1/num_features
	param->coef0 = config.coef0;
	param->nu = config.nu;
	param->cache_size = config.cache_size;
	param->C = config.C;
	param->eps = config.eps;
	param->p = config.p;
	param->shrinking = config.shrinking;
	param->probability = config.probability;
	param->nr_weight = config.nr_weight;
	param->weight_label = config.weight_label;
	param->weight = config.weight;

	if ( config.kernel_type == _LINEAR ) {
			param->kernel_type = LINEAR;
		}
		else if ( config.kernel_type == _POLY ) {
			param->kernel_type = POLY;
		}
		else if ( config.kernel_type == _RBF ) {
			param->kernel_type = RBF;
		}
		else if ( config.kernel_type == _SIGMOID ) {
			param->kernel_type = SIGMOID;
		}
	return param;
}

/*
 * Prediction
 */

struct svm_node *x;
int max_nr_attr = 64;

int predict_probability = 0;

/*
 struct svm_model* model;
 static char *line = NULL;
 static int max_line_len;
 */

/*
 * Armadillo matrix format to libsvm format
 */
struct svm_node ** LibSVMRunner::armatlib(arma::mat x) {
	int r = x.n_rows;
	int c = x.n_cols;
	struct svm_node** sparse;
	int i, ii, count;

	sparse = (struct svm_node **) malloc(r * sizeof(struct svm_node *));
	/* iterate over rows */
	for (i = 0; i < r; i++) {
		/* determine nr. of non-zero elements */
		/* iterate over columns */
		for (count = ii = 0; ii < c; ii++)
			if (x(i, ii) != 0)
				count++;

		/* allocate memory for column elements */
		sparse[i] = (struct svm_node *) malloc(
				(count + 1) * sizeof(struct svm_node));

		/* set column elements */
		for (count = ii = 0; ii < c; ii++)
			if (x(i, ii) != 0) {
				sparse[i][count].index = ii + 1;
				sparse[i][count].value = x(i, ii);
				count++;
			}

		/* set termination element */
		sparse[i][count].index = -1;
	}

	return sparse;
}

/*
 * Vector with target to lisvm format
 */
double * LibSVMRunner::vectlib(arma::vec target) {
	double* return_target;
	return_target = Malloc(double, target.n_rows);
	for (unsigned int i = 0; i < target.n_rows; i++) {
		return_target[i] = target(i);
	}
	return return_target;
}




void LibSVMRunner::arma_prediction(SVMConfiguration& config) {
	struct svm_model* m;
	struct svm_node ** train;
	svm_parameter *params;
	arma::mat training_mat = config.data;
	int training_examples = training_mat.n_rows;

	params = configuration_to_problem(config);
	m = load_model_from_config(config, params);

//	TODO: READ MODEL FROM PARAMETERS

	train = armatlib(config.data);
	double* ret = Malloc(double, training_examples);

	for (int i = 0; i < training_examples; i++)
		ret[i] = svm_predict(m, train[i],config.log);

	arma::vec ret_vec(ret, training_examples);
	config.result = ret_vec;
	/* TODO: CLEAN MEMORY IN BETTER WAY THINK OF OTHER PARAMETERS
	 * Clean memory:
	 * -array matrix
	 * -model
	 */
	for (int i = 0; i < training_examples; i++)
		free(train[i]);
	free(train);
	//TODO: THIS SHOULD WORK WITH PREDICTIONS 2X, now it's not working
//	svm_free_and_destroy_model(&m);
	svm_destroy_param(params,config.log);
	free(ret);
}

