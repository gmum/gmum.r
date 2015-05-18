#include "svm_utils.h"
arma::sp_mat SvmUtils::SvmNodeToArmaSpMat(
    svm_node** svm_nodes, int nr_sv, int dim
) {
    arma::uvec colptr(nr_sv+1); //this always has this dim
    colptr(0) = 0; //always

    int non_zero = 0;
    //get necessary statistics 
    for (int row = 0; row < nr_sv; row++) {
        int j = 0;
        for (j = 0; svm_nodes[row][j].index != -1; j++) ;
        non_zero += j;
    }


    arma::uvec rowind(non_zero);
    arma::vec values(non_zero);

    int current = 0;
    for (int row = 0; row < nr_sv; row++) {
        svm_node* tmp_row = svm_nodes[row];
        for (int j = 0; tmp_row[j].index != -1; j++) {
            rowind[current] = tmp_row[j].index - 1;
            values[current] = tmp_row[j].value;
            ++current;
        }
        colptr(row+1) = current;
    }

    //Transposed matrix of SV
    return arma::sp_mat(rowind, colptr, values, dim, nr_sv); 
}
