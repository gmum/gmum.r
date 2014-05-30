#ifdef INSIDE                 // for the embedded R via RInside
#include <iostream>

#include "svm_basic.h"
#include "libsvm_runner.h"
#include "test_libsvm.h"

#define Malloc(type,n) (type *)malloc((n)*sizeof(type))



using namespace arma;
using namespace std;

int main() {
	test_libsvm_arma();
}
#endif
