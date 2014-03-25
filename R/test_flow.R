#' Description
#' Testing architecture flow
#' Details
#' On success it should print out the argument.
#' @export
test_flow <- function(msg){
  .Call( "testFlow", msg, PACKAGE = "gmum.r" )
}
