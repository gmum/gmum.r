#ifdef INSIDE
#include <Rcpp.h>
#include <RInside.h>                    // for the embedded R via RInside

#include "svm_basic.h"
#include "lib_svm_runner.h"

using namespace Rcpp;
using namespace std;
int main() {
    using namespace Rcpp ;

    CharacterVector x = CharacterVector::create( "foo", "bar" )  ;
    NumericVector y   = NumericVector::create( 0.0, 1.0 ) ;
    List z            = List::create( x, y ) ;


	return 0;
}
#endif
