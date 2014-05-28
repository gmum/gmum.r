#' Description
#' Testing architecture flow
#' Details
#' On success it should print out the argument.
#' @export
test_libsvm <- function(){
	.Call( "test_libsvm", PACKAGE = "gmum.r" )
}
