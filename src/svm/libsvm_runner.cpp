#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "libsvm_runner.h"
#include "svm_basic.h"
#include "svm_utils.h"

#define Malloc(type,n) (type *)malloc((n)*sizeof(type))
void read_problem(SVMConfiguration& config, const char *filename, svm_parameter& param);
static char* readline(FILE *input);
void exit_input_error(SVMConfiguration& config, int line_num);
void predict(SVMConfiguration& config, FILE *input, FILE *output);
svm_parameter get_default_params();

//struct svm_parameter param;		// set by parse_command_line
struct svm_problem prob;		// set by read_problem
struct svm_model *model;
struct svm_node *x_space;

static char *line = NULL;
static int max_line_len;

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
		if (config.getFilename().empty()) {
			prob.l = config.target.n_rows;
			svm_node** node = armatlib(config.data);
			prob.y = vectlib(config.target);
			prob.x = node;
		} else {
			std::string s_filename = config.getFilename();
			const char * filename = s_filename.c_str();
//			read_problem(filename, param);
		}
//		Real training using svm
		if (config.getModelFilename().empty()) {
			save_model_to_config(config, param, prob);
		} else {
			save_model_to_file(config, param, prob);
		}
	} else {
		if (config.getFilename().empty()) {
			arma_prediction(config);
		} else {
			file_prediction(config);
		}
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
		LOG(config.log, LogLevel::Error, "ERROR: " + to_string(error_msg))
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

	config.SV = (svm_node **) malloc(config.l * sizeof(svm_node*));
	for (int i = 0; i < config.l; i++) {
		config.SV[i] = (svm_node*) malloc(sizeof(svm_node));
		memcpy(config.SV, model->SV, sizeof(svm_node));
	}

	//	TODO: WTF!!!!!???
	if (config.svm_type < 2) {
		config.label = (int *) malloc(*nclasses * sizeof(int));
		config.nSV = (int *) malloc(*nclasses * sizeof(int));
		memcpy(config.label, model->label, *nclasses * sizeof(int));
		memcpy(config.nSV, model->nSV, *nclasses * sizeof(int));
	}

	svm_destroy_param(param, config.log);
	svm_free_and_destroy_model(&model, config.log);

	return true;
}

