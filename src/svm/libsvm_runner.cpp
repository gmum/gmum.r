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

void LibSVMRunner::ProcessRequest(SVMConfiguration& config) {

//	Training
	if (!config.prediction()) {
		svm_parameter* param = ConfigurationToProblem(config);
		prob_.l = config.target_.n_rows;
		svm_node** node = Armatlib(config.data_);
		prob_.y = Vectlib(config.target_);
		prob_.x = node;
		SaveModelToConfig(config, param, prob_);
		//Xk is already transposed
		//examples x dim
		arma::vec arma_coefs = SvmUtils::ArrToArmaVec(config.sv_coef_, config.l_);
		//DIM W: (nsv x 1)^T x nsv x dim = 1 x dim
		config.w_ = (arma_coefs.t() * config.arma_sv_).t();
	} else {
		ArmaPrediction(config);
	}
}

bool LibSVMRunner::CanHandle(SVMConfiguration& config) {
	return config.library_ == kLibSMV;
}

bool LibSVMRunner::SaveModelToConfig(SVMConfiguration& config,
		svm_parameter* param, svm_problem& problem) {

	const char *error_msg;

	error_msg = svm_check_parameter(&prob_, param, config.log_);

	if (error_msg) {
		LOG(config.log_, LogLevel::kError, "ERROR: " + ToString(error_msg))
		return false;
	}
	int* nr = Malloc(int, 1);
	int* nclasses = Malloc(int, 1);

	model_ = svm_train(&prob_, param, config.log_);
	*nr = model_->l; //support vectors
	*nclasses = model_->nr_class;
	config.nr_class_ = model_->nr_class;
	config.l_ = model_->l;
	//TODO: don't keep support vectors as svm node, remember when Staszek wasn't happy about it?
	config.sv_coef_ = (double **) malloc(model_->nr_class * sizeof(double*));
	for (int i = 0; i < config.nr_class_ - 1; i++) {
		config.sv_coef_[i] = (double *) malloc(model_->l * sizeof(double));
		memcpy(config.sv_coef_[i * config.l_], model_->sv_coef[i],
				config.l_ * sizeof(double*));
	}

	config.rho_ = (double *) malloc(
			config.nr_class_ * (config.nr_class_ - 1) / 2 * sizeof(double));
	memcpy(config.rho_, model_->rho,
			config.nr_class_ * (config.nr_class_ - 1) / 2 * sizeof(double));

	config.sv_indices_ = (int*) malloc(config.l_ * sizeof(int));
	svm_get_sv_indices(model_, config.sv_indices_, config.log_);

	int dim = config.data_.n_cols;
	ASSERT(dim > 0);
	config.arma_sv_ = SvmUtils::libtoarma(model_->SV, config.l_, dim);

	// config.SV = (svm_node **) malloc(config.l * sizeof(svm_node*));
	// for (int i = 0; i < config.l; i++) {
	// 	config.SV[i] = (svm_node*) malloc(sizeof(svm_node));
	// 	memcpy(config.SV, model->SV, sizeof(svm_node));
	// }

	//	TODO: WTF!!!!!???
	if (config.svm_type_ < 2) {
		config.label_ = (int *) malloc(*nclasses * sizeof(int));
		config.n_sv_ = (int *) malloc(*nclasses * sizeof(int));
		memcpy(config.label_, model_->label, *nclasses * sizeof(int));
		memcpy(config.n_sv_, model_->nSV, *nclasses * sizeof(int));
	}

	svm_destroy_param(param,config.log_);
	svm_free_and_destroy_model(&model_,config.log_);

	return true;
}

