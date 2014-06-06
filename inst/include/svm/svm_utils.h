/*
 * utils.h
 *
 *  Created on: Jun 6, 2014
 *      Author: sacherus
 */

#ifndef UTILS_H_
#define UTILS_H_
#include <svm.h>
//#include <RcppArmadillo.h>
#include <armadillo>

class SvmUtils {
private:
	SvmUtils();
	virtual ~SvmUtils();
public:
	//convert sparse matrix to armadillo matrix
	static arma::vec libtoarma(svm_node** svm_nodes, int nr_sv, int dim) {
		arma::mat ret(nr_sv, dim);
		int j;
		for(int row=0; row < nr_sv; row++) {
			j = 0;
			svm_node* tmp_row = svm_nodes[row];
			while(tmp_row[j++].value != -1) {
				ret(row, tmp_row[j].index) = tmp_row[j].value;
			}
		}
		return ret;
	}
};

#endif /* UTILS_H_ */
