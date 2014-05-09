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
#include <Rcpp.h>

#include "lib_svm_runner.h"
#include "svm.h"
#include "svm_basic.h"

#define Malloc(type,n) (type *)malloc((n)*sizeof(type))
void read_problem(const char *filename, svm_parameter& param);
static char* readline(FILE *input);
void exit_input_error(int line_num);
void predict(FILE *input, FILE *output);
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

SVM_Result LibSVMRunner::processRequest(SVM_Configuration config,
		SVM_Result result) {
	if (!config.isPrediction()) {
		std::string s_filename = config.getFilename();
		const char * filename = s_filename.c_str();
		svm_parameter param = get_default_params();
		read_problem(filename, param);
		processRequest(config, result, param, prob);
	} else {
		svm_predict(config, result);
	}
	return result;
}

bool LibSVMRunner::canHandle(SVM_Configuration config) {
	return true;
}

void LibSVMRunner::processRequest(SVM_Configuration& config, SVM_Result& result,
		svm_parameter& param, svm_problem& problem) {

	const char * model_file_name = config.getModelFilename().c_str();

	const char *error_msg;

	error_msg = svm_check_parameter(&prob, &param);

	if (error_msg) {
		fprintf(stderr, "ERROR: %s\n", error_msg);
		exit(1);
	}

	model = svm_train(&prob, &param);

	if (svm_save_model(model_file_name, model)) {
		fprintf(stderr, "can't save model to file %s\n", model_file_name);
		exit(1);
	}
	svm_free_and_destroy_model(&model);
	svm_destroy_param(&param);
	free(prob.y);
	free(prob.x);
	free(x_space);
	free(line);
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

void LibSVMRunner::svm_predict(SVM_Configuration& config, SVM_Result& result) {

	FILE *input, *output;

	const char * model_filename = config.getModelFilename().c_str();
	const char * input_filename = config.getFilename().c_str();
	const char * output_filename = config.getOutputFilename().c_str();

	input = fopen(input_filename, "r");
	if (input == NULL) {
		fprintf(stderr, "can't open input file %s\n", input_filename);
		exit(1);
	}

	output = fopen(output_filename, "w");
	if (output == NULL) {
		fprintf(stderr, "can't open output file %s\n", output_filename);
		exit(1);
	}

	if ((model = svm_load_model(model_filename)) == 0) {
		fprintf(stderr, "can't open model file %s\n", model_filename);
		exit(1);
	}

	x = (struct svm_node *) malloc(max_nr_attr * sizeof(struct svm_node));
	if (predict_probability) {
		if (svm_check_probability_model(model) == 0) {
			fprintf(stderr, "Model does not support probabiliy estimates\n");
			exit(1);
		}
	} else {
		if (svm_check_probability_model(model) != 0)
			printf(
					"Model supports probability estimates, but disabled in prediction.\n");
	}

	predict(input, output);
}

void predict(FILE *input, FILE *output) {
	int correct = 0;
	int total = 0;
	double error = 0;
	double sump = 0, sumt = 0, sumpp = 0, sumtt = 0, sumpt = 0;

	int svm_type = svm_get_svm_type(model);
	int nr_class = svm_get_nr_class(model);
	double *prob_estimates = NULL;
	int j;

	if (predict_probability) {
		if (svm_type == NU_SVR || svm_type == EPSILON_SVR)
			printf(
					"Prob. model for test data: target value = predicted value + z,\nz: Laplace distribution e^(-|z|/sigma)/(2sigma),sigma=%g\n",
					svm_get_svr_probability(model));
		else {
			int *labels = (int *) malloc(nr_class * sizeof(int));
			svm_get_labels(model, labels);
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
			exit_input_error(total + 1);

		target_label = strtod(label, &endptr);
		if (endptr == label || *endptr != '\0')
			exit_input_error(total + 1);

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
				exit_input_error(total + 1);
			else
				inst_max_index = x[i].index;

			errno = 0;
			x[i].value = strtod(val, &endptr);
			if (endptr == val || errno != 0
					|| (*endptr != '\0' && !isspace(*endptr)))
				exit_input_error(total + 1);

			++i;
		}
		x[i].index = -1;

		if (predict_probability && (svm_type == C_SVC || svm_type == NU_SVC)) {
			predict_label = svm_predict_probability(model, x, prob_estimates);
			fprintf(output, "%g", predict_label);
			for (j = 0; j < nr_class; j++)
				fprintf(output, " %g", prob_estimates[j]);
			fprintf(output, "\n");
		} else {
			predict_label = svm_predict(model, x);
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
		printf("Mean squared error = %g (regression)\n", error / total);
		printf("Squared correlation coefficient = %g (regression)\n",
				((total * sumpt - sump * sumt) * (total * sumpt - sump * sumt))
						/ ((total * sumpp - sump * sump)
								* (total * sumtt - sumt * sumt)));
	} else
		printf("Accuracy = %g%% (%d/%d) (classification)\n",
				(double) correct / total * 100, correct, total);
	if (predict_probability)
		free(prob_estimates);
}

struct svm_parameter get_default_params() {
	svm_parameter param;
	param.svm_type = C_SVC;
	param.kernel_type = RBF;
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

void exit_input_error(int line_num) {
	fprintf(stderr, "Wrong input format at line %d\n", line_num);
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

void read_problem(const char *filename, svm_parameter& param) {
	int elements, max_index, inst_max_index, i, j;
	FILE *fp = fopen(filename, "r");
	char *endptr;
	char *idx, *val, *label;

	if (fp == NULL) {
		fprintf(stderr, "can't open input file %s\n", filename);
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
			exit_input_error(i + 1);

		prob.y[i] = strtod(label, &endptr);
		if (endptr == label || *endptr != '\0')
			exit_input_error(i + 1);

		while (1) {
			idx = strtok(NULL, ":");
			val = strtok(NULL, " \t");

			if (val == NULL)
				break;

			errno = 0;
			x_space[j].index = (int) strtol(idx, &endptr, 10);
			if (endptr == idx || errno != 0 || *endptr != '\0'
					|| x_space[j].index <= inst_max_index)
				exit_input_error(i + 1);
			else
				inst_max_index = x_space[j].index;

			errno = 0;
			x_space[j].value = strtod(val, &endptr);
			if (endptr == val || errno != 0
					|| (*endptr != '\0' && !isspace(*endptr)))
				exit_input_error(i + 1);

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
				fprintf(stderr,
						"Wrong input format: first column must be 0:sample_serial_number\n");
				exit(1);
			}
			if ((int) prob.x[i][0].value <= 0
					|| (int) prob.x[i][0].value > max_index) {
				fprintf(stderr,
						"Wrong input format: sample_serial_number out of range\n");
				exit(1);
			}
		}

	fclose(fp);
}

