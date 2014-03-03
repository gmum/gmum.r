#' Description
#' Hello gmum call!
#' Details
#' Prints out "Hello Gmum"
#' @export
hello_gmum <- function(){
	.Call( "hello_gmum", PACKAGE = "gmum.r" )
}