svm_model* LibSVMRunner::load_model_from_config(SVMConfiguration& config,
		svm_parameter* param) {

	const char *error_msg;
	error_msg = svm_check_parameter(&prob, param, config.log);

	if (error_msg) {
		LOG(config.log, LogLevel::Error, "ERROR: " + to_string(error_msg))
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

	model->SV = (svm_node **) malloc(config.l * sizeof(svm_node*));
	for (int i = 0; i < config.l; i++) {
		model->SV[i] = (svm_node*) malloc(sizeof(svm_node));
		memcpy(model->SV, config.SV, sizeof(svm_node));
	}

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

void LibSVMRunner::save_model_to_file(SVMConfiguration& config,
		svm_parameter* param, svm_problem& problem) {

	const char * model_file_name = config.getModelFilename().c_str();

	const char *error_msg;

	error_msg = svm_check_parameter(&prob, param, config.log);

	if (error_msg) {
		LOG(config.log, LogLevel::Error, "ERROR: " + to_string(error_msg))
		exit(1);
	}

	model = svm_train(&prob, param, config.log);

	if (svm_save_model(model_file_name, model, config.log)) {
		LOG(config.log, LogLevel::Error, "can't save model to file " + to_string(model_file_name))
		exit(1);
	}
	svm_free_and_destroy_model(&model, config.log);
	svm_destroy_param(param, config.log);
	free(prob.y);
	free(prob.x);
	free(x_space);
	free(line);
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

void LibSVMRunner::file_prediction(SVMConfiguration& config) {

	FILE *input, *output;

	const char * model_filename = config.getModelFilename().c_str();
	const char * input_filename = config.getFilename().c_str();
	const char * output_filename = config.getOutputFilename().c_str();

	input = fopen(input_filename, "r");
	if (input == NULL) {
		LOG(config.log, LogLevel::Error, "can't open input file " + to_string(input_filename))
		exit(1);
	}

	output = fopen(output_filename, "w");
	if (output == NULL) {
		LOG(config.log, LogLevel::Error, "can't open output file " + to_string(output_filename))
		exit(1);
	}

	if ((model = svm_load_model(model_filename, config.log)) == 0) {
		LOG(config.log, LogLevel::Error, "can't open model file " + to_string(model_filename));
		exit(1);
	}

	x = (struct svm_node *) malloc(max_nr_attr * sizeof(struct svm_node));
	if (predict_probability) {
		if (svm_check_probability_model(model, config.log) == 0) {
			LOG(config.log, LogLevel::Error, "Model does not support probabiliy estimates");
			exit(1);
		}
	} else {
		if (svm_check_probability_model(model, config.log) != 0)
			LOG(config.log, LogLevel::Info, "Model supports probability estimates, but disabled in prediction.");
	}

	predict(config, input, output);

}

void predict(SVMConfiguration& config, FILE *input, FILE *output) {
	int correct = 0;
	int total = 0;
	double error = 0;
	double sump = 0, sumt = 0, sumpp = 0, sumtt = 0, sumpt = 0;

	int svm_type = svm_get_svm_type(model, config.log);
	int nr_class = svm_get_nr_class(model, config.log);
	double *prob_estimates = NULL;
	int j;

	if (predict_probability) {
		if (svm_type == NU_SVR || svm_type == EPSILON_SVR) {
			LOG(config.log, LogLevel::Info, "Prob. model for test data: target value = predicted value + z,\nz: Laplace distribution e^(-|z|/sigma)/(2sigma),sigma="
							+ to_string(svm_get_svr_probability(model, config.log)));
		}
		else {
			int *labels = (int *) malloc(nr_class * sizeof(int));
			svm_get_labels(model, labels, config.log);
			prob_estimates = (double *) malloc(nr_class * sizeof(double));
			fprintf(output, "labels");
			for (j = 0; j < nr_class; j++)
				fprintf(output, " %d", labels[j]);
			fprintf(output, "\n");
			free(labels);
		}
	}

	max_line_len = 1024;
	line = (char *) malloc(max_line_len * sizeof(char));
	while (readline(input) != NULL) {
		int i = 0;
		double target_label, predict_label;
		char *idx, *val, *label, *endptr;
		int inst_max_index = -1; // strtol gives 0 if wrong format, and precomputed kernel has <index> start from 0

		label = strtok(line, " \t\n");
		if (label == NULL) // empty line
			exit_input_error(config, total + 1);

		target_label = strtod(label, &endptr);
		if (endptr == label || *endptr != '\0')
			exit_input_error(config, total + 1);

		while (1) {
			if (i >= max_nr_attr - 1)	// need one more for index = -1
					{
				max_nr_attr *= 2;
				x = (struct svm_node *) realloc(x,
						max_nr_attr * sizeof(struct svm_node));
			}

			idx = strtok(NULL, ":");
			val = strtok(NULL, " \t");

			if (val == NULL)
				break;
			errno = 0;
			x[i].index = (int) strtol(idx, &endptr, 10);
			if (endptr == idx || errno != 0 || *endptr != '\0'
					|| x[i].index <= inst_max_index)
				exit_input_error(config, total + 1);
			else
				inst_max_index = x[i].index;

			errno = 0;
			x[i].value = strtod(val, &endptr);
			if (endptr == val || errno != 0
					|| (*endptr != '\0' && !isspace(*endptr)))
				exit_input_error(config, total + 1);

			++i;
		}
		x[i].index = -1;

		if (predict_probability && (svm_type == C_SVC || svm_type == NU_SVC)) {
			predict_label = svm_predict_probability(model, x, prob_estimates, config.log);
			fprintf(output, "%g", predict_label);
			for (j = 0; j < nr_class; j++)
				fprintf(output, " %g", prob_estimates[j]);
			fprintf(output, "\n");
		} else {
			predict_label = svm_predict(model, x, config.log);
			fprintf(output, "%g\n", predict_label);
		}

		if (predict_label == target_label)
			++correct;
		error += (predict_label - target_label)
				* (predict_label - target_label);
		sump += predict_label;
		sumt += target_label;
		sumpp += predict_label * predict_label;
		sumtt += target_label * target_label;
		sumpt += predict_label * target_label;
		++total;
	}
	if (svm_type == NU_SVR || svm_type == EPSILON_SVR) {
		LOG(config.log, LogLevel::Info, "Mean squared error = " + to_string(error / total) + "(regression)");
		LOG(config.log, LogLevel::Info, "Squared correlation coefficient = "
				+ to_string(((total * sumpt - sump * sumt) * (total * sumpt - sump * sumt))
				/ ((total * sumpp - sump * sump)
				* (total * sumtt - sumt * sumt))) + "(regression)");
	} else
		LOG(config.log, LogLevel::Info, "Accuracy = " + to_string((double) correct / total * 100) + "(classification)");
	if (predict_probability)
		free(prob_estimates);
}

struct svm_parameter get_default_params() {
	svm_parameter param;
	param.svm_type = C_SVC;
	param.kernel_type = LINEAR;
	param.degree = 3;
	param.gamma = 0;	// 1/num_features
	param.coef0 = 0;
	param.nu = 0.5;
	param.cache_size = 100;
	param.C = 1;
	param.eps = 1e-3;
	param.p = 0.1;
	param.shrinking = 1;
	param.probability = 0;
	param.nr_weight = 0;
	param.weight_label = NULL;
	param.weight = NULL;
	return param;
}

void exit_input_error(SVMConfiguration& config, int line_num) {
	LOG(config.log, LogLevel::Error, "Wrong input format at line " + to_string(line_num));
	exit(1);
}

static char* readline(FILE *input) {
	int len;

	if (fgets(line, max_line_len, input) == NULL)
		return NULL;

	while (strrchr(line, '\n') == NULL) {
		max_line_len *= 2;
		line = (char *) realloc(line, max_line_len);
		len = (int) strlen(line);
		if (fgets(line + len, max_line_len - len, input) == NULL)
			break;
	}
	return line;
}

void read_problem(SVMConfiguration& config, const char *filename, svm_parameter& param) {
	int elements, max_index, inst_max_index, i, j;
	FILE *fp = fopen(filename, "r");
	char *endptr;
	char *idx, *val, *label;

	if (fp == NULL) {
		LOG(config.log, LogLevel::Error, "Wcan't open input file " + to_string(filename));
		exit(1);
	}

	prob.l = 0;
	elements = 0;

	max_line_len = 1024;
	line = Malloc(char, max_line_len);
	while (readline(fp) != NULL) {
		char *p = strtok(line, " \t"); // label

		// features
		while (1) {
			p = strtok(NULL, " \t");
			if (p == NULL || *p == '\n') // check '\n' as ' ' may be after the last feature
				break;
			++elements;
		}
		++elements;
		++prob.l;
	}
	rewind(fp);

	prob.y = Malloc(double, prob.l);
	prob.x = Malloc(struct svm_node *, prob.l);
	x_space = Malloc(struct svm_node, elements);

	max_index = 0;
	j = 0;
	for (i = 0; i < prob.l; i++) {
		inst_max_index = -1; // strtol gives 0 if wrong format, and precomputed kernel has <index> start from 0
		readline(fp);
		prob.x[i] = &x_space[j];
		label = strtok(line, " \t\n");
		if (label == NULL) // empty line
			exit_input_error(config, i + 1);

		prob.y[i] = strtod(label, &endptr);
		if (endptr == label || *endptr != '\0')
			exit_input_error(config, i + 1);

		while (1) {
			idx = strtok(NULL, ":");
			val = strtok(NULL, " \t");

			if (val == NULL)
				break;

			errno = 0;
			x_space[j].index = (int) strtol(idx, &endptr, 10);
			if (endptr == idx || errno != 0 || *endptr != '\0'
					|| x_space[j].index <= inst_max_index)
				exit_input_error(config, i + 1);
			else
				inst_max_index = x_space[j].index;

			errno = 0;
			x_space[j].value = strtod(val, &endptr);
			if (endptr == val || errno != 0
					|| (*endptr != '\0' && !isspace(*endptr)))
				exit_input_error(config, i + 1);

			++j;
		}

		if (inst_max_index > max_index)
			max_index = inst_max_index;
		x_space[j++].index = -1;
	}

	if (param.gamma == 0 && max_index > 0)
		param.gamma = 1.0 / max_index;

	if (param.kernel_type == PRECOMPUTED)
		for (i = 0; i < prob.l; i++) {
			if (prob.x[i][0].index != 0) {
				LOG(config.log, LogLevel::Error, "Wrong input format: first column must be 0:sample_serial_number");
				exit(1);
			}
			if ((int) prob.x[i][0].value <= 0
					|| (int) prob.x[i][0].value > max_index) {
				LOG(config.log, LogLevel::Error, "Wrong input format: sample_serial_number out of range");
				exit(1);
			}
		}

	fclose(fp);
}

void LibSVMRunner::arma_prediction(SVMConfiguration& config) {
	struct svm_model* m;
	struct svm_node ** train;
	svm_parameter *params;
	arma::mat training_mat = config.data;
	int training_examples = training_mat.n_rows;

	if (config.model_filename.empty()) {
		params = configuration_to_problem(config);
		m = load_model_from_config(config, params);
	} else {
		const char * model_filename = config.getModelFilename().c_str();
		if ((m = svm_load_model(model_filename, config.log)) == 0) {
//			config.error_msg = std::string("Can't open model name. Please provide name for a model");
			return;
		}
	}

//	TODO: READ MODEL FROM PARAMETERS

	train = armatlib(config.data);
	double* ret = Malloc(double, training_examples);

	for (int i = 0; i < training_examples; i++)
		ret[i] = svm_predict(m, train[i], config.log);

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
	//TODO: THIS SHOULD WORKING WITH PREDICTIONS 2X, now it's not working
//	svm_free_and_destroy_model(&m);
	svm_destroy_param(params, config.log);
	free(ret);
}

