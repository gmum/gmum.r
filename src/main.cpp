#ifdef INSIDE                 // for the embedded R via RInside
#include <iostream>

#include "svm_basic.h"
#include "libsvm_runner.h"
#include "test_libsvm.h"

struct svm_node ** sparsify(double **x, int r, int c) {
	struct svm_node** sparse;
	int i, ii, count;

	sparse = (struct svm_node **) malloc(r * sizeof(struct svm_node *));
	/* iterate over rows */
	for (i = 0; i < r; i++) {
		/* determine nr. of non-zero elements */
		/* iterate over columns */
		for (count = ii = 0; ii < c; ii++)
			if (x[i][ii] != 0)
				count++;

		/* allocate memory for column elements */
		sparse[i] = (struct svm_node *) malloc(
				(count + 1) * sizeof(struct svm_node));

		/* set column elements */
		for (count = ii = 0; ii < c; ii++)
			if (x[i][ii] != 0) {
				sparse[i][count].index = ii + 1;
				sparse[i][count].value = x[i][ii];
				count++;
			}

		/* set termination element */
		sparse[i][count].index = -1;
	}

	return sparse;
}

using namespace arma;
using namespace std;
int main() {
	double **mat;
	int count = 4;
	mat = (double **) malloc((count + 1) * sizeof(double *));
	for(int i = 0; i < 2; i++) {
		mat[i] = (double *) malloc(2 * sizeof(double));
		for(int j = 0; j < 2; j++) {
			mat[i][j] = i+j;
		}
	}
	int rows, cols;
	rows = cols = 2;
	svm_node ** node = sparsify(mat, rows, cols);
	printf("%d", node[1][0].index);
	printf("%d", node[1][1].value);
	return 0;
}
#endif
