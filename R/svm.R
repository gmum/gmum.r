#' Description
#' Hello gmum call!
#' Details
#' Prints out "Hello Gmum"
#' @export
test_libsvm <- function() {
	.Call('rcpp_hello_world', PACKAGE = 'gmum.r')
}