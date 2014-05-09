#ifdef INSIDE
#include <Rcpp.h>
#include <RInside.h>                    // for the embedded R via RInside
#include <iostream>

#include "svm_basic.h"
#include "lib_svm_runner.h"
#include "test_libsvm.h"

using namespace Rcpp;
using namespace std;
int main() {
    test_libsvm();
	return 0;
}
#endif
