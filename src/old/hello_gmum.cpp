#include "hello_gmum.h"

SEXP hello_gmum(){
    using namespace Rcpp ;
    
    CharacterVector x = CharacterVector::create( "hello", "gmum" )  ;

    return x ;
}