svm_model* LibSVMRunner::LoadModelFromConfig(SVMConfiguration& config,
		svm_parameter* param) {

	const char *error_msg;
	error_msg = svm_check_parameter(&prob_, param,config.log_);

	if (error_msg) {
		LOG(config.log_, LogLevel::kError, "ERROR: " + ToString(error_msg))
		return 0;
	}

	model_ = Malloc(svm_model, 1);

	model_->l = config.l_; //support vectors
	model_->nr_class = config.nr_class_;
	model_->param = *param;

	//TODO: don't keep support vectors as svm node, remember when Staszek wasn't happy about it?
	model_->sv_coef = (double **) malloc(model_->nr_class * sizeof(double*));
	for (int i = 0; i < config.nr_class_ - 1; i++) {
		model_->sv_coef[i] = (double *) malloc(model_->l * sizeof(double));
		memcpy(model_->sv_coef[i * config.l_], config.sv_coef_[i],
				config.l_ * sizeof(double*));
	}

	// model->SV = (svm_node **) malloc(config.l * sizeof(svm_node*));
	// for (int i = 0; i < config.l; i++) {
	// 	model->SV[i] = (svm_node*) malloc(sizeof(svm_node));
	// 	memcpy(model->SV, config.SV, sizeof(svm_node));
	// }

	model_->SV = Armatlib(config.arma_sv_);

	model_->rho = (double *) malloc(
			config.nr_class_ * (config.nr_class_ - 1) / 2 * sizeof(double));

	memcpy(model_->rho, config.rho_,
			config.nr_class_ * (config.nr_class_ - 1) / 2 * sizeof(double));

	model_->free_sv = 1;

//TODO: check if neccessery
//	model->sv_indices =(int*) malloc( config.l * sizeof(int));
//	model->sv_indicessvm_get_sv_indices(model, config.sv_indices);

	if (config.svm_type_ < 2) {
		model_->label = (int *) malloc(config.nr_class_ * sizeof(int));
		model_->nSV = (int *) malloc(config.nr_class_ * sizeof(int));
		memcpy(model_->label, config.label_, config.nr_class_ * sizeof(int));
		memcpy(model_->nSV, config.n_sv_, config.nr_class_ * sizeof(int));
	}

	return model_;
}

svm_parameter* LibSVMRunner::ConfigurationToProblem(
		SVMConfiguration& config) {
	svm_parameter* param;
	param = Malloc(svm_parameter, 1);
	param->svm_type = config.svm_type_;
	// param->kernel_type = config.kernel_type;
	param->degree = config.degree_;
	param->gamma = config.gamma_;	// 1/num_features
	param->coef0 = config.coef0_;
	param->nu = config.nu_;
	param->cache_size = config.cache_size_;
	param->C = config.c_;
	param->eps = config.eps_;
	param->p = config.p_;
	param->shrinking = config.shrinking_;
	param->probability = config.probability_;
	param->nr_weight = config.nr_weight_;
	param->weight_label = config.weight_label_;
	param->weight = config.weight_;

	if ( config.kernel_type_ == kLinear ) {
			param->kernel_type = LINEAR;
		}
		else if ( config.kernel_type_ == kPoly ) {
			param->kernel_type = POLY;
		}
		else if ( config.kernel_type_ == kRBF ) {
			param->kernel_type = RBF;
		}
		else if ( config.kernel_type_ == kSigmoid ) {
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
struct svm_node ** LibSVMRunner::Armatlib(arma::mat x) {
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
double * LibSVMRunner::Vectlib(arma::vec target) {
	double* return_target;
	return_target = Malloc(double, target.n_rows);
	for (unsigned int i = 0; i < target.n_rows; i++) {
		return_target[i] = target(i);
	}
	return return_target;
}




void LibSVMRunner::ArmaPrediction(SVMConfiguration& config) {
	struct svm_model* m;
	struct svm_node ** train;
	svm_parameter *params;
	arma::mat training_mat = config.data_;
	int training_examples = training_mat.n_rows;

	params = ConfigurationToProblem(config);
	m = LoadModelFromConfig(config, params);

//	TODO: READ MODEL FROM PARAMETERS

	train = Armatlib(config.data_);
	double* ret = Malloc(double, training_examples);

	for (int i = 0; i < training_examples; i++)
		ret[i] = svm_predict(m, train[i],config.log_);

	arma::vec ret_vec(ret, training_examples);
	config.result_ = ret_vec;
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
	svm_destroy_param(params,config.log_);
	free(ret);
}

